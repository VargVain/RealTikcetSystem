#ifndef TICKETSYSTEM_MAP_H
#define TICKETSYSTEM_MAP_H

#include <iostream>
#include <cstring>
#include <cstddef>

template<
        class Key,
        class T,
        class Compare = std::less<Key>
> class map {
private:
    struct TreeNode;
    class AVL_Tree;
    AVL_Tree tree;
public:
    typedef std::pair<const Key, T> value_type;
    class const_iterator;
    class iterator {
    private:
        TreeNode* _node = nullptr;
        map* _ptr = nullptr;
    public:
        iterator() = default;
        iterator(TreeNode* node, map* ptr) : _node(node), _ptr(ptr) {}
        iterator(const iterator &other) {
            _node = other._node;
            _ptr = other._ptr;
        }
        iterator operator++(int) {
            iterator tmp(_node, _ptr);
            ++(*this);
            return tmp;
        }
        iterator & operator++() {
            if (_node->_right) {
                _node = _node->_right;
                while (_node->_left) {
                    _node = _node->_left;
                }
            }
            else {
                TreeNode* parent = _node->_parent;
                while (_node == parent->_right) {
                    _node = parent;
                    parent = parent->_parent;
                }
                _node = parent;
            }
            return *this;
        }
        iterator operator--(int) {
            iterator tmp(_node, _ptr);
            --(*this);
            return tmp;
        }
        iterator & operator--() {
            if (_node->_left) {
                _node = _node->_left;
                while (_node->_right) {
                    _node = _node->_right;
                }
            }
            else {
                TreeNode* parent = _node->_parent;
                while (_node == parent->_left) {
                    _node = parent;
                    parent = parent->_parent;
                }
                _node = parent;
            }
            return *this;
        }
        value_type & operator*() const {
            return _node->_value;
        }
        bool operator==(const iterator &rhs) const {
            return _node == rhs._node;
        }
        bool operator==(const const_iterator &rhs) const {
            return _node == rhs._node;
        }
        bool operator!=(const iterator &rhs) const {
            return _node != rhs._node;
        }
        bool operator!=(const const_iterator &rhs) const {
            return _node != rhs._node;
        }
        value_type* operator->() const noexcept {
            return &(_node->_value);
        }
        friend class map;
        friend class const_iterator;
    };
    class const_iterator {
        // it should have similar member method as iterator.
        //  and it should be able to construct from an iterator.
    private:
        const TreeNode* _node = nullptr;
        const map* _ptr = nullptr;
    public:
        const_iterator() = default;
        const_iterator(const TreeNode* node, const map* ptr) : _node(node), _ptr(ptr) {}
        const_iterator(const const_iterator &other) {
            _node = other._node;
            _ptr = other._ptr;
        }
        const_iterator(const iterator &other) {
            _node = other._node;
            _ptr = other._ptr;
        }
        const_iterator operator++(int) {
            const_iterator tmp(_node, _ptr);
            ++(*this);
            return tmp;
        }
        const_iterator & operator++() {
            if (_node->_right) {
                _node = _node->_right;
                while (_node->_left) {
                    _node = _node->_left;
                }
            }
            else {
                TreeNode* parent = _node->_parent;
                while (_node == parent->_right) {
                    _node = parent;
                    parent = parent->_parent;
                }
                _node = parent;
            }
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp(_node, _ptr);
            --(*this);
            return tmp;
        }
        const_iterator & operator--() {
            if (_node->_left) {
                _node = _node->_left;
                while (_node->_right) {
                    _node = _node->_right;
                }
            }
            else {
                TreeNode* parent = _node->_parent;
                while (_node == parent->_left) {
                    _node = parent;
                    parent = parent->_parent;
                }
                _node = parent;
            }
            return *this;
        }
        value_type operator*() const {
            return _node->_value;
        }
        bool operator==(const iterator &rhs) const {
            return _node == rhs._node;
        }
        bool operator==(const const_iterator &rhs) const {
            return _node == rhs._node;
        }
        bool operator!=(const iterator &rhs) const {
            return _node != rhs._node;
        }
        bool operator!=(const const_iterator &rhs) const {
            return _node != rhs._node;
        }
        const value_type* operator->() const noexcept {
            return &(_node->_value);
        }
        friend class map;
        friend class iterator;
    };
    map() = default;
    map(const map &other) {
        if (other.tree._root == nullptr) return;
        tree.insert(other.tree._root->_value);
        tree._root->_bf = other.tree._root->_bf;
        copyKey(tree._root, other.tree._root, other.tree._end);
        tree.size = other.size();
        TreeNode* node = tree._root;
        while (node->_right && node->_right != tree._end) node = node->_right;
        node->_right = tree._end;
        tree._end->_parent = node;
    }
    void copyKey(TreeNode* cur, TreeNode* other, TreeNode* end) {
        if (other->_left) {
            TreeNode* tmp = new TreeNode(other->_left->_value);
            cur->_left = tmp;
            tmp->_parent = cur;
            cur->_bf = other->_bf;
            copyKey(cur->_left, other->_left, end);
        }
        if (other->_right && other->_right != end) {
            TreeNode* tmp = new TreeNode(other->_right->_value);
            cur->_right = tmp;
            tmp->_parent = cur;
            cur->_bf = other->_bf;
            copyKey(cur->_right, other->_right, end);
        }
    }
    map & operator=(const map &other) {
        if (&tree == &other.tree) return *this;
        tree.clear();
        if (other.tree._root == nullptr) return *this;
        tree.insert(other.tree._root->_value);
        tree._root->_bf = other.tree._root->_bf;
        copyKey(tree._root, other.tree._root, other.tree._end);
        tree.size = other.size();
        TreeNode* node = tree._root;
        while (node->_right && node->_right != tree._end) node = node->_right;
        node->_right = tree._end;
        tree._end->_parent = node;
        return *this;
    }
    ~map() {
        tree.clear();
    }
    T & at(const Key &key) {
        TreeNode *node = tree.find(key);
        return node->_value.second;
    }
    const T & at(const Key &key) const {
        TreeNode *node = tree.find(key);
        return node->_value.second;
    }
    T & operator[](const Key &key) {
        TreeNode *node = tree.find(key);
        if (node == tree._end) {
            value_type value(key, T());
            return tree.insert(value)->_value.second;
        }
        return node->_value.second;
    }
    const T & operator[](const Key &key) const {
        TreeNode *node = tree.find(key);
        return node->_value.second;
    }
    iterator begin() {
        TreeNode* node = tree._root;
        if (node) {
            while (node->_left) node = node->_left;
            return iterator(node, this);
        }
        else return end();
    }
    const_iterator cbegin() const {
        TreeNode* node = tree._root;
        if (node) {
            while (node->_left) node = node->_left;
            return const_iterator(node, this);
        }
        else return cend();
    }
    iterator end() {
        return iterator(tree._end, this);
    }
    const_iterator cend() const {
        return const_iterator(tree._end, this);
    }
    bool empty() const {
        return tree._root == nullptr;
    }
    size_t size() const {
        return tree.size;
    }
    void clear() {
        tree.clear();
    }
    std::pair<iterator, bool> insert(const value_type &value) {
        iterator iter = find(value.first);
        if (iter != end()) return std::pair<iterator, bool>(iter, false);
        else {
            iter = iterator(tree.insert(value), this);
            return std::pair<iterator, bool>(iter, true);
        }
    }
    void erase(iterator pos) {
        tree.erase((*pos).first);
    }
    size_t count(const Key &key) const {
        TreeNode* node = tree.find(key);
        if (node != tree._end) return 1;
        else return 0;
    }
    iterator find(const Key &key) {
        return iterator(tree.find(key), this);
    }
    const_iterator find(const Key &key) const {
        return const_iterator(tree.find(key), this);
    }
private:
    struct TreeNode {
        typedef std::pair<const Key, T> value_type;
        TreeNode* _left;
        TreeNode* _right;
        TreeNode* _parent;
        value_type _value;
        int _bf;
        TreeNode (const value_type value)
                : _left(nullptr)
                , _right(nullptr)
                , _parent(nullptr)
                , _value(value)
                , _bf(0) {}
    };
    class AVL_Tree {
        typedef std::pair<const Key, T> value_type;
        typedef TreeNode node;
    public:
        AVL_Tree () : _root(nullptr) {
            //_end = new node(value_type(Key(), T()));
            _end = (node*)malloc(sizeof(node));
            _end->_left = nullptr;
            _end->_right = nullptr;
            _end->_parent = nullptr;
            _end->_bf = 0;
        }
        ~AVL_Tree() {
            clear();
            free(_end);
        }
        node* _root;
        node* _end;
        int size = 0;
        node* insert(const value_type value) {
            if (_end->_parent) _end->_parent->_right = nullptr;
            if (_root == nullptr) {
                _root = new node(value);
                //_root->_right = _end;
                //_end->_parent = _root;
                ++size;
                return _root;
            }
            node* cur = _root;
            node* parent = nullptr;
            while (cur && cur != _end) {
                if (Compare{}(value.first, cur->_value.first)) {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (Compare{}(cur->_value.first, value.first)) {
                    parent = cur;
                    cur = cur->_right;
                }
                else return nullptr;
            }
            cur = new node(value);
            node* ret = cur;
            if (Compare{}(value.first, parent->_value.first)) parent->_left = cur;
            else parent->_right = cur;
            cur->_parent = parent;

            while (cur != _root) {
                if (cur == parent->_left) parent->_bf--;
                else parent->_bf++;
                if (parent->_bf == 0) break;
                else if (parent->_bf == 1 || parent->_bf == -1) {
                    cur = parent;
                    parent = parent->_parent;
                }
                else if (parent->_bf == 2) {
                    if (cur->_bf == 1) rotateL(parent);
                    else rotateRL(parent);
                    break;
                }
                else if (parent->_bf == -2) {
                    if (cur->_bf == -1) rotateR(parent);
                    else rotateLR(parent);
                    break;
                }
            }
            ++size;
            TreeNode* node = _root;
            if (node) {
                while (node->_right && node->_right != _end) node = node->_right;
                node->_right = _end;
                _end->_parent = node;
            }
            else _end->_parent = nullptr;
            return ret;
        }
        bool erase(const Key& key) {
            if (_end->_parent) _end->_parent->_right = nullptr;
            node* cur = _root;
            node* parent = nullptr;
            node* del = nullptr;
            node* delParent = nullptr;
            while (cur && cur != _end) {
                if (Compare{}(key, cur->_value.first)) {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (Compare{}(cur->_value.first, key)) {
                    parent = cur;
                    cur = cur->_right;
                }
                else {
                    if (cur->_left == nullptr) {
                        if (cur == _root) {
                            _root = _root->_right;
                            if (_root) _root->_parent = nullptr;
                            delete cur;
                            --size;
                            TreeNode* node = _root;
                            if (node) {
                                while (node->_right && node->_right != _end) node = node->_right;
                                node->_right = _end;
                                _end->_parent = node;
                            }
                            else _end->_parent = nullptr;
                            if (!size) _end->_parent = nullptr;
                            return true;
                        }
                        else {
                            del = cur;
                        }
                    }
                    else if (cur->_right == nullptr) {
                        if (cur == _root) {
                            _root = _root->_left;
                            if (_root) _root->_parent = nullptr;
                            delete cur;
                            --size;
                            TreeNode* node = _root;
                            if (node) {
                                while (node->_right && node->_right != _end) node = node->_right;
                                node->_right = _end;
                                _end->_parent = node;
                            }
                            else _end->_parent = nullptr;
                            if (!size) _end->_parent = nullptr;
                            return true;
                        }
                        else {
                            del = cur;
                        }
                    }
                    else {
                        del = cur->_right;
                        while (del->_left) {
                            del = del->_left;
                        }
                        node* curP = cur->_parent, * curL = cur->_left, * curR = cur->_right;
                        node* delP = del->_parent, * delL = del->_left, * delR = del->_right;
                        cur->_parent = delP;
                        cur->_left = delL;
                        cur->_right = delR;
                        del->_parent = curP;
                        if (curP == nullptr) _root = del;
                        del->_left = curL;
                        del->_right = curR;
                        if (del == curR) {
                            cur->_parent = del;
                            del->_right = cur;
                        }
                        if (curP) {
                            if (cur == curP->_left) curP->_left = del;
                            else curP->_right = del;
                        }
                        if (curL) curL->_parent = del;
                        if (del != curR) {
                            curR->_parent = del;
                            if (del == delP->_left) delP->_left = cur;
                            else delP->_right = cur;
                        }
                        if (delL) delL->_parent = cur;
                        if (delR) delR->_parent = cur;
                        int tmp = cur->_bf;
                        cur->_bf = del->_bf;
                        del->_bf = tmp;
                        node* tmpn = del;
                        del = cur;
                        cur = tmpn;
                    }
                    break;
                }
            }
            if (del == nullptr) return false;

            cur = del;
            parent = cur->_parent;
            delParent = del->_parent;
            while (cur != _root) {
                if (cur == parent->_left) parent->_bf++;
                else parent->_bf--;
                if (parent->_bf == 0) { }
                else if (parent->_bf == 1 || parent->_bf == -1) break;
                else if (parent->_bf == 2) {
                    if (parent->_right->_bf == 1) {
                        node* tmp = parent->_right;
                        rotateL(parent);
                        parent = tmp;
                    }
                    else if (parent->_right->_bf == -1) {
                        node* tmp = parent->_right->_left;
                        rotateRL(parent);
                        parent = tmp;
                    }
                    else {
                        node* tmp = parent->_right;
                        rotateL(parent);
                        parent = tmp;
                        parent->_bf = -1;
                        parent->_left->_bf = 1;
                        break;
                    }
                }
                else if (parent->_bf == -2) {
                    if (parent->_left->_bf == -1) {
                        node* tmp = parent->_left;
                        rotateR(parent);
                        parent = tmp;
                    }
                    else if (parent->_left->_bf == 1) {
                        node* tmp = parent->_left->_right;
                        rotateLR(parent);
                        parent = tmp;
                    }
                    else {
                        node* tmp = parent->_left;
                        rotateR(parent);
                        parent = tmp;
                        parent->_bf = 1;
                        parent->_right->_bf = -1;
                        break;
                    }
                }
                cur = parent;
                parent = parent->_parent;
            }
            if (del->_left == nullptr) {
                if (del == delParent->_left) delParent->_left = del->_right;
                else delParent->_right = del->_right;
                if (del->_right) del->_right->_parent = delParent;
            }
            else {
                if (del == delParent->_left) delParent->_left = del->_left;
                else delParent->_right = del->_left;
                if (del->_left) del->_left->_parent = delParent;
            }
            delete del;
            TreeNode* node = _root;
            if (node) {
                while (node->_right && node->_right != _end) node = node->_right;
                node->_right = _end;
                _end->_parent = node;
            }
            else _end->_parent = nullptr;
            --size;
            if (!size) _end->_parent = nullptr;
            return true;
        }
        node* find(const Key& key) const {
            node* cur = _root;
            node* parent = nullptr;
            while (cur && cur != _end) {
                if (Compare{}(key, cur->_value.first)) {
                    parent = cur;
                    cur = cur->_left;
                }
                else if (Compare{}(cur->_value.first, key)) {
                    parent = cur;
                    cur = cur->_right;
                }
                else {
                    return cur;
                }
            }
            return _end;
        }
        void clear() {
            deleteNode(_root);
            _root = nullptr;
            _end->_parent = nullptr;
            size = 0;
        }
        void deleteNode(node* cur) {
            if (cur == nullptr || cur == _end) return;
            deleteNode(cur->_left);
            deleteNode(cur->_right);
            delete cur;
        }
    private:
        void rotateL(node* parent) {
            node* subR = parent->_right;
            node* subRL = subR->_left;
            node* grandParent = parent->_parent;
            parent->_parent = subR;
            subR->_left = parent;
            parent->_right = subRL;
            if (subRL) subRL->_parent = parent;
            if (grandParent) {
                if (parent == grandParent->_left) grandParent->_left = subR;
                else grandParent->_right = subR;
                subR->_parent = grandParent;
            }
            else {
                _root = subR;
                subR->_parent = nullptr;
            }
            subR->_bf = parent->_bf = 0;
        }
        void rotateR(node* parent) {
            node* subL = parent->_left;
            node* subLR = subL->_right;
            node* grandParent = parent->_parent;
            parent->_parent = subL;
            subL->_right = parent;
            parent->_left = subLR;
            if (subLR) subLR->_parent = parent;
            if (grandParent) {
                if (parent == grandParent->_left) grandParent->_left = subL;
                else grandParent->_right = subL;
                subL->_parent = grandParent;
            }
            else {
                _root = subL;
                subL->_parent = nullptr;
            }
            subL->_bf = parent->_bf = 0;
        }
        void rotateLR(node* parent) {
            node* subL = parent->_left;
            node* subLR = subL->_right;
            int bf = subLR->_bf;
            rotateL(subL);
            rotateR(parent);
            if (bf == 1) {
                subL->_bf = -1;
                parent->_bf = 0;
                subLR->_bf = 0;
            }
            else if (bf == -1) {
                subL->_bf = 0;
                parent->_bf = 1;
                subLR->_bf = 0;
            }
            else if (bf == 0) {
                subL->_bf = 0;
                parent->_bf = 0;
                subLR->_bf = 0;
            }
        }
        void rotateRL(node* parent) {
            node* subR = parent->_right;
            node* subRL = subR->_left;
            int bf = subRL->_bf;
            rotateR(subR);
            rotateL(parent);
            if (bf == 1) {
                subR->_bf = 0;
                parent->_bf = -1;
                subRL->_bf = 0;
            }
            else if (bf == -1) {
                subR->_bf = 1;
                parent->_bf = 0;
                subRL->_bf = 0;
            }
            else if (bf == 0) {
                subR->_bf = 0;
                parent->_bf = 0;
                subRL->_bf = 0;
            }
        }
    };
};

#endif //TICKETSYSTEM_MAP_H
