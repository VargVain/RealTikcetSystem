#ifndef TICKETSYSTEM_CALENDAR_H
#define TICKETSYSTEM_CALENDAR_H

#include "Others.h"

short ToShort(std::string arg) {
    std::string token;
    token += arg[0];
    token += arg[1];
    token += arg[3];
    token += arg[4];
    return std::stoi(token);
}

int nextDay(int &day) {
    if (day == 630) day = 701;
    else if (day == 731) day = 801;
    else if (day == 831) day = 901;
    else ++day;
    return day;
}

int beforeDay(int &day) {
    if (day == 901) day = 831;
    else if (day == 801) day = 731;
    else if (day == 701) day = 630;
    else if (day == 601) day = 531;
    else --day;
    return day;
}

int nextDays(int &day, int len) {
    for (int i = 0; i < len; ++i) {
        nextDay(day);
    }
    return day;
}

int beforeDays(int &day, int len) {
    for (int i = 0; i < len; ++i) {
        beforeDay(day);
    }
    return day;
}

int nextDays_n(int day, int len) {
    for (int i = 0; i < len; ++i) {
        nextDay(day);
    }
    return day;
}

int beforeDays_n(int day, int len) {
    for (int i = 0; i < len; ++i) {
        beforeDay(day);
    }
    return day;
}

int passDays(int endDate, int startDate) {
    if (endDate < startDate) return -1;
    int passDays = 1;
    while (endDate != startDate) {
        nextDay(startDate);
        ++passDays;
    }
    return passDays;
}

std::string printDay(int day) {
    std::string a = std::to_string(day / 100), b = std::to_string(day % 100);
    if (a.length() == 1) a = "0" + a;
    if (b.length() == 1) b = "0" + b;
    return a + "-" + b;
}

std::string printTime(int time) {
    std::string a = std::to_string(time / 100), b = std::to_string(time % 100);
    if (a.length() == 1) a = "0" + a;
    if (b.length() == 1) b = "0" + b;
    return a + ":" + b;
}

int addTime(int &time, int len) {
    int m = time % 100 + len;
    int h = time / 100;
    int mm = m % 60;
    int hh = (h + m / 60) % 24;
    time = hh * 100 + mm;
    return (h + m / 60) / 24;
}

int passTime(int dayEnd, int timeEnd, int dayStart, int timeStart) {
    int d = dayEnd - dayStart;
    int m = (timeEnd / 100 - timeStart / 100) * 60 + (timeEnd % 100 - timeStart % 100);
    return d * 1440 + m;
}

int getNo(int trainID, int trainNo) {
    return trainID * 100 + trainNo;
}

const int days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};

class Date {
    friend class DateTime;
private:
    int date;
public:
    explicit Date() : date(0) {}
    explicit Date(const std::string &str) {
        // mm-dd
        int month = (str[0] - '0') * 10 + str[1] - '0';
        int day = (str[3] - '0') * 10 + str[4] - '0';
        date = days[month - 1] + day;
    }
    Date(int month, int day) : date(days[month - 1] + day) {}
    std::pair<int, int> GetDate() const {
        int month = sjtu::lower_bound(days, 0, 12, date);
        int day = date - days[month - 1];
        return std::make_pair(month, day);
    }
    std::string ToString() const {
        std::pair<int, int> p = GetDate();
        std::string str;
        str.push_back((p.first / 10) + '0');
        str.push_back((p.first % 10) + '0');
        str.push_back('-');
        str.push_back((p.second / 10) + '0');
        str.push_back((p.second % 10) + '0');
        return str;
    }
    inline bool operator==(const Date &y) const {
        return date == y.date;
    }
    inline bool operator<(const Date &y) const {
        return date < y.date;
    }
    inline bool operator<=(const Date &y) const {
        return date <= y.date;
    }
    Date &operator++() {
        ++date;
        return *this;
    }
    Date &operator--() {
        --date;
        return *this;
    }
    Date operator+(int x) const {
        Date tmp = *this;
        tmp.date += x;
        return tmp;
    }
    Date operator-(int x) const {
        Date tmp = *this;
        tmp.date -= x;
        return tmp;
    }
    Date &operator+=(int x) {
        date += x;
        return *this;
    }
    Date &operator-=(int x) {
        date -= x;
        return *this;
    }
    int operator-(const Date &y) const {
        return date - y.date;
    }
};
class Time {
    friend class DateTime;
private:
    int delta_day = 0, _time;
public:
    Time() : delta_day(0), _time(0) {}
    Time(const std::string &str) : delta_day(0) {
        // mm-dd
        int hour = (str[0] - '0') * 10 + str[1] - '0';
        int minute = (str[3] - '0') * 10 + str[4] - '0';
        _time = hour * 60 + minute;
    }
    Time(int hour, int minute) : _time(hour * 60 + minute), delta_day(0) {}
    std::pair<int, int> GetTime() const {
        return std::make_pair(_time / 60, _time % 60);
    }
    int GetDay() const { return delta_day; }
    std::string ToString() const {
        std::pair<int, int> p = GetTime();
        std::string str;
        str.push_back((p.first / 10) + '0');
        str.push_back((p.first % 10) + '0');
        str.push_back(':');
        str.push_back((p.second / 10) + '0');
        str.push_back((p.second % 10) + '0');
        return str;
    }
    Time EraseDay() {
        Time tmp(*this);
        tmp.delta_day = 0;
        return tmp;
    }
    inline bool operator==(const Time &y) const {
        return _time == y._time;
    }
    inline bool operator<(const Time &y) const {
        return _time < y._time;
    }
    inline bool operator<=(const Time &y) const {
        return _time <= y._time;
    }
    Time &operator++() {
        ++_time;
        if (_time >= 1440) {
            _time -= 1440;
            ++delta_day;
        }
        return *this;
    }
    Time &operator--() {
        --_time;
        if (_time < 0) {
            _time += 1440;
            --delta_day;
        }
        return *this;
    }
    Time operator+(const int x) {
        Time tmp(*this);
        tmp._time += x;
        tmp.delta_day += tmp._time / 1440;
        tmp._time %= 1440;
        return tmp;
    }
    Time operator-(const int x) {
        Time tmp(*this);
        tmp._time -= x;
        while (tmp._time < 0) {
            tmp._time += 1440;
            --tmp.delta_day;
        }
        return tmp;
    }
    Time &operator+=(const int x) {
        _time += x;
        delta_day += _time / 1440;
        _time %= 1440;
        return *this;
    }
    Time &operator-=(const int x) {
        _time -= x;
        while (_time < 0) {
            _time += 1440;
            --delta_day;
        }
        return *this;
    }
    int operator-(const Time &y) const {
        return (_time - y._time) + (delta_day - y.delta_day) * 1440;
    }
};
class DateTime {
private:
    int date, _time;
public:
    // string format: mm-dd hh:mm
    DateTime() : date(0), _time(0) {}
    explicit DateTime(const std::string &str) {
        // mm-dd
        int month = (str[0] - '0') * 10 + str[1] - '0';
        int day = (str[3] - '0') * 10 + str[4] - '0';
        date = days[month - 1] + day;
        // hh:mm
        int hour = (str[6] - '0') * 10 + str[7] - '0';
        int minute = (str[9] - '0') * 10 + str[10] - '0';
        _time = hour * 60 + minute;
    }
    DateTime(int month, int day, int hour, int minute) : date(days[month - 1] + day), _time(hour * 60 + minute) {}
    DateTime(const Date &d, const Time &t) : date(d.date + t.GetDay()), _time(t._time) {}
    inline bool operator==(const DateTime &y) const {
        return date == y.date && _time == y._time;
    }
    inline bool operator<(const DateTime &y) const {
        return date < y.date || (date == y.date && _time < y._time);
    }
    inline bool operator<=(const DateTime &y) const {
        return date < y.date || (date == y.date && _time <= y._time);
    }
    DateTime operator+(const int delta) {
        DateTime tmp(*this);
        tmp._time += delta;
        tmp.date += tmp._time / 1440;
        tmp._time %= 1440;
        return tmp;
    }
    DateTime operator-(const int delta) {
        DateTime tmp(*this);
        tmp._time -= delta;
        while (tmp._time < 0) {
            tmp._time += 1440;
            --tmp.date;
        }
        return tmp;
    }
    DateTime &operator+=(const int delta) {
        _time += delta;
        date += _time / 1440;
        _time %= 1440;
        return *this;
    }
    DateTime &operator-=(const int delta) {
        _time -= delta;
        while (_time < 0) {
            _time += 1440;
            --date;
        }
        return *this;
    }
    int operator-(const DateTime &y) const {
        return (date - y.date) * 1440 + (_time - y._time);
    }
    std::pair<int, int> GetDate() const {
        int month = sjtu::lower_bound(days, 0, 12, date);
        int day = date - days[month - 1];
        return std::make_pair(month, day);
    }
    std::pair<int, int> GetTime() const {
        return std::make_pair(_time / 60, _time % 60);
    }
    std::string ToString() const {
        std::pair<int, int> p = GetDate();
        std::string str;
        str.push_back((p.first / 10) + '0');
        str.push_back((p.first % 10) + '0');
        str.push_back('-');
        str.push_back((p.second / 10) + '0');
        str.push_back((p.second % 10) + '0');
        str.push_back(' ');
        p = GetTime();
        str.push_back((p.first / 10) + '0');
        str.push_back((p.first % 10) + '0');
        str.push_back(':');
        str.push_back((p.second / 10) + '0');
        str.push_back((p.second % 10) + '0');
        return str;
    }
};

#endif //TICKETSYSTEM_CALENDAR_H
