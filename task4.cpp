#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#define Y "YES"
#define N "NO"

using namespace std;

/* незаконченная реализация детерминированного/недетерменированного конечного автомата */
class StateMachine {
    int states_num, init_state, current_state;
    unordered_multimap<char, pair<int, int>> func_map;
    unordered_set<int> finite_states;
    bool is_deterministic = true;
    vector<int> non_det_states; // всевозможные состояния для недетерминированных автоматов
public:
    StateMachine(int states_num, int init_state):
    states_num(states_num), init_state(init_state), current_state(init_state) {}
    ~StateMachine() {}

    // если мы имеем НКА, и можем прийти в конечное состояние хотя бы 1 способом, то вывод yes
    // поэтому в приоритете функции, которые приводят автомат в конечное состояние
    void state_transition(char function) {
        int temp_state;
        auto range = func_map.equal_range(function);
        int counter = 0;
        for (auto it = range.first; it != range.second; ++it) {
            if (current_state == it->second.first) {
                temp_state = it->second.second;
                counter++;
                if (counter > 1) {
                    // если автомат недетерминированный
                    is_deterministic = false;
                    non_det_states.push_back(it->second.second);
                }
            }
        }
        current_state = temp_state;
    }

    void parse_states(int fin_states_num, istream& stream) {
        int state;
        for (auto i = 0; i < fin_states_num; i++) {
            stream >> state;
            if (state < states_num) {
                finite_states.insert(state);
            }
        }
    }

    void parse_func(int funct_num, istream& stream) {
        char func_name;
        int from, to;
        for (auto i = 0; i < funct_num; i++) {
            stream >> from >> to >> func_name;
            func_map.emplace(func_name, make_pair(from, to));
        }
    }

    bool is_finite(int state) { // проверка на конечность состояния
        auto search = finite_states.find(state);
        return (search != finite_states.end());
    }

    void test_parse(string& str) {
        for (auto pos = 0; pos < str.size(); pos++) {
            char func = str[pos];
            state_transition(func);
            if (!is_deterministic) {
                vector<int> this_func_states = non_det_states;
                non_det_states.clear();
                string sub = str.substr(pos+1);
                for (auto v : this_func_states) {
                    current_state = v;
                    // если это не последняя функция в строке
                    if (pos < str.size()-1) {
                        test_parse(sub);
                    }
                    if (is_finite(current_state)) {
                        this_func_states.clear();
                        return;
                    }
                } // for end
                this_func_states.clear();
            } // if end
        } // for end
    }

    void parse_strings(int strings_num, istream& stream, ostream& ostream) {
        string input_string;
        for (auto i = 0; i < strings_num; i++) {
            stream >> input_string;
            test_parse(input_string);
            if (is_finite(current_state)) {
                ostream << Y << endl;
            }
            else {
                ostream << N << endl;
            }
            current_state = init_state;
        }
    }
};

void machine_parse(istream& inpstream, ostream& outstream) {
    int states_num, init_state, fin_states_num, funct_num, strings_num;

    inpstream >> states_num >> init_state >> fin_states_num;
    StateMachine simple_machine(states_num, init_state);
    simple_machine.parse_states(fin_states_num, inpstream);

    inpstream >> funct_num;
    simple_machine.parse_func(funct_num, inpstream);

    inpstream >> strings_num;
    simple_machine.parse_strings(strings_num, inpstream, outstream);
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    machine_parse(fin, fout);

    fin.close();
    fout.close();
    return 0;
}
