#ifndef TICKETSYSTEM_BPTREE_H
#define TICKETSYSTEM_BPTREE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "Vector.h"
#include "DataType.h"

const int MaxSize = 200, MinSize = 100;

template<class K, class V>
struct mPair {
    K key;
    V val;

    mPair() = default;

    mPair(const K &k, const V &v) {
        key = k;
        val = v;
    }

    mPair &operator=(const mPair &x) {
        if (this == &x) return *this;
        key = x.key;
        val = x.val;
        return *this;
    }
};

template<class K, class V>
bool operator==(const mPair<K, V> &a, const mPair<K, V> &b) {
    return a.key == b.key && a.val == b.val;
}

template<class K, class V>
bool operator!=(const mPair<K, V> &a, const mPair<K, V> &b) {
    return !(a == b);
}

template<class K, class V>
bool operator<(const mPair<K, V> &a, const mPair<K, V> &b) {
    return (a.key < b.key) || (a.key == b.key && a.val < b.val);
}

template<class K, class V>
bool operator>(const mPair<K, V> &a, const mPair<K, V> &b) {
    return !(a.key < b.key) && !(a.key == b.key);
}

template<class K, class V>
bool operator<=(const mPair<K, V> &a, const mPair<K, V> &b) {
    return !(a > b);
}

template<class K, class V>
bool operator>=(const mPair<K, V> &a, const mPair<K, V> &b) {
    return !(a < b);
}

template<class K, class V>
struct Node {
    int isLeaf = 1;
    int siz = 0;
    int nxt = -1;
    mPair<K, V> data[MaxSize + 5];
    int chd[MaxSize + 5];
};

template<class K, class V>
class BPlusTree {
private:
    int size = -1, last = -1;
    std::fstream _file;
    std::string _filename;
    int bufferPool[5000];

    int getPos() {
        if (bufferPool[0]) {
            int ret = bufferPool[bufferPool[0]--];
            last = ret;
            return ret;
        }
        last = (++size);
        return size;
    }

    inline int LastPos() {
        return last;
    }

    inline void Pool(int num) {
        bufferPool[++bufferPool[0]] = num;
    }

    void getNode(int pos, Node<K, V> &nd) {
        _file.seekg(pos * sizeof(Node<K, V>) + sizeof(int) * 2 + sizeof(bufferPool));
        _file.read(reinterpret_cast<char *>(&nd), sizeof(nd));
    }

    void writeNode(int pos, const Node<K, V> &nd) {
        _file.seekp(pos * sizeof(Node<K, V>) + sizeof(int) * 2 + sizeof(bufferPool));
        _file.write(reinterpret_cast<const char *>(&nd), sizeof(nd));
    }

    void readHead(int pos, Node<K, V> &nd) {
        _file.seekg(pos * sizeof(Node<K, V>) + sizeof(int) * 2 + sizeof(bufferPool));
        _file.read(reinterpret_cast<char *>(&(nd.isLeaf)), sizeof(nd.isLeaf));
        _file.read(reinterpret_cast<char *>(&(nd.siz)), sizeof(nd.siz));
        _file.read(reinterpret_cast<char *>(&(nd.nxt)), sizeof(nd.nxt));
        _file.read(reinterpret_cast<char *>(&(nd.data)), sizeof(nd.data));
    }

    void writeHead(int pos, const Node<K, V> &nd) {
        _file.seekp(pos * sizeof(Node<K, V>) + sizeof(int) * 2 + sizeof(bufferPool));
        _file.write(reinterpret_cast<const char *>(&(nd.isLeaf)), sizeof(nd.isLeaf));
        _file.write(reinterpret_cast<const char *>(&(nd.siz)), sizeof(nd.siz));
        _file.write(reinterpret_cast<const char *>(&(nd.nxt)), sizeof(nd.nxt));
        _file.write(reinterpret_cast<const char *>(&(nd.data)), sizeof(nd.data));
    }

    mPair<K, V> pass;

    bool InternalInsert(Node<K, V> &cur, int pos, const mPair<K, V> &data) {
        if (cur.isLeaf) {
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (cur.data[mid] > data) {
                    r = mid;
                } else {
                    l = mid + 1;
                }
            }
            if (cur.siz < MaxSize) {
                for (int i = cur.siz - 1; i >= l; --i) {
                    cur.data[i + 1] = cur.data[i];
                }
                ++cur.siz;
                cur.data[l] = data;
                writeHead(pos, cur);
                return false;
            }
            for (int i = cur.siz - 1; i >= l; --i) {
                cur.data[i + 1] = cur.data[i];
            }
            ++cur.siz;
            cur.data[l] = data;
            int newpos = getPos();
            static Node<K, V> nd;
            nd.isLeaf = true;
            nd.siz = MinSize + 1;
            nd.nxt = cur.nxt;
            cur.nxt = newpos;
            for (int i = 0; i <= MinSize; ++i) {
                nd.data[i] = cur.data[i + MinSize];
            }
            cur.siz = MinSize;
            if (root == pos) {
                static Node<K, V> newroot;
                newroot.isLeaf = false;
                newroot.siz = 1;
                newroot.data[0] = cur.data[MinSize];
                newroot.chd[0] = pos;
                newroot.chd[1] = newpos;
                writeHead(pos, cur);
                writeHead(newpos, nd);
                int rootpos = getPos();
                writeNode(rootpos, newroot);
                root = rootpos;
                return false;
            }
            writeNode(pos, cur);
            writeNode(newpos, nd);
            pass = nd.data[0];
            return true;
        }
        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cur.data[mid] >= data) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        if (l < cur.siz && cur.data[l] == data) {
            ++l;
        }
        Node<K, V> child;
        getNode(cur.chd[l], child);
        bool state = InternalInsert(child, cur.chd[l], data);
        if (!state) return false;

        if (cur.siz < MaxSize) {
            for (int i = cur.siz - 1; i >= l; --i) {
                cur.data[i + 1] = cur.data[i];
                cur.chd[i + 2] = cur.chd[i + 1];
            }
            ++cur.siz;
            cur.data[l] = pass;
            cur.chd[l + 1] = LastPos();
            writeNode(pos, cur);
            return false;
        }
        for (int i = cur.siz - 1; i >= l; --i) {
            cur.data[i + 1] = cur.data[i];
            cur.chd[i + 2] = cur.chd[i + 1];
        }
        ++cur.siz;
        cur.data[l] = pass;
        cur.chd[l + 1] = LastPos();
        int newpos = getPos();
        pass = cur.data[MinSize];
        static Node<K, V> nd;
        nd.isLeaf = false;
        nd.siz = MinSize;
        for (int i = 0; i < MinSize; ++i) {
            nd.data[i] = cur.data[i + MinSize + 1];
            nd.chd[i] = cur.chd[i + MinSize + 1];
        }
        nd.chd[MinSize] = cur.chd[cur.siz];
        cur.siz = MinSize;
        if (root == pos) {
            static Node<K, V> newroot;
            newroot.isLeaf = false;
            newroot.siz = 1;
            newroot.data[0] = pass;
            newroot.chd[0] = pos;
            newroot.chd[1] = newpos;
            writeNode(pos, cur);
            writeNode(newpos, nd);
            int rootpos = getPos();
            writeNode(rootpos, newroot);
            root = rootpos;
            return false;
        }
        writeNode(pos, cur);
        writeNode(newpos, nd);
        return true;
    }

    bool InternalDelete(Node<K, V> &cur, int pos, const mPair<K, V> &data) {
        if (cur.isLeaf) {
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (cur.data[mid] > data) {
                    r = mid;
                } else {
                    l = mid + 1;
                }
            }
            --l;
            if (l < 0 || l >= cur.siz || cur.data[l] != data) {
                return false;
            }
            for (int i = l + 1; i < cur.siz; ++i) {
                cur.data[i - 1] = cur.data[i];
            }
            --cur.siz;
            if (pos == root) {
                writeNode(pos, cur);
            }
            writeHead(pos, cur);
            if (cur.siz < MinSize) {
                return true;
            }
            return false;
        }
        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (cur.data[mid] >= data) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        if (l < cur.siz && data == cur.data[l]) {
            ++l;
        }
        Node<K, V> child;
        getNode(cur.chd[l], child);
        bool state = InternalDelete(child, cur.chd[l], data);
        if (!state) return false;
        if (pos == root && cur.siz == 1) {
            static Node<K, V> nd[2];
            readHead(cur.chd[0], nd[0]);
            readHead(cur.chd[1], nd[1]);
            if (nd[0].siz + nd[1].siz < MaxSize) {
                getNode(cur.chd[0], nd[0]);
                getNode(cur.chd[1], nd[1]);
                Pool(cur.chd[1]);
                Pool(root);
                if (nd[0].isLeaf) {
                    for (int i = 0; i < nd[1].siz; ++i) {
                        nd[0].data[i + nd[0].siz] = nd[1].data[i];
                    }
                    nd[0].siz += nd[1].siz;
                    nd[0].nxt = nd[1].nxt;
                    root = cur.chd[0];
                    writeHead(cur.chd[0], nd[0]);
                    return false;
                }
                for (int i = 0; i < nd[1].siz; ++i) {
                    nd[0].data[i + nd[0].siz + 1] = nd[1].data[i];
                    nd[0].chd[i + nd[0].siz + 1] = nd[1].chd[i];
                }
                nd[0].chd[nd[0].siz + nd[1].siz + 1] = nd[1].chd[nd[1].siz];
                nd[0].data[nd[0].siz] = cur.data[0];
                nd[0].siz += nd[1].siz + 1;
                root = cur.chd[0];
                writeNode(cur.chd[0], nd[0]);
                return false;
            }
        }
        if (l > 0) {
            static Node<K, V> nd;
            readHead(cur.chd[l - 1], nd);
            if (nd.siz > MinSize) {
                if (child.isLeaf) {
                    readHead(cur.chd[l - 1], nd);
                    for (int i = child.siz - 1; i >= 0; --i) {
                        child.data[i + 1] = child.data[i];
                    }
                    child.data[0] = nd.data[nd.siz - 1];
                    ++child.siz;
                    --nd.siz;
                    cur.data[l - 1] = child.data[0];
                    writeNode(pos, cur);
                    writeHead(cur.chd[l - 1], nd);
                    writeHead(cur.chd[l], child);
                    return false;
                }
                getNode(cur.chd[l - 1], nd);
                for (int i = child.siz; i >= 1; --i) {
                    child.data[i] = child.data[i - 1];
                    child.chd[i + 1] = child.chd[i];
                }
                child.chd[1] = child.chd[0];
                ++child.siz;
                child.data[0] = cur.data[l - 1];
                child.chd[0] = nd.chd[nd.siz];
                cur.data[l - 1] = nd.data[nd.siz - 1];
                --nd.siz;
                writeNode(pos, cur);
                writeHead(cur.chd[l - 1], nd);
                writeNode(cur.chd[l], child);
                return false;
            }
            if (child.isLeaf) {
                readHead(cur.chd[l - 1], nd);
                Pool(cur.chd[l]);
                for (int i = 0; i < child.siz; ++i) {
                    nd.data[i + nd.siz] = child.data[i];
                }
                nd.siz += child.siz;
                nd.nxt = child.nxt;
                for (int i = l; i < cur.siz; ++i) {
                    cur.data[i - 1] = cur.data[i];
                    cur.chd[i] = cur.chd[i + 1];
                }
                --cur.siz;
                nd.nxt = child.nxt;
                writeNode(pos, cur);
                writeHead(cur.chd[l - 1], nd);
                if (cur.siz < MinSize) return true;
                return false;
            }
            getNode(cur.chd[l - 1], nd);
            Pool(cur.chd[l]);
            for (int i = 0; i < child.siz; ++i) {
                nd.data[i + nd.siz + 1] = child.data[i];
                nd.chd[i + nd.siz + 1] = child.chd[i];
            }
            nd.chd[nd.siz + child.siz + 1] = child.chd[child.siz];
            nd.data[nd.siz] = cur.data[l - 1];
            nd.siz += child.siz + 1;
            for (int i = l - 1; i < cur.siz - 1; ++i) {
                cur.data[i] = cur.data[i + 1];
                cur.chd[i + 1] = cur.chd[i + 2];
            }
            --cur.siz;
            writeNode(pos, cur);
            writeNode(cur.chd[l - 1], nd);
            if (cur.siz < MinSize) return true;
            return false;
        } else if (l < cur.siz) {
            static Node<K, V> nd;
            readHead(cur.chd[l + 1], nd);
            if (nd.siz > MinSize) {
                if (child.isLeaf) {
                    readHead(cur.chd[l + 1], nd);
                    child.data[child.siz] = nd.data[0];
                    ++child.siz;
                    for (int i = 0; i < nd.siz - 1; ++i) {
                        nd.data[i] = nd.data[i + 1];
                    }
                    --nd.siz;
                    cur.data[l] = nd.data[0];
                    writeNode(pos, cur);
                    writeHead(cur.chd[l], child);
                    writeHead(cur.chd[l + 1], nd);
                    return false;
                }
                getNode(cur.chd[l + 1], nd);
                child.data[child.siz] = cur.data[l];
                child.chd[child.siz + 1] = nd.chd[0];
                ++child.siz;
                cur.data[l] = nd.data[0];
                for (int i = 0; i < nd.siz - 1; ++i) {
                    nd.data[i] = nd.data[i + 1];
                    nd.chd[i] = nd.chd[i + 1];
                }
                nd.chd[nd.siz - 1] = nd.chd[nd.siz];
                --nd.siz;
                writeNode(pos, cur);
                writeNode(cur.chd[l], child);
                writeNode(cur.chd[l + 1], nd);
                return false;
            }
            if (child.isLeaf) {
                readHead(cur.chd[l + 1], nd);
                Pool(cur.chd[l + 1]);
                for (int i = 0; i < nd.siz; ++i) {
                    child.data[i + child.siz] = nd.data[i];
                }
                child.siz += nd.siz;
                child.nxt = nd.nxt;
                for (int i = l; i < cur.siz - 1; ++i) {
                    cur.data[i] = cur.data[i + 1];
                    cur.chd[i + 1] = cur.chd[i + 2];
                }
                --cur.siz;
                child.nxt = nd.nxt;
                writeNode(pos, cur);
                writeHead(cur.chd[l], child);
                if (cur.siz < MinSize) return true;
                return false;
            }
            getNode(cur.chd[l + 1], nd);
            Pool(cur.chd[l + 1]);
            for (int i = 0; i < nd.siz; ++i) {
                child.data[i + child.siz + 1] = nd.data[i];
                child.chd[i + child.siz + 1] = nd.chd[i];
            }
            child.chd[child.siz + nd.siz + 1] = nd.chd[nd.siz];
            child.data[child.siz] = cur.data[l];
            child.siz += nd.siz + 1;
            for (int i = l; i < cur.siz - 1; ++i) {
                cur.data[i] = cur.data[i + 1];
                cur.chd[i + 1] = cur.chd[i + 2];
            }
            --cur.siz;
            writeNode(pos, cur);
            writeNode(cur.chd[l], child);
            if (cur.siz < MinSize) return true;
            return false;
        } else {
            exit(1);
        }
    }

public:
    int root = -1;

    BPlusTree() = default;

    explicit BPlusTree(const std::string &name) {
        _filename = name;
        _file.open(_filename);
        if (!_file) {
            _file.open(_filename, std::fstream::out);
            _file.close();
            _file.open(_filename);
            size = -1;
            _file.seekp(0);
            _file.write(reinterpret_cast<const char *>(&size), sizeof(size));
            root = -1;
            _file.write(reinterpret_cast<const char *>(&root), sizeof(root));
            bufferPool[0] = 0;
            _file.write(reinterpret_cast<const char *>(&bufferPool), sizeof(bufferPool));
            static Node<K, V> tmp;
            _file.write(reinterpret_cast<const char *>(&tmp), sizeof(tmp));
        } else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&size), sizeof(size));
            _file.read(reinterpret_cast<char *>(&root), sizeof(root));
            _file.read(reinterpret_cast<char *>(&bufferPool), sizeof(bufferPool));
        }
    }

    ~BPlusTree() {
        _file.seekp(0);
        _file.write(reinterpret_cast<const char *>(&size), sizeof(size));
        _file.write(reinterpret_cast<const char *>(&root), sizeof(root));
        _file.write(reinterpret_cast<const char *>(&bufferPool), sizeof(bufferPool));
        _file.close();
    }

    vector<V> find(const K &key) {
        vector<V> ans;
        if (root == -1) {
            return ans;
        }
        static Node<K, V> cur;
        cur.isLeaf = false;
        int pos = root;
        while (true) {
            getNode(pos, cur);
            if (cur.isLeaf) {
                break;
            }
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (!(cur.data[mid].key < key)) {
                    r = mid;
                } else {
                    l = mid + 1;
                }
            }
            pos = cur.chd[l];
        }

        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (!(cur.data[mid].key < key)) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        if (l > 0) --l;
        if (l < cur.siz && key < cur.data[l].key) {
            return ans;
        }
        while (true) {
            for (int i = l; i < cur.siz; ++i) {
                if (key < cur.data[i].key) {
                    break;
                }
                if (key == cur.data[i].key) {
                    ans.push_back(cur.data[i].val);
                }
            }
            pos = cur.nxt;
            if (pos == -1) break;
            readHead(pos, cur);
            l = 0;
        }
        return ans;
    }

    int count(const K key) {
        int ans = 0;
        if (root == -1) {
            return ans;
        }
        static Node<K, V> cur;
        cur.isLeaf = false;
        int pos = root;
        while (true) {
            getNode(pos, cur);
            if (cur.isLeaf) {
                break;
            }
            int l = 0, r = cur.siz;
            while (l < r) {
                int mid = (l + r) >> 1;
                if (!(cur.data[mid].key < key)) {
                    r = mid;
                } else {
                    l = mid + 1;
                }
            }
            pos = cur.chd[l];
        }

        int l = 0, r = cur.siz;
        while (l < r) {
            int mid = (l + r) >> 1;
            if (!(cur.data[mid].key < key)) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        if (l > 0) --l;
        if (l < cur.siz && key < cur.data[l].key) {
            return ans;
        }
        while (true) {
            for (int i = l; i < cur.siz; ++i) {
                if (key < cur.data[i].key) {
                    break;
                }
                if (key == cur.data[i].key) {
                    ++ans;
                }
            }
            pos = cur.nxt;
            if (pos == -1) break;
            readHead(pos, cur);
            l = 0;
        }
        return ans;
    }

    bool empty() {
        return root == -1;
    }

    void insert(const mPair<K, V> &data) {
        if (root == -1) {
            root = size = 0;
            static Node<K, V> cur;
            cur.siz = 1;
            cur.data[0] = data;
            cur.isLeaf = true;
            cur.nxt = -1;
            writeHead(size, cur);
            return;
        }
        Node<K, V> cur;
        getNode(root, cur);
        InternalInsert(cur, root, data);
    }

    void insert(const K &key, const V &value) {
        insert(mPair<K, V>(key, value));
    }

    void erase(const mPair<K, V> &data) {
        if (root == -1) return;
        static Node<K, V> cur;
        getNode(root, cur);
        InternalDelete(cur, root, data);
    }

    void erase(const K &key, const V &value) {
        erase(mPair<K, V>(key, value));
    }
};

#endif //TICKETSYSTEM_BPTREE_H
