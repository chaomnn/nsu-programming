#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

class Expression {
public:
    string contents;
    virtual void print() {
        cout << contents;
    }
    virtual int eval(const string& values) = 0; // вычисление значения
    virtual Expression* derivative(const string& vrbl) = 0; // дифференцирование
};

class Number: public Expression {
    int value;
public:
    Number(const int num) {
        value = num;
        contents = to_string(num);
    }
    int eval(const string& values) {
        return  value;
    }
    Expression* derivative(const string& vrbl) {
        return new Number(0);
    }
};

class Variable: public Expression {
    string value;
public:
    Variable(const string& str) {
        value = str;
        contents = str;
    }
    int eval(const string& values) {
        size_t pos = values.find(value);
        string sub = values.substr(pos);
        size_t pos2 = sub.find("<-");
        pos2+=2;
        return stoi(sub.substr(pos2));
    }
    Expression* derivative(const string& vrbl) {
        if (value == vrbl) {
            return new Number(1);
        }
        return this;
    }
};

class Add: public Expression {
    Expression *val1, *val2;
public:
    Add(Expression *summand1, Expression *summand2): val1(summand1), val2(summand2) {
        contents = "(" + val1->contents + "+" + val2->contents + ")";
    }
    int eval(const string& values) {
        int res = val1->eval(values) + val2->eval(values);
        return res;
    }
    Expression* derivative(const string& vrbl) {
        return new Add(val1->derivative(vrbl), val2->derivative(vrbl));
    }
};

class Sub: public Expression {
    Expression *val1, *val2;
public:
    Sub(Expression *subtrahend, Expression *minuend): val1(subtrahend), val2(minuend) {
        contents = "(" + val1->contents + "-" + val2->contents + ")";
    }
    int eval(const string& values) {
        int res = val1->eval(values) - val2->eval(values);
        return res;
    }
    Expression* derivative(const string& vrbl) {
        return new Sub(val1->derivative(vrbl), val2->derivative(vrbl));
    }
};

class Mul: public Expression {
    Expression *val1, *val2;
public:
    Mul(Expression *mult1, Expression *mult2): val1(mult1), val2(mult2) {
        contents = "(" + val1->contents + "*" + val2->contents + ")";
    }
    int eval(const string& values) {
        int res = val1->eval(values) * val2->eval(values);
        return res;
    }
    Expression* derivative(const string& vrbl) {
        return new Add(new Mul(val1->derivative(vrbl), val2), new Mul(val1, val2->derivative(vrbl)));
    }
};

class Div: public Expression{
    Expression *val1, *val2;
public:
    Div(Expression *divd1, Expression *divd2): val1(divd1), val2(divd2) {
        contents = "(" + val1->contents + "/" + val2->contents + ")";
    }
    int eval(const string& values) {
        int res = val1->eval(values) / val2->eval(values);
        return res;
    }
    Expression* derivative(const string& vrbl) {
        return new Add(new Div(val1->derivative(vrbl), val2), new Div(val1, val2->derivative(vrbl)));
    }
};

// парсер для чисел и переменных
Expression* simple_parse(const string& token_str) {
    if (isdigit(token_str[0])) {
        return new Number(stoi(token_str));
    }
    if (isalpha(token_str[0])) {
        return new Variable(token_str);
    }
};

// парсер для операторов
Expression* operator_parse(char op, Expression* exp1, Expression* exp2) {
    switch(op) {
        case '+':
            return new Add(exp1, exp2);
        case '-':
            return new Sub(exp1, exp2);
        case '*':
            return new Mul(exp1, exp2);
        case '/':
            return new Div(exp1, exp2);
        default:
            return nullptr;
    }
}

// определение приоритета операторов
int precedence(char op){
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    return 0;
};

// проверка на оператора
bool isoperator(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/') {
        return true;
    }
    return false;
};

Expression* parse(const string& token_str) {
    stack<char> operator_stack; // стек операторов
    stack<Expression*> output; // стек выражений
    string long_token;
    Expression* temp1, *temp2;

    for (int i = 0; i < token_str.size(); i++) {
        // если токен - число или переменная
        if (isdigit(token_str[i]) || isalpha(token_str[i])) {
            while ((isdigit(token_str[i]) || isalpha(token_str[i]))) {
                long_token += token_str[i];
                i++;
            }
            output.push(simple_parse(long_token));
            long_token.clear();
            i--;
            if (i != token_str.size()-1) { // если токен - не последний элемент
                continue;
            }
        }
        // если токен - оператор
        if (isoperator(token_str[i])) {
            while ((!operator_stack.empty()) && (operator_stack.top() != '(') && (precedence(operator_stack.top()) >= precedence(token_str[i]))) {
                temp2 = output.top();
                output.pop();
                temp1 = output.top();
                output.pop();
                output.push(operator_parse(operator_stack.top(), temp1, temp2));
                operator_stack.pop();
            }
            operator_stack.push(token_str[i]);
            continue;
        }
        // если токен - открывающая скобка
        if (token_str[i] == '(') {
            operator_stack.push(token_str[i]);
            continue;
        }
        // если токен - закрывающая скобка
        if (token_str[i] == ')') {
            while (operator_stack.top() != '(') {
                temp2 = output.top();
                output.pop();
                temp1 = output.top();
                output.pop();
                output.push(operator_parse(operator_stack.top(), temp1, temp2));
                operator_stack.pop();
            }
            operator_stack.pop();
            continue;
        }
        // если остались операторы
        while (!operator_stack.empty()) {
            temp2 = output.top();
            output.pop();
            temp1 = output.top();
            output.pop();
            output.push(operator_parse(operator_stack.top(), temp1, temp2));
            operator_stack.pop();
        }
    }
    return output.top();
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    string inp_str;
    fin >> inp_str;
    Expression *e = parse(inp_str);
    Expression *de = e->derivative("x");
    fout << de->contents;

    fin.close();
    fout.close();
    return 0;
}
