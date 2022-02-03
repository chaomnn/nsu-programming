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
    constexpr static double rehash_factor = 0.6;

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
                insert(new_table[i]->first, new_table[i]->second);
            }
        }
        for (auto i = 0; i < (capacity/2); i++) {
            if (new_table[i] && new_table[i] != deleted) {
                delete new_table[i];
            }
        }
        delete[] new_table;
    }

    size_t get_hash(K key) {
        return hash<K>{}(key) % capacity;
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

    void insert(const K& key, const V& value) {
        if (size + 1 > rehash_factor*capacity) {
            Rehash();
        }
        size_t hash_v = get_hash(key); // ниже используется компаратор, а не ==, тк ключи могут быть даблами
        if (table[hash_v] != nullptr && table[hash_v] != deleted && !(compare(key, table[hash_v]->first))) { // если случилась коллизия
            while (table[hash_v] != nullptr) {
                hash_v++;
                hash_v %= capacity;
            }
        }
        if (table[hash_v] != nullptr && (compare(key, table[hash_v]->first))) { // если ключи одинаковые
            table[hash_v]->second = value;
        }
        else {
            table[hash_v] = new pair<K, V>(key, value);
            size++;
        }
    }

    void remove(const K& key) {
        size_t hash_v = get_hash(key);
        while (table[hash_v] != nullptr) {
            if (compare(key, table[hash_v]->first)) {
                table[hash_v] = deleted;
                size--;
                break;
            }
            hash_v++;
            hash_v %= capacity;
        }
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
                insert(key, value);
            }
            else {
                remove(key);
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
            if (index >= map->capacity-1) {
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

    Iterator find(const K& key) {
        size_t hash_v = get_hash(key);
        while (table[hash_v] != nullptr) {
            if (compare(key, table[hash_v]->first)) {
                return Iterator(this, hash_v);
            }
            hash_v++;
            hash_v %= capacity;
        }
        return end(); // возвращает конечный итератор, если ничего не найдено
    }
};

template<typename K, typename V>
void print(HashMap<K, V>* map, int n, istream& stream, ostream& ostream) {
    map->parse(n, stream);
    ostream << map->getsize() << ' ' << map->get_unique();
    delete map;
}

template <typename V> void read_key_type(char key_type, int n, istream& stream, ostream& ostream) {
    if (key_type == 'I') {
        auto* my_map = new HashMap<int, V>;
        print(my_map, n, stream, ostream);
    }
    else if (key_type == 'D') {
        auto* my_map = new HashMap<double, V>;
        print(my_map, n, stream, ostream);
    }
    else {
        auto* my_map = new HashMap<string, V>;
        print(my_map, n, stream, ostream);
    }
}

void create_map(char key_type, char value_type, int n, istream& stream, ostream& ostream) {
    if (value_type == 'I') {
        read_key_type<int>(key_type, n, stream, ostream);
    }
    else if (value_type == 'D') {
        read_key_type<double>(key_type, n, stream, ostream);
    }
    else if (value_type == 'S') {
        read_key_type<string>(key_type, n, stream, ostream);
    }
}

int main() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    int n;
    char key_type, value_type;
    fin >> key_type >> value_type >> n;

    create_map(key_type, value_type, n, fin, fout);

    fin.close();
    fout.close();
    return 0;
}
