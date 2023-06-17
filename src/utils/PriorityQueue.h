#ifndef TICKETSYSTEM_PRIORITYQUEUE_H
#define TICKETSYSTEM_PRIORITYQUEUE_H

#include <cstddef>

template<typename T, class Compare>
class priority_queue {
public:
    priority_queue() {
        heap = skewHeap();
    }

    priority_queue(const priority_queue &other) {
        heap.copyHeap(other.heap);
    }
    ~priority_queue() {}

    priority_queue &operator=(const priority_queue &other) {
        heap.copyHeap(other.heap);
        return *this;
    }

    const T &top() const {
        return heap.root->key;
    }

    void push(const T &e) {
        heap.exception = true;
        heap.insert(e);
    }

    void pop() {
        heap.exception = true;
        heap.remove();
    }

    size_t size() const {
        return heap.size;
    }

    bool empty() const {
        return (heap.size == 0);
    }

    void merge(priority_queue &other) {
        heap.exception = true;
        heap.merge(other.heap);
        heap.size += other.size();
        other.heap.root = nullptr;
        other.heap.size = 0;
    }

private:
    class skewNode {
    public:
        T key;
        skewNode *left;
        skewNode *right;

        skewNode(T val, skewNode *l, skewNode *r) : key(val), left(l), right(r) {}
    };

    class skewHeap {
    public:
        skewNode *root;
        int size;
        bool exception = false;

        skewHeap() : root(nullptr), size(0) {}

        skewHeap(const skewHeap &other) {
            copy(root, other.root);
            size = other.size;
        }

        void copyHeap(const skewHeap &other) {
            if (root == other.root) return;
            destroy(root);
            copy(root, other.root);
            size = other.size;
        }

        ~skewHeap() { destroy(root); }

        void swapNode(skewNode *&x, skewNode *&y) {
            skewNode *tmp = x;
            x = y;
            y = tmp;
        }

        skewNode *merge(skewNode *x, skewNode *y) {
            if (x == nullptr) return y;
            if (y == nullptr) return x;
            try {
                if (!Compare{}(x->key, y->key)) { swapNode(x, y); }
                skewNode *tmp = merge(x->right, y);
                if (exception) {
                    x->right = x->left;
                    x->left = tmp;
                }
            } catch (...) { exception = false; }
            return x;
        }

        void merge(skewHeap &other) {
            root = merge(root, other.root);
        }

        void insert(T key) {
            skewNode *node;
            node = new skewNode(key, nullptr, nullptr);
            root = merge(root, node);
            if (exception) ++size;
            else delete node;
        }

        void remove() {
            if (root == nullptr) return;
            skewNode *l = root->left;
            skewNode *r = root->right;
            skewNode *tmp = root;
            delete root;
            root = merge(l, r);
            if (exception) {
                --size;
            } else root = tmp;
        }

        void copy(skewNode *&node, skewNode *other) {
            if (other == nullptr) return;
            node = new skewNode(other->key, nullptr, nullptr);
            if (other->left != nullptr) {
                copy(node->left, other->left);
            }
            if (other->right != nullptr) {
                copy(node->right, other->right);
            }
        }

        void destroy(skewNode *node) {
            if (node == nullptr) return;
            if (node->left != nullptr) destroy(node->left);
            if (node->right != nullptr) destroy(node->right);
            delete node;
            size = 0;
        }

    };

public:
    skewHeap heap;
};


#endif //TICKETSYSTEM_PRIORITYQUEUE_H
