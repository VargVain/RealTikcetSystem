#include <iostream>
#include <sstream>
#include "Order.h"
#include "Train.h"
#include "User.h"

UserList userList;
TrainList trainList;
OrderList orderList;

int main() {
/*
    system("rm -rf uBPT");
    system("rm -rf uList");
    system("rm -rf tBPT");
    system("rm -rf tList");
    system("rm -rf qBPT");
    system("rm -rf qList");
    system("rm -rf sBPT");
    system("rm -rf sList");
    system("rm -rf oBPT");
    system("rm -rf oList");
    system("rm -rf Pending");
    system("rm -rf trBPT");
    system("rm -rf trList");

    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
*/
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string line, opt, time;
    while (getline(std::cin, line)) {
        std::stringstream ss;
        ss.clear();
        ss << line;
        ss >> time;
        std::cout << time << " ";
        ss >> opt;
        if (opt[0] == 'a' && opt[4] == 'u') {
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege = -1;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            userList.AddUser(cur_username, username, password, name, mailAddr, privilege);
        }
        else if (opt[0] == 'l' && opt[3] == 'i') {
            std::string tmp, username, password;
            for (int unused = 0; unused < 2; ++unused) {
                ss >> tmp;
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                }
            }
            userList.Login(username, password);
        }
        else if (opt[0] == 'l' && opt[3] == 'o') {
            std::string tmp, username;
            ss >> tmp >> username;
            userList.Logout(username);
        }
        else if (opt[0] == 'q' && opt[6] == 'p') {
            std::string tmp, cur_username, username;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                }
            }
            userList.QueryProfile(cur_username, username);
        }
        else if (opt[0] == 'm') {
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege = -1;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            userList.ModifyProfile(cur_username, username, password, name, mailAddr, privilege);
        }
        else if (opt[0] == 'a' && opt[4] == 't') {
            std::string tmp, trainID, stations, prices, startTime, travelTimes, stopoverTimes, saleDate;
            int stationNum, seatNum;
            char _type;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'n': {
                        ss >> stationNum;
                        break;
                    }
                    case 'm': {
                        ss >> seatNum;
                        break;
                    }
                    case 's': {
                        ss >> stations;
                        break;
                    }
                    case 'p': {
                        ss >> prices;
                        break;
                    }
                    case 'x': {
                        ss >> startTime;
                        break;
                    }
                    case 't': {
                        ss >> travelTimes;
                        break;
                    }
                    case 'o': {
                        ss >> stopoverTimes;
                        break;
                    }
                    case 'd': {
                        ss >> saleDate;
                        break;
                    }
                    case 'y': {
                        ss >> _type;
                        break;
                    }
                }
            }
            trainList.AddTrain(trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate, _type);
        }
        else if (opt[0] == 'd') {
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            trainList.DeleteTrain(trainID);
        }
        else if (opt[0] == 'r' && opt[2] == 'l') {
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            trainList.ReleaseTrain(trainID);
        }
        else if (opt[0] == 'q' && opt[9] == 'i') {
            std::string tmp, trainID, date;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        break;
                    }
                }
            }
            trainList.QueryTrain(trainID, date);
        }
        else if (opt[0] == 'q' && opt[7] == 'i') {
            std::string tmp, date, from, to;
            bool cmp = 0; //0-time, 1-cost
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp[0] == 'c') {
                            cmp = 1;
                        }
                        break;
                    }
                }
            }
            trainList.QueryTicket(date, from, to, cmp);
        }
        else if (opt[0] == 'q' && opt[9] == 'n') {
            std::string tmp, date, from, to;
            bool cmp = 0; //0-time, 1-cost
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp[0] == 'c') {
                            cmp = 1;
                        }
                        break;
                    }
                }
            }
            trainList.QueryTransfer(date, from, to, cmp);
        }
        else if (opt[0] == 'b') {
            std::string tmp, username, trainID, date, from, to;
            int num; bool que = false;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                    case 'f': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'q': {
                        ss >> tmp;
                        if (tmp == "true") {
                            que = true;
                        }
                        else if (tmp == "false") {
                            que = false;
                        }
                        break;
                    }
                }
            }
            orderList.BuyTicket(username, trainID, date, from, to, num, que);
        }
        else if (opt[0] == 'q' && opt[9] == 'e') {
            std::string tmp, username;
            ss >> tmp >> username;
            orderList.QueryOrder(username);
        }
        else if (opt[0] == 'r' && opt[2] == 'f') {
            std::string tmp, username;
            int num;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                }
            }
            orderList.RefundTicket(username, num);
        }
        else if (opt[0] == 'e') {
            //userList.exit();
            std::cout << "bye" << std::endl;
            return 0;
        }
    }
    return 0;
}
