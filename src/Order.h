#ifndef TICKETSYSTEM_ORDER_H
#define TICKETSYSTEM_ORDER_H

#include "User.h"
#include "Train.h"

extern UserList userList;
extern TrainList trainList;

struct Order {
    int status; //0-success 1-pending 2-refunded
    mString trainID;
    int trainNID, trainNo;
    mString start, end;
    int st, ed, levDate, levTime, arrDate, arrTime, price, number;

    void print() {
        if (status == 0) std::cout << "[success] ";
        else if (status == 1) std::cout << "[pending] ";
        else std::cout << "[refunded] ";
        std::cout << trainID << " " << start << " " << printDay(levDate) << " " << printTime(levTime)
                  << " -> " << end << " " << printDay(arrDate) << " " << printTime(arrTime) << " "
                  << price << " " << number;
    }
};

class OrderList {
public:
    OrderList() : OrderData("oBPT", "oList"), PendingList("Pending") {}

    ~OrderList() = default;

    void BuyTicket(const std::string &username, const std::string &ID, const std::string &date,
                   const std::string &from, const std::string &to, const int num, const bool que) {
        if (!userList.loggedIn(username)) {
            std::cout << "-1\n";
            return;
        }
        int day = ToShort(date);
        auto f = trainList.QueryData.range(std::pair<mString, mString>(from, to));
        bool flag = false;
        Info info;
        Train train;
        for (auto it : f) {
            trainList.QueryData.get(it, info);
            if (info.TrainID == ID) {
                trainList.TrainData.get(info.TrainNID, train);
                flag = true;
                break;
            }
        }
        if (!flag) {
            std::cout << "-1\n";
            return;
        }
        short TrainNo = passDays(beforeDays_n(day, train.levDayPass[info.from]), train.saleDate[0]);
        if (TrainNo < 0 || TrainNo > passDays(train.saleDate[1], train.saleDate[0]) || num > train.seatNum) {
            std::cout << "-1\n";
            return;
        }
        int checkMin = trainList.QuerySeat.find(getNo(info.TrainNID, TrainNo)).min(info.from, info.to);
        if (num <= checkMin) {

            Seat seat = trainList.QuerySeat.find(getNo(info.TrainNID, TrainNo)).buy(info.from, info.to, num);

            trainList.QuerySeat.modify(getNo(info.TrainNID, TrainNo), seat);

            std::cout << num * (train.totalPrice[info.to] - train.totalPrice[info.from]);
            Order orderInfo = {0, train.TrainID, info.TrainNID, TrainNo, from, to, info.from, info.to,
                               day, train.levTime[info.from],
                               nextDays_n(day, train.arrDayPass[info.to] - train.levDayPass[info.from]),
                               train.arrTime[info.to],
                               train.totalPrice[info.to] - train.totalPrice[info.from], num};
            OrderData.add(username, orderInfo);
            std::cout << '\n';
        }
        else {
            if (que) {
                std::cout << "queue";

                int pending = getNo(train.TrainNID, TrainNo);
                Order orderInfo = {1, train.TrainID, info.TrainNID, TrainNo, from, to, info.from, info.to,
                                   day, train.levTime[info.from],
                                   nextDays_n(day, train.arrDayPass[info.to] - train.levDayPass[info.from]),
                                   train.arrTime[info.to],
                                   train.totalPrice[info.to] - train.totalPrice[info.from], num};
                OrderData.add(username, orderInfo);
                PendingList.insert(pending, OrderData.n);
                std::cout << '\n';
            }
            else {
                std::cout << "-1\n";
            }
        }
    }

    void RefundTicket(const std::string &username, const int n) {
        if (!userList.loggedIn(username)) {
            std::cout << "-1\n";
            return;
        }
        auto f = OrderData.range(username);
        if (f.size() < n) {
            std::cout << "-1\n";
            return;
        }
        int it = f.size();
        for (int i = 1; i <= n; ++i) {it--;}
        Order orderInfo;
        OrderData.get(f[it], orderInfo);
        if (orderInfo.status == 2) {
            std::cout << "-1\n";
            return;
        }
        else if (orderInfo.status == 1) {
            orderInfo.status = 2;
            OrderData.modify(f[it], orderInfo, 0);
            PendingList.erase(getNo(orderInfo.trainNID, orderInfo.trainNo), f[it]);
            std::cout << "0\n";
        }
        else {
            orderInfo.status = 2;
            OrderData.modify(f[it], orderInfo, 0);
            Order temp;
            int pos = getNo(orderInfo.trainNID, orderInfo.trainNo);

            Seat seat = trainList.QuerySeat.find(pos).buy(orderInfo.st, orderInfo.ed, 0 - orderInfo.number);

            trainList.QuerySeat.modify(pos, seat);

            auto ff = PendingList.find(pos);
            for (int i = 0; i < ff.size(); ++i) {
                OrderData.get(ff[i], temp);
                if (temp.number <= trainList.QuerySeat.find(pos).min(temp.st, temp.ed)) {

                    Seat seat2 = trainList.QuerySeat.find(pos).buy(temp.st, temp.ed, temp.number);

                    trainList.QuerySeat.modify(pos, seat2);

                    temp.status = 0;
                    OrderData.modify(ff[i], temp, 0);
                    PendingList.erase(pos, ff[i]);
                }
            }
            std::cout << "0\n";
        }
    }

    void QueryOrder(const std::string &username) {
        if (!userList.loggedIn(username)) {
            std::cout << "-1\n";
            return;
        }
        auto f = OrderData.range(username);
        std::cout << f.size();
        for (int i = f.size() - 1; i >= 0; --i) {
            std::cout << "\n";
            Order orderInfo;
            OrderData.get(f[i], orderInfo);
            int k = f[i];
            orderInfo.print();
        }
        std::cout << "\n";
    }

private:
    Database<mString, Order> OrderData;

    BPlusTree<int, int> PendingList;
};

#endif //TICKETSYSTEM_ORDER_H
