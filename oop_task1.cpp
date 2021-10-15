#include <iostream>
#include <fstream>
using namespace std;

class basic_str {
protected:
    char* str_value;
    int str_length;
    void copy(const basic_str& str) {
        str_length = strlen(str.str_value);
        str_value = new char[str_length + 1];
        strcpy(str_value, str.str_value);
    }
public:

    // конструктор по умолчанию для строки нулевой длины
    basic_str() {
        str_value = new char[1];
        str_value[0] = '\0';
        str_length = 0;
    }

    // конструктор строки единичной длины
    basic_str(char c) {
        str_length = 1;
        str_value = new char[2];
        str_value[0] = c;
        str_value[1] = '\0';
    }

    // копия строки
    basic_str(const basic_str& str) {
        copy(str);
    }

    // конструктор принимающий zero-terminated строку
    basic_str(const char* s) {
        str_length = strlen(s);
        str_value = new char[str_length + 1];
        strcpy(str_value, s);
    }

    // деструктор
    ~basic_str() {
        delete[] str_value;
    }

    // пусть типичный размер строки - 16 символов
    int buffer_size = 16;

    // добавление символа в конец строки
    void push_back(char c) {
        if (str_length + 2 >= buffer_size) {
            buffer_size *= 2; }
        char* new_str = new char[buffer_size];
        if (str_length > 0) {
            strcpy(new_str, str_value);
        }
        str_value = new_str;
        str_value[str_length++] = c;
        str_value[str_length] = '\0';
    }

    // ввод строки
    void input_str(istream& stream) {
        char c;
        delete[] str_value; // очищаем строку
        str_value = new char[buffer_size];
        while (stream.get(c) && c != '\n') {
            push_back(c);
        }
    }

    // вывод строки
    void output_str(ostream& stream) {
        stream << "" << str_value << endl;
    }

    // перегружаем =
    basic_str& operator= (const basic_str& str) {
        if (this != &str) {
            delete [] str_value;
            copy(str);
        }
        return *this;
    }

    // конкатенация строк (перегружаем +=)
    void operator+= (const basic_str& summand) {
        for (int i = 0; i < summand.str_length; i++) {
            push_back(summand.str_value[i]);
        }
    }

    // конкатенация строк (перегружаем +)
    friend basic_str operator+ (const basic_str& summand1, const basic_str& summand2) {
        basic_str sum(summand1);
        for (int i = 0; i < summand2.str_length; i++) {
            sum.push_back(summand2.str_value[i]);
        }
        return sum;
    }

    // взятие элемента по индексу (перегружаем [])
    char& operator[] (const int index) {
        if (index > str_length || index < 0) {
            cerr << "Index out of range: index = "<< index;
            throw out_of_range("out_of_range exception");
        }
            return str_value[index];
    }

    // взятие подстроки (перегружаем скобочки)
    basic_str operator() (const int index_first, const int index_last) {
        basic_str newstr;
        if (index_first > str_length || index_first < 0 || index_last > str_length || index_last < 0) {
            cerr << "Index of substring is out of range: index = "<< index_first << " and " << index_last << endl;
            throw out_of_range("out_of_range exception");
        }
        newstr.str_value = new char[index_last - index_first + 2];
            for (int i = index_first; i <= index_last; i++) {
                newstr.push_back(str_value[i]);
            }
        return newstr;
    }

    // дупликация строки (перегружаем *)
    basic_str operator* (int count) {
        basic_str multiplied;
        for (int i = 0; i < count; i++) {
            multiplied += str_value;
        }
        return multiplied;
    }

    // перегружаем ==
    bool operator== (const basic_str& str1) {
        bool flag = 1;
            for (int i = 0; i < str_length; i++) {
                if (str1.str_value[i] != str_value[i]) {
                    flag = 0;
                    break;
                }
            }
        return flag;
    }

    // удаление всех вхождений подстроки из строки (перегружаем -)
    basic_str operator- (const basic_str& subtrahend) {
        basic_str minuend(*this);
        basic_str diff;
        basic_str substr;
        for (int i = 0; i < str_length; i++) {
            if (i <= str_length - subtrahend.str_length) {
                substr = minuend(i, subtrahend.str_length - 1 + i); //подстрока
            }
            if (substr == subtrahend) {
                i += subtrahend.str_length;
                i--;
                substr.str_value = new char[1]; // очистка подстроки
                continue;
            }
                diff.push_back(str_value[i]);
        }
        return diff;
    }

    // замена вхождений подстроки
    basic_str replace(const basic_str& str1, const basic_str& str2) {
        basic_str substr;
        basic_str newstr;
        basic_str cpy(*this);
        for (int i = 0; i < str_length; i++) {
            if (i <= str_length - str1.str_length) {
                substr = cpy(i, str1.str_length - 1 + i);
            }
            if (substr == str1) {
                i += str1.str_length;
                i--;
                newstr+=str2;
                substr.str_value = new char[1];
                continue;
            }
            newstr.push_back(str_value[i]);
        }
        return newstr;
    }
};

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    basic_str str1, str2, str3;
    str1.input_str(fin);
    str2.input_str(fin);
    str3.input_str(fin);

    str1 = str1.replace(str2, str3);
    str1.output_str(fout);

    fin.close();
    fout.close();
    return 0;
}
