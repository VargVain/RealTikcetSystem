#ifndef TICKETSYSTEM_TRAIN_H
#define TICKETSYSTEM_TRAIN_H

#include "utils/DataType.h"
#include "utils/Database.h"
#include "utils/Calendar.h"
#include "utils/Map.h"
#include "utils/PriorityQueue.h"

struct Train {
    mString TrainID, station[100];
    int stationNum, seatNum;
    short TrainNID, saleDate[2], levTime[100], arrTime[100];
    short arrDayPass[100], levDayPass[100];
    int totalTime[100], totalPrice[100];
    bool released = 1;
    char type;
};

struct Seat {
    int seat[100];
    Seat set(int stationNum, int n) {
        for (int i = 1; i < stationNum; ++i) {
            seat[i] = n;
        }
        return *this;
    }
    Seat buy(int start, int end, int n) {
        for (int i = start; i < end; ++i) {
            seat[i] -= n;
        }
        return *this;
    }
    int min(int start, int end) {
        int min = seat[start];
        for (int i = start; i < end; ++i) {
            min = seat[i] < min ? seat[i] : min;
        }
        return min;
    }
};

struct Info {
    short TrainNID, from, to;
    mString TrainID;
    int cmp = 0;
};

class Compare {
public:
    bool operator()(const Info &lhs, const Info &rhs) {
        return lhs.cmp < rhs.cmp || ((lhs.cmp == rhs.cmp) && (lhs.TrainID < rhs.TrainID));
    }
};

class TrainList {
    friend class TicketSystem;
public:
    TrainList() : TrainData("tBPT", "tlist"), QueryData("qBPT", "qList"), QuerySeat("sBPT", "sList"), TransferData("trBPT", "trList") {}
    ~TrainList() = default;

    void AddTrain(const std::string &ID, const int stationNum, const int seatNum,
                  const std::string &stations, const std::string &prices, const std::string &startTime,
                  const std::string &travelTimes, const std::string &stopoverTimes,
                  const std::string &saleDate, const char _type) {
        Train newTrain;
        if (TrainData.found(ID, newTrain) != -1) {
            std::cout << "-1\n";
            return;
        }
        newTrain.TrainID = ID;
        newTrain.stationNum = stationNum;
        newTrain.type = _type;
        newTrain.seatNum = seatNum;
        newTrain.released = false;
        std::stringstream ss;
        ss << stations;
        for (int i = 1; i <= stationNum; ++i) {
            std::string sta;
            getline(ss, sta, '|');
            newTrain.station[i] = sta;
        }
        ss.clear();
        ss << prices;
        newTrain.totalPrice[1] = 0;
        for (int i = 2; i <= stationNum; ++i) {
            std::string pri;
            getline(ss, pri, '|');
            newTrain.totalPrice[i] = std::stoi(pri) + newTrain.totalPrice[i - 1];
        }
        ss.clear();

        newTrain.levTime[1] = ToShort(startTime);

        int travelTime[100];
        ss << travelTimes;
        for (int i = 1; i < stationNum; ++i) {
            std::string tra;
            getline(ss, tra, '|');
            travelTime[i] = std::stoi(tra);
        }
        ss.clear();

        int stopoverTime[100];
        ss << stopoverTimes;
        if (stationNum > 2) {
            for (int i = 1; i < stationNum - 1; ++i) {
                std::string sto;
                getline(ss, sto, '|');
                stopoverTime[i] = std::stoi(sto);
            }
        }
        ss.clear();
        ss << saleDate;
        for (int i = 0; i < 2; ++i) {
            std::string dat;
            getline(ss, dat, '|');
            newTrain.saleDate[i] = ToShort(dat);
        }

        int day = 0, num = newTrain.stationNum, time = newTrain.levTime[1], totalTime = 0, totalPrice = 0;
        for (int i = 1; i <= num; ++i) {
            int dayPass;
            if (i == 1) {
                newTrain.levTime[1] = time;
                newTrain.levDayPass[1] = 0;
                newTrain.totalTime[1] = 0;
            }
            else if (i == num) {
                dayPass = addTime(time, travelTime[i - 1]);
                nextDays(day, dayPass);
                newTrain.arrDayPass[i] = day;
                newTrain.arrTime[i] = time;
                totalTime += travelTime[i - 1];
                newTrain.totalTime[i] = totalTime;
            }
            else {
                dayPass = addTime(time, travelTime[i - 1]);
                nextDays(day, dayPass);
                newTrain.arrDayPass[i] = day;
                newTrain.arrTime[i] = time;
                dayPass = addTime(time, stopoverTime[i - 1]);
                nextDays(day, dayPass);
                newTrain.levTime[i] = time;
                newTrain.levDayPass[i] = day;
                totalTime += travelTime[i - 1] + stopoverTime[i - 1];
                newTrain.totalTime[i] = totalTime;
            }
        }
        newTrain.TrainNID = TrainData.n + 1;
        TrainData.add(ID, newTrain);

        for (int k = newTrain.saleDate[0], trainNo = 1; k <= newTrain.saleDate[1]; nextDay(k), ++trainNo) {
            Seat seat;
            seat.set(newTrain.stationNum, newTrain.seatNum);
            QuerySeat.add(newTrain.TrainNID * 100 + trainNo, seat);
        }
        std::cout << "0\n";
    }

    void DeleteTrain(const std::string &ID) {
        Train train;
        if (TrainData.found(ID, train) == -1) {
            std::cout << "-1\n";
            return;
        }
        if (train.released) {
            std::cout << "-1\n";
            return;
        }
        TrainData.erase(ID);
        std::cout << "0\n";
    }

    void ReleaseTrain(const std::string &ID) {
        Train train;
        if (TrainData.found(ID, train) == -1) {
            std::cout << "-1\n";
            return;
        }
        if (train.released) {
            std::cout << "-1\n";
            return;
        }
        train.released = true;
        TrainData.modify(train.TrainID, train);
        short num = train.stationNum;
        for (short i = 1; i <= num - 1; ++i) {
            for (short j = i + 1; j <= num; ++j) {
                Info info = {train.TrainNID, i, j, train.TrainID};
                QueryData.add(std::pair<mString, mString>(train.station[i], train.station[j]),
                        info);
                TransferData.add(train.station[i], train.station[j]);
            }
        }
        std::cout << "0\n";
    }

    void QueryTrain(const std::string &ID, const std::string &date_) {
        Train train;
        if (TrainData.found(ID, train) == -1) {
            std::cout << "-1\n";
            return;
        }
        int date = ToShort(date_);
        if ((date < train.saleDate[0]) || (date > train.saleDate[1])) {
            std::cout << "-1\n";
            return;
        }

        std::cout << train.TrainID << " " << train.type;
        int num = train.stationNum, trainNo = passDays(date, train.saleDate[0]);
        for (int i = 1; i <= num; ++i) {
            std::cout << '\n';
            std::string date1, time1, date2, time2;
            if (i == 1) {
                date1 = "xx-xx";
                time1 = "xx:xx";
                date2 = printDay(date);
                time2 = printTime(train.levTime[1]);
                std::cout << train.station[i] << " " << date1 << " " << time1 << " -> ";
                std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                          << QuerySeat.find(train.TrainNID * 100 + trainNo).seat[i];
            }
            else if (i == num) {
                date1 = printDay(nextDays_n(date, train.arrDayPass[i]));
                time1 = printTime(train.arrTime[i]);
                date2 = "xx-xx";
                time2 = "xx:xx";
                std::cout << train.station[i] << " " << date1 << " " << time1 << " -> ";
                std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                          << "x";
            }
            else {
                date1 = printDay(nextDays_n(date, train.arrDayPass[i]));
                time1 = printTime(train.arrTime[i]);
                date2 = printDay(nextDays_n(date, train.levDayPass[i]));
                time2 = printTime(train.levTime[i]);
                std::cout << train.station[i] << " " << date1 << " " << time1 << " -> ";
                std::cout << date2 << " " << time2 << " " << train.totalPrice[i] << " "
                          << QuerySeat.find(train.TrainNID * 100 + trainNo).seat[i];
            }
        }
        std::cout << '\n';
    }

    void QueryTicket(const std::string &date, const std::string &from, const std::string &to, bool cmp) {
        int day = ToShort(date);
        auto f = QueryData.range(std::pair<mString, mString>(from, to));
        priority_queue<Info, Compare> ans;
        for (auto it : f) {
            Info info;
            Train train;
            QueryData.get(it, info);
            TrainData.get(info.TrainNID, train);
            short TrainNo = passDays(beforeDays_n(day, train.levDayPass[info.from]), train.saleDate[0]);
            if (TrainNo < 0 || TrainNo > passDays(train.saleDate[1], train.saleDate[0])) continue;
            if (cmp) info.cmp = train.totalPrice[info.to] - train.totalPrice[info.from];
            else info.cmp = passTime(train.arrDayPass[info.to], train.arrTime[info.to],
                                     train.levDayPass[info.from], train.levTime[info.from]);
            ans.push(info);
        }
        std::cout << ans.size();
        while(!ans.empty()) {
            Info it = ans.top();
            ans.pop();
            Train train;
            TrainData.get(it.TrainNID, train);
            std::cout << '\n' << train.TrainID << " " << from << " " << date << " "
                      << printTime(train.levTime[it.from]) << " -> "
                      << to << " " << printDay(nextDays_n(day, train.arrDayPass[it.to] - train.levDayPass[it.from]))
                      << " " << printTime(train.arrTime[it.to]) << " " << train.totalPrice[it.to] - train.totalPrice[it.from] << " "
                      << QuerySeat.find(it.TrainNID * 100 + passDays(beforeDays_n(day, train.levDayPass[it.from]), train.saleDate[0])).min(it.from, it.to);
        }
        std::cout << '\n';
    }

    void QueryTransfer(const std::string &date, const std::string &from, const std::string &to, bool cmp) {
        int day = ToShort(date);
        Info minInfo1, minInfo2;
        int minNo1, minNo2;
        int mintotalCost, minTime;
        mString minID1, minID2;
        bool found = false;
        auto f = TransferData.range(from);
        for (auto it : f) {
            mString mid;
            TransferData.get(it, mid);
            auto ff = QueryData.range(std::pair<mString, mString>(from, mid));
            for (auto itt : ff) {
                Train train1;
                Info info1;
                QueryData.get(itt, info1);
                TrainData.get(info1.TrainNID, train1);
                short TrainNo1 = passDays(beforeDays_n(day, train1.levDayPass[info1.from]), train1.saleDate[0]);
                if (TrainNo1 < 0 || TrainNo1 > passDays(train1.saleDate[1], train1.saleDate[0])) continue;
                int arrDate1 = nextDays_n(train1.saleDate[0], TrainNo1 + train1.arrDayPass[info1.to] - 1);
                int arrTime1 = train1.arrTime[info1.to];
                auto fff = QueryData.range(std::pair<mString, mString>(mid, to));
                for (auto ittt : fff) {
                    Train train2;
                    Info info2;
                    QueryData.get(ittt, info2);
                    TrainData.get(info2.TrainNID, train2);
                    if (train2.TrainNID == train1.TrainNID) continue;
                    int day2 = arrDate1 > nextDays_n(train2.saleDate[0], train2.levDayPass[info2.from]) ? arrDate1 : nextDays_n(train2.saleDate[0], train2.levDayPass[info2.from]);
                    if (day2 == arrDate1 && train2.levTime[info2.from] < arrTime1) ++day2;
                    short TrainNo2 = passDays(beforeDays_n(day2, train2.levDayPass[info2.from]), train2.saleDate[0]);
                    if (TrainNo2 < 0 || TrainNo2 > passDays(train2.saleDate[1], train2.saleDate[0])) continue;


                    int totalCost = train1.totalPrice[info1.to] - train1.totalPrice[info1.from] + train2.totalPrice[info2.to] - train2.totalPrice[info2.from];
                    int passtime = passTime(nextDays_n(train2.saleDate[0], TrainNo2 + train2.arrDayPass[info2.to] - 1), train2.arrTime[info2.to],
                                            nextDays_n(train1.saleDate[0], TrainNo1 + train1.arrDayPass[info2.from] - 1), train1.arrTime[info1.to]);
                    if (!found) {
                        minInfo1 = info1;
                        minInfo2 = info2;
                        mintotalCost = totalCost;
                        minTime = passtime;
                        minID1 = train1.TrainID;
                        minID2 = train2.TrainID;
                        minNo1 = TrainNo1;
                        minNo2 = TrainNo2;
                        found = true;
                        continue;
                    }
                    bool less = false;
                    if (cmp) {
                        if (totalCost < mintotalCost) less = true;
                        else if (totalCost > mintotalCost);
                        else {
                            if (passtime < minTime) less = true;
                            else if (passtime > minTime);
                            else {
                                if (train1.TrainID < minID1) less = true;
                                else if (train1.TrainID > minID1)
                                if (train2.TrainID < minID2) less = true;
                            }
                        }
                    }
                    else {
                        if (passtime < minTime) less = true;
                        else if (passtime > minTime);
                        else {
                            if (totalCost < mintotalCost) less = true;
                            else if (totalCost > mintotalCost);
                            else {
                                if (train1.TrainID < minID1) less = true;
                                else if (train1.TrainID > minID1)
                                    if (train2.TrainID < minID2) less = true;
                            }
                        }
                    }
                    if (less) {
                        minInfo1 = info1;
                        minInfo2 = info2;
                        mintotalCost = totalCost;
                        minTime = passtime;
                        minID1 = train1.TrainID;
                        minID2 = train2.TrainID;
                        minNo1 = TrainNo1;
                        minNo2 = TrainNo2;
                    }
                }
            }
        }
        if (!found) std::cout << "0\n";
        else {
            Train train1, train2;
            TrainData.get(minInfo1.TrainNID, train1);
            TrainData.get(minInfo2.TrainNID, train2);
            short TrainNo1 = minNo1;
            short TrainNo2 = minNo2;
            std::cout << minInfo1.TrainID << " " << from << " " << date << " "
                      << printTime(train1.levTime[minInfo1.from]) << " -> " << train1.station[minInfo1.to] << " " << printDay(nextDays_n(day, train1.arrDayPass[minInfo1.to] - train1.levDayPass[minInfo1.from]))
                      << " " << printTime(train1.arrTime[minInfo1.to]) << " " << train1.totalPrice[minInfo1.to] - train1.totalPrice[minInfo1.from] << " "
                      << QuerySeat.find(getNo(minInfo1.TrainNID, TrainNo1)).min(minInfo1.from, minInfo1.to);
            std::cout << '\n' << minInfo2.TrainID << " " << train2.station[minInfo2.from] << " " << printDay(nextDays_n(train2.saleDate[0], train2.levDayPass[minInfo2.from] + TrainNo2 - 1)) << " "
                      << printTime(train2.levTime[minInfo2.from]) << " -> " << train2.station[minInfo2.to] << " " << printDay(nextDays_n(train2.saleDate[0], train2.arrDayPass[minInfo2.to] + TrainNo2 - 1))
                      << " " << printTime(train2.arrTime[minInfo2.to]) << " " << train2.totalPrice[minInfo2.to] - train2.totalPrice[minInfo2.from] << " "
                      << QuerySeat.find(getNo(minInfo2.TrainNID, TrainNo2)).min(minInfo2.from, minInfo2.to)
                      << '\n';
        }
    }

    Database<mString, Train> TrainData;

    Database<std::pair<mString, mString>, Info> QueryData;

    Database<mString, mString> TransferData;

    Database<int, Seat> QuerySeat;

private:

};

#endif //TICKETSYSTEM_TRAIN_H
