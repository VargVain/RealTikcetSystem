#ifndef TICKETSYSTEM_DATABASE_H
#define TICKETSYSTEM_DATABASE_H

#include "BPTree.h"

template<class K, class V>
class Database {
private:
    BPlusTree<K, int> tree;
    std::fstream _file;
public:
    int n = -1;
    Database() = default;
    Database(const std::string &BPTName, const std::string &fileName) : tree(BPTName) {
        _file.open(fileName);
        if (!_file) {
            _file.open(fileName, std::fstream::out);
            _file.close();
            _file.open(fileName);
            _file.seekp(0);
            _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&n), sizeof(n));
        }
    }
    ~Database() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&n), sizeof(n));
        _file.close();
    }

    void add(const K &key, const V &value) {
        tree.insert(key, ++n);
        _file.seekp(4 + n * sizeof(V));
        _file.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    int found(const K &key, V &dest) {
        if (tree.find(key).empty()) return -1;
        int ret = tree.find(key)[0];
        _file.seekg(4 + ret * sizeof(V));
        _file.read(reinterpret_cast<char *>(&dest), sizeof(dest));
        return ret;
    }

    vector<int> range(const K &key) {
        return tree.find(key);
    }

    void get(int it, V &dest) {
        _file.seekg(4 + it * sizeof(V));
        _file.read(reinterpret_cast<char *>(&dest), sizeof(dest));
    }

    V find(const K &key) {
        V value;
        get((tree.find(key))[0], value);
        return value;
    }

    void erase(const K &key) {
        tree.erase(key, tree.find(key)[0]);
    }

    void erase(const K &key, const V &value) {
        auto f = tree.find(key);
        V dest;
        for (auto it : f) {
            _file.seekg(4 + it * sizeof(V));
            _file.read(reinterpret_cast<char *>(&dest), sizeof(dest));
            if (dest == value) {
                tree.erase(key, it);
                return;
            }
        }
    }

    void modify(const K &key, const V &newValue) {
        int ret = tree.find(key)[0];
        _file.seekp(4 + ret * sizeof(V));
        _file.write(reinterpret_cast<const char *>(&newValue), sizeof(newValue));
    }

    void modify(const K &key, const V &value, const V &newValue) {
        auto f = tree.find(key);
        V dest;
        for (auto it : f) {
            _file.seekg(4 + it * sizeof(V));
            _file.read(reinterpret_cast<char *>(&dest), sizeof(dest));
            if (dest == value) {
                _file.seekp(4 + it * sizeof(V));
                _file.write(reinterpret_cast<const char *>(&newValue), sizeof(newValue));
                return;
            }
        }
    }

    void modify(int it, const V &newValue, int p) {
        _file.seekp(4 + it * sizeof(V));
        _file.write(reinterpret_cast<const char *>(&newValue), sizeof(newValue));
    }
};

#endif //TICKETSYSTEM_DATABASE_H
