#ifndef TICKETSYSTEM_USER_H
#define TICKETSYSTEM_USER_H

#include "utils/DataType.h"
#include "utils/Database.h"
#include "utils/Map.h"

struct User {
    mString id, password, name, mailAddr;
    int pvl;
    void print() {
        std::cout << id << " " << name << " " << mailAddr << " " << pvl;
    }
};

class UserList {
    friend class TicketSystem;
public:
    UserList() : UserData("uBPT", "uList") {}

    ~UserList() = default;

    void AddUser(const std::string &curID, const std::string &ID, const std::string &pwd,
                 const std::string &name, const std::string &mailAddr, const int pvl) {
        User newUser;
        if (UserData.n == -1) {
            newUser = {ID, pwd, name, mailAddr, 10};
            UserData.add(ID, newUser);
            std::cout << "0\n";
            return;
        }
        if (!loggedIn(curID)) {
            std::cout << "-1\n";
            return;
        }
        User temp;
        if (UserData.found(ID, temp) != -1) {
            std::cout << "-1\n";
            return;
        }
        UserData.found(curID, temp);
        if (temp.pvl <= pvl) {
            std::cout << "-1\n";
            return;
        }
        newUser = {ID, pwd, name, mailAddr, pvl};
        UserData.add(ID, newUser);
        std::cout << "0\n";
    }

    void Login(const std::string &ID, const std::string &pwd) {
        User user;
        if (loggedIn(ID) || UserData.found(ID, user) == -1 || user.password != pwd) {
            std::cout << "-1\n";
            return;
        }
        LoginList[ID] = 1;
        std::cout << "0\n";
    }

    void Logout(const std::string &ID) {
        if (!loggedIn(ID)) {
            std::cout << "-1\n";
            return;
        }
        LoginList.erase(LoginList.find(ID));
        std::cout << "0\n";
    }

    void QueryProfile(const std::string &curID, const std::string &ID) {
        User curUser, queryUser;
        if (!loggedIn(curID)) {
            std::cout << "-1\n";
            return;
        }
        if (UserData.found(ID, queryUser) == -1) {
            std::cout << "-1\n";
            return;
        }
        UserData.found(curID, curUser);
        if (curUser.pvl <= queryUser.pvl && curID != ID) {
            std::cout << "-1\n";
            return;
        }
        queryUser.print();
        std::cout << '\n';
    }

    void ModifyProfile(const std::string &curID, const std::string &ID, const std::string &pwd,
                       const std::string &name, const std::string &mailAddr, const int pvl) {
        User curUser, modifyUser;
        if (!loggedIn(curID)) {
            std::cout << "-1\n";
            return;
        }
        if (UserData.found(ID, modifyUser) == -1) {
            std::cout << "-1\n";
            return;
        }
        UserData.found(curID, curUser);
        if (curUser.pvl <= modifyUser.pvl && curID != ID) {
            std::cout << "-1\n";
            return;
        }
        if (curUser.pvl <= pvl) {
            std::cout << "-1\n";
            return;
        }
        if (!pwd.empty()) modifyUser.password = pwd;
        if (!name.empty()) modifyUser.name = name;
        if (!mailAddr.empty()) modifyUser.mailAddr = mailAddr;
        if (pvl != -1) modifyUser.pvl = pvl;
        UserData.modify(ID, modifyUser);
        modifyUser.print();
        std::cout << '\n';
    }

    bool loggedIn(const mString &s) {
        return LoginList.find(s) != LoginList.end();
    }

    void exit() {
        LoginList.clear();
    }

private:

    Database<mString, User> UserData;

    map<mString, int> LoginList;
};

#endif //TICKETSYSTEM_USER_H
