#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include <string>

#include "Entity.h"

class Account {
   private:
    int userId;
    std::string userName;
    std::string passWord;
    std::string phoneNumber;
    int role;  // admin : 1, user : 2
   public:
    void setUserId(int userId);
    void setUserName(const std::string& userName);
    void setPassWord(const std::string& passWord);
    void setPhoneNumber(const std::string& phoneNumber);
    void setRole(int role);
    int getUserId() const;
    std::string getUserName() const;
    std::string getPassWord() const;
    std::string getPhoneNumber() const;
    int getRole() const;
    bool isAdmin() const;
    void display() const;
    void displayCol() const;
    // bool updateInformation();
    // bool confirmPassWord();
    bool saveToFile(const std::string& filename) const;
    void readFromFile(std::stringstream& ss);
    Account(const std::string& username, const std::string& password,
            const std::string& phoneNumber);
    Account(const std::string& username, const std::string& password);
    Account(int userId, std::string userName, std::string passWord,
            std::string phoneNumber, int role);
    Account(const Account& source);
    Account();
    ~Account();
    bool operator==(const Account& other) const {
        return this->userId == other.userId;
    }
};

#endif  // _ACCOUNT_H
