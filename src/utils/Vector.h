#ifndef TICKETSYSTEM_VECTOR_H
#define TICKETSYSTEM_VECTOR_H

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>

template<typename T>
class vector {
public:
    class const_iterator;

    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::output_iterator_tag;

    private:
        vector<T> *v_ptr;
        T *m_ptr;
    public:
        vector<T> *get_v_ptr() const { return v_ptr; }

        T *get_m_ptr() const { return m_ptr; }

        iterator() = default;

        iterator(vector<T> *_v_ptr, T *_m_ptr) : v_ptr(_v_ptr), m_ptr(_m_ptr) {}

        iterator operator+(const int &n) const {
            iterator ret(v_ptr, m_ptr + n);
            return ret;
        }

        iterator operator-(const int &n) const {
            iterator ret(v_ptr, m_ptr - n);
            return ret;
        }

        int operator-(const iterator &rhs) const {
            if (v_ptr != rhs.v_ptr) throw std::exception();
            return m_ptr - rhs.m_ptr;
        }

        iterator &operator+=(const int &n) {
            m_ptr += n;
            return *this;
        }

        iterator &operator-=(const int &n) {
            m_ptr -= n;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(v_ptr, m_ptr);
            ++m_ptr;
            return tmp;
        }

        iterator &operator++() {
            ++m_ptr;
            return *this;
        }

        iterator operator--(int) {
            iterator tmp(v_ptr, m_ptr);
            --m_ptr;
            return tmp;
        }

        iterator &operator--() {
            --m_ptr;
            return *this;
        }

        T &operator*() const { return *m_ptr; }

        bool operator==(const iterator &rhs) const { return m_ptr == rhs.get_m_ptr(); }

        bool operator==(const const_iterator &rhs) const { return m_ptr == rhs.get_m_ptr(); }

        bool operator!=(const iterator &rhs) const { return m_ptr != rhs.get_m_ptr(); }

        bool operator!=(const const_iterator &rhs) const { return m_ptr != rhs.get_m_ptr(); }
    };

    class const_iterator {
    private:
        const vector<T> *v_ptr;
        const T *m_ptr;
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::output_iterator_tag;

        const vector<T> *get_v_ptr() const { return v_ptr; }

        const T *get_m_ptr() const { return m_ptr; }

        const_iterator(const vector<T> *_v_ptr, const T *_m_ptr) : v_ptr(_v_ptr), m_ptr(_m_ptr) {}

        const_iterator() = default;

        const_iterator operator+(const int &n) const {
            const_iterator ret(v_ptr, m_ptr + n);
            return ret;
        }

        const_iterator operator-(const int &n) const {
            const_iterator ret(v_ptr, m_ptr - n);
            return ret;
        }

        int operator-(const iterator &rhs) const {
            if (v_ptr != rhs.v_ptr) throw std::exception();
            return m_ptr - rhs.m_ptr;
        }

        const_iterator &operator+=(const int &n) {
            m_ptr += n;
            return *this;
        }

        const_iterator &operator-=(const int &n) {
            m_ptr -= n;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp(v_ptr, m_ptr);
            ++m_ptr;
            return tmp;
        }

        const_iterator &operator++() {
            ++m_ptr;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp(v_ptr, m_ptr);
            --m_ptr;
            return tmp;
        }

        const_iterator &operator--() {
            --m_ptr;
            return *this;
        }

        T operator*() const { return *m_ptr; }

        bool operator==(const iterator &rhs) const { return m_ptr == rhs.get_m_ptr(); }

        bool operator==(const const_iterator &rhs) const { return m_ptr == rhs.get_m_ptr(); }

        bool operator!=(const iterator &rhs) const { return m_ptr != rhs.get_m_ptr(); }

        bool operator!=(const const_iterator &rhs) const { return m_ptr != rhs.get_m_ptr(); }
    };

    vector() : max_length(2), current_length(0) {
        data = (T *) malloc(max_length * sizeof(T));
    }

    vector(const vector &other) : max_length(other.max_length), current_length(other.current_length) {
        data = (T *) malloc(max_length * sizeof(T));
        memcpy(data, other.data, max_length * sizeof(T));
    }

    ~vector() {
        for (int i = 0; i < current_length; ++i) {
            data[i].~T();
        }
        free(data);
    }

    vector &operator=(const vector &other) {
        if (&other == this) return *this;
        max_length = other.max_length;
        current_length = other.current_length;
        free(data);
        data = (T *) malloc(max_length * sizeof(T));
        memcpy(data, other.data, max_length * sizeof(T));
        return *this;
    }

    T &at(const size_t &pos) {
        if ((pos < 0) || (pos > current_length - 1)) throw std::exception();
        return data[pos];
    }

    const T &at(const size_t &pos) const {
        if ((pos < 0) || (pos > current_length - 1)) throw std::exception();
        return data[pos];
    }

    T &operator[](const size_t &pos) {
        if ((pos < 0) || (pos > current_length - 1)) throw std::exception();
        return data[pos];
    }

    const T &operator[](const size_t &pos) const {
        if ((pos < 0) || (pos > current_length - 1)) throw std::exception();
        return data[pos];
    }

    const T &front() const {
        if (current_length == 0) throw std::exception();
        return data[0];
    }

    const T &back() const {
        if (current_length == 0) throw std::exception();
        return data[current_length - 1];
    }

    iterator begin() {
        iterator ret(this, data);
        return ret;
    }

    const_iterator cbegin() const {
        const_iterator ret(this, data);
        return ret;
    }

    iterator end() {
        iterator ret(this, data + current_length);
        return ret;
    }

    const_iterator cend() const {
        const_iterator ret(this, data + current_length);
        return ret;
    }

    bool empty() const {
        return current_length == 0;
    }

    size_t size() const {
        return current_length;
    }

    void clear() {
        for (int i = 0; i < current_length; ++i) {
            data[i].~T();
        }
        free(data);
        max_length = 2;
        current_length = 0;
        data = (T *) malloc(max_length * sizeof(T));
    }

    void doubleSpace() {
        T *tmp = data;
        data = (T *) malloc(max_length * 2 * sizeof(T));
        memcpy(data, tmp, max_length * sizeof(T));
        max_length *= 2;
        free(tmp);
    }

    void divideSpace() {
        T *tmp = data;
        data = (T *) malloc(max_length / 2 * sizeof(T));
        memcpy(data, tmp, (max_length >> 1) * sizeof(T));
        max_length >>= 1;
        free(tmp);
    }

    iterator insert(iterator pos, const T &value) {
        int n_pos = pos - this->begin();
        if (current_length == max_length)
            doubleSpace();
        for (int i = current_length; i > n_pos; --i) {
            data[i] = data[i - 1];
        }
        data[n_pos] = value;
        ++current_length;
        iterator ret(this, data + n_pos);
        return ret;
    }

    iterator insert(const size_t &ind, const T &value) {
        if ((ind < 0) || (ind > current_length)) throw std::exception();
        if (current_length == max_length)
            doubleSpace();
        for (int i = current_length; i > ind; --i) {
            data[i] = data[i - 1];
        }
        data[ind] = value;
        ++current_length;
        iterator ret(this, data + ind);
        return ret;
    }

    iterator erase(iterator pos) {
        int n_pos = pos - this->begin();
        if (current_length == (max_length >> 1) - 1)
            divideSpace();
        data[n_pos].~T();
        for (int i = n_pos + 1; i < current_length; ++i) {
            data[i - 1] = data[i];
        }
        --current_length;
        iterator ret(this, data + n_pos);
        return ret;
    }

    iterator erase(const size_t &ind) {
        if ((ind < 0) || (ind > current_length - 1)) throw std::exception();
        if (current_length == (max_length >> 1) - 1)
            divideSpace();
        data[ind].~T();
        for (int i = ind + 1; i < current_length; ++i) {
            data[i - 1] = data[i];
        }
        --current_length;
        iterator ret(this, data + ind);
        return ret;
    }

    void push_back(const T &value) {
        if (current_length == max_length)
            doubleSpace();
        memset(data + current_length, 0, sizeof(T));
        data[current_length] = value;
        ++current_length;
    }

    void pop_back() {
        if (current_length == (max_length >> 1) - 1)
            divideSpace();
        data[current_length - 1].~T();
        --current_length;
    }

private:
    size_t max_length;
    size_t current_length;
    T *data;
};

#endif //TICKETSYSTEM_VECTOR_H
