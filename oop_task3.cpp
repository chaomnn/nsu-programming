#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <utility>
#include <limits>

using namespace std;

template <typename T>
bool compare(T a, T b) {
    return (a == b);
}

template<> bool compare(double a, double b) {
    return fabs(a - b) < numeric_limits<double>::epsilon();
}

template <typename K, typename V>
class HashMap {
    static const int default_size = 8;
    constexpr static const double rehash_factor = 0.6;

    int capacity;
    int size;
    int u_val_counter;

    pair<K, V>** table;
    pair<K, V>* deleted;

    void Rehash() {
        capacity *= 2;
        size = 0;

        auto** new_table = new pair<K, V>*[capacity];
        for (auto i = 0; i < capacity; i++) {
            new_table[i] = nullptr;
        }
        swap(table, new_table);
        for (auto i = 0; i < (capacity/2); i++) {
            if (new_table[i] && new_table[i] != deleted) {
                Insert(new_table[i]->first, new_table[i]->second);
            }
        }
        for (auto i = 0; i < (capacity/2); i++) {
            if (new_table[i] && new_table[i] != deleted) {
                delete new_table[i];
            }
        }
    }

public:

    HashMap() {
        capacity = default_size;
        size = 0;
        u_val_counter = 0;
        table = new pair<K, V>*[capacity];
        for (auto i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
        deleted = new pair<K, V>();
    }

    ~HashMap() {
        for (auto i = 0; i < capacity; i++) {
            if (table[i] && table[i] != deleted) {
                delete table[i];
            }
        }
        delete deleted;
        delete[] table;
    }

    void Insert(const K& add_key, const V& value) {
        if (size + 1 > rehash_factor*capacity) {
            Rehash();
        }
        size_t hash_v = hash<K>{}(add_key) % capacity;
        if (table[hash_v] != nullptr && table[hash_v] != deleted && !(compare(add_key, table[hash_v]->first))) { // если случилась коллизия
            while (table[hash_v] != nullptr) {
                hash_v++;
                hash_v %= capacity;
            }
        }
        if (table[hash_v] != nullptr && (compare(add_key, table[hash_v]->first))) { // если ключи одинаковые
            table[hash_v]->second = value;
        }
        else {
            table[hash_v] = new pair<K, V>(add_key, value);
            size++;
        }
    }

    void Remove(const K& delete_key) {
        size_t hash_v = hash<K>{}(delete_key) % capacity;
        while (table[hash_v] != nullptr) {
            if (compare(delete_key, table[hash_v]->first)) {
                table[hash_v] = deleted;
                size--;
                break;
            }
            hash_v++;
            hash_v %= capacity;
        }
    }

    V Find(const K& find_key) {
        size_t hash_v = hash<K>{}(find_key) % capacity;
        while (table[hash_v] != nullptr) {
            if (compare(find_key, table[hash_v]->first)) {
                return table[hash_v]->second;
            }
            hash_v++;
            hash_v %= capacity;
        }
        return deleted->second; // возвращает пустое значение, если ничего не найдено
    }

    int getsize() {
        return size;
    }

    int get_unique() {
        u_val_counter = 0;
        int repeat = 0;
        for (Iterator it = begin(); it != end(); ++it) {
            bool flag = true;
            int elem_repeat = 0;
            for (Iterator other = begin(); other != end(); ++other) {
                if (elem_repeat++ > repeat && it != other && (compare(it.elem->second, other.elem->second))) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                u_val_counter++;
            }
            repeat++;
        }
        return u_val_counter;
    }

    void parse(const int& num_of_commands, istream& pstream) {
        char command;
        K key;
        V value;
        for (int i = 0; i < num_of_commands; i++) {
            pstream >> command >> key;
            if (command == 'A') {
                pstream >> value;
                Insert(key, value);
            }
            else {
                Remove(key);
            }
        }
    }
    class Iterator {
        friend class HashMap;
        pair<K, V>* elem;
        HashMap<K, V>* map;
        int index;
        Iterator(): elem(nullptr), map(nullptr), index(-1) {}
        Iterator(HashMap<K, V>* map, int ind): elem(nullptr), map(map), index(-1) {
            for (int i = ind; i < map->capacity; i++) {
                if (map->table[i] != nullptr && map->table[i] != map->deleted) {
                    index = i;
                    elem = map->table[i];
                    break;
                }
            }
        }
    public:
        Iterator(HashMap<K, V>* map, pair<K, V>* elem, int index): elem(elem), map(map), index(index) {}
        Iterator(const Iterator& iter): elem(iter.elem), map(iter.map), index(iter.index) {}
        Iterator& operator++() {
            if (*this == map->last()) {
                *this = map->end();
                return *this;
            }
            index++;
            while ((map->table[index] == nullptr || map->table[index] == map->deleted) && index < map->capacity) {
                index++;
            }
            elem = map->table[index];
            *this = Iterator(map, elem, index);
            return *this;
        };
        bool operator!=(Iterator const& other) const {
            return elem != other.elem;
        }
        bool operator==(Iterator const& other) const {
            return elem == other.elem;
        }
        Iterator& operator=(const Iterator& iter) {
            if (this == &iter) {
                return *this;
            }
            map = iter.map;
            elem = iter.elem;
            index = iter.index;
            return *this;
        }
    };

    Iterator begin() {
        return Iterator(this, 0);
    };

    Iterator last() {
        for (int i = capacity-1; i > 0; i--) {
            if (table[i] && table[i] != deleted) {
                return Iterator(this, table[i], i);
            }
        }
        return Iterator(this, 0);
    };

    Iterator end() {
        return Iterator();
    }
};

void createHashMap(char key_type, char value_type, int n, istream& stream, ostream& ostream) {
    if (key_type == 'I') {
        if (value_type == 'I') {
            auto* my_map = new HashMap<int, int>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'D') {
            auto* my_map = new HashMap<int, double>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'S') {
            auto* my_map = new HashMap<int, string>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
    }
    else if (key_type == 'D') {
        if (value_type == 'I') {
            auto* my_map = new HashMap<double, int>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'D') {
            auto* my_map = new HashMap<double, double>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'S') {
            auto* my_map = new HashMap<double, string>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
    }
    else if (key_type == 'S') {
        if (value_type == 'I') {
            auto* my_map = new HashMap<string, int>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'D') {
            auto* my_map = new HashMap<string, double>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
        else if (value_type == 'S') {
            auto* my_map = new HashMap<string, string>;
            my_map->parse(n, stream);
            ostream << my_map->getsize() << ' ' << my_map->get_unique();
            delete my_map;
        }
    }
}
int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int n;
    char key_type, value_type;
    fin >> key_type >> value_type >> n;

    createHashMap(key_type, value_type, n, fin, fout);

    fin.close();
    fout.close();
    return 0;
}
