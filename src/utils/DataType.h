#ifndef TICKETSYSTEM_DATATYPE_H
#define TICKETSYSTEM_DATATYPE_H

#include <string>
#include <iostream>
#include <cstring>

struct mString {
    char _str[65];

    mString() = default;

    mString(const std::string &s) {
        strcpy(_str, s.c_str());
    }

    mString(char *t) {
        strcpy(_str, t);
    }

    friend bool operator<(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) < 0;
    }

    friend bool operator>(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) > 0;
    }

    friend bool operator==(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) == 0;
    }

    friend bool operator!=(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) != 0;
    }

    friend bool operator<=(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) <= 0;
    }

    friend bool operator>=(const mString &lhs, const mString &rhs) {
        return strcmp(lhs._str, rhs._str) >= 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const mString& s) {
        os << s._str;
        return os;
    }
};

#endif //TICKETSYSTEM_DATATYPE_H
