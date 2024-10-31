#ifndef _ACCOUNTS_H
#define _ACCOUNTS_H

#include <string>

#include "Account.h"
#include "Vector.h"

class Accounts {
   private:
    Vector<Account> accounts;
    bool isUserExist(int userId) const;

   public:
    bool updateAccount(Account& account);
    const Vector<Account>& getAccounts() const;
    void addAccount(Account& account);
        // bool signIn();
        bool authenticate(const std::string& username,
                          const std::string& password);
    bool updateInformation(int userId);
    void deleteInformation(int userId);
    bool updateInformationByUserIdForAdmin();
    void display() const;
    void displayAll();
    void displayHistoryTicket();
    Account findUser();
    bool isUsernameTaken(const std::string& username);
    void setPhoneNumber(std::string& numberPhone);

    Accounts(const std::string& filename);
    Accounts() {}
    ~Accounts();
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

};

#endif  // _ACCOUNTS_H
