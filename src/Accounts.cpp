#include "../include/Accounts.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "../include/my_lib.h"

Accounts::Accounts(const std::string &filename) { loadFromFile(filename); }
Accounts::~Accounts() {}

bool Accounts::updateAccount(Account &account) {
  for (auto &acc : accounts) {
    if (acc.getUserId() == account.getUserId()) {
      acc = account;
      saveToFile(ACCOUNT_FILE);
      return true;
    }
  }
  return false;
}

void Accounts::setPhoneNumber(std::string &numberPhone) {
  while (true) {
    std::cin.ignore();
    // numberPhone = inputPhoneNumber(
    //     "Enter phone number: ");  // Yêu cầu nhập số điện thoại

    // Kiểm tra xem số điện thoại đã tồn tại chưa
    bool phoneExists = false;
    for (const auto &acc : accounts) {
      if (acc.getPhoneNumber() == numberPhone) {
        phoneExists = true; // Tìm thấy số điện thoại trùng lặp
        break;
      }
    }

    if (phoneExists) {
      std::cout << "Phone number already exists. Please choose a "
                   "different number."
                << std::endl;
    } else {
      return; // Số điện thoại hợp lệ, thoát khỏi vòng lặp
    }
  }
}

bool Accounts::isUsernameTaken(const std::string &username) {
  for (const auto &account : accounts) {
    if (username == account.getUserName()) {
      return true;
    }
  }
  return false;
}

void Accounts::addAccount(Account &account) {
  int userId;
  if (!accounts.empty()) {
    userId = accounts.back().getUserId() + 1;
  } else {
    userId = 10000;
  }
  // phone number

  account.setUserId(userId);
  account.setRole(2);
  accounts.push_back(account);
  account.saveToFile(ACCOUNT_FILE);

  std::cout << "Account created successfully." << std::endl;
}
// bool Accounts::signIn() {
//     std::cin.ignore();
//     std::string userName, passWord, phoneNumber;
//     int userId;

//     setUserName(userName);
//     passWord = inputPassword("Enter password: ");
//     setPhoneNumber(phoneNumber);

//     // TODO: Generate userId
//     if (!accounts.empty()) {
//         userId = accounts.back().getUserId() + 1;
//     } else {
//         userId = 10000;
//     }

//     Account account{userId, userName, passWord, phoneNumber,
//                     2};  // Assuming default role is 2 (user)
//     accounts.push_back(account);
//     account.saveToFile(ACCOUNT_FILE);

//     std::cout << "Account created successfully." << std::endl;
//     return true;
// }
void Accounts::forgotPassword(Account &account, std::string &message) {
  for (auto &acc : accounts) {
    if (acc.getUserName() == account.getUserName() &&
        acc.getPhoneNumber() == account.getPhoneNumber()) {
      if (account.getRole() == 2) {
        acc.setPassWord(account.getPassWord());
        saveToFile(ACCOUNT_FILE);
        message = "Password changed successfully.";
        return;
      } else if (account.getRole() == 1) {
        message = "Your don't have perrmisson to change password";
        return;
      }
    }
  }
  message = "Account does not exist.";

}
bool Accounts::authenticate(const std::string &username,
                            const std::string &password, Account &acc) {
  for (const auto &account : accounts) {
    if (username == account.getUserName() &&
        password == account.getPassWord()) {
      acc = account;
      return true;
    }
  }
  return false;
}

void Accounts::deleteInformation(int userId) {
  for (auto it = accounts.begin(); it != accounts.end(); ++it) {
    if (it->getUserId() == userId) {
      accounts.erase(it);
      std::cout << "Account deleted successfully." << std::endl;
      return;
    }
  }
  std::cout << "Account with userId " << userId << " not found." << std::endl;
}

void Accounts::display() const {
  for (const Account &account : accounts) {
    account.display();
  }
}

void Accounts::displayAll() {
  if (accounts.empty()) {
    std::cout << "No accounts to display." << std::endl;
  } else {
    std::cout << "Accounts list:" << std::endl;
    for (const auto &acc : accounts) {
      acc.display();
    }
  }
}

void Accounts::displayHistoryTicket() {
  Account acc = findUser();
  if (acc.getUserId() != -1) { // Check if account is valid
    std::cout << "History for user " << acc.getUserName() << ":" << std::endl;
    // acc.displayTicketHistory();
  } else {
    std::cout << "User does not exist!" << std::endl;
  }
}

Account Accounts::findUser() {
  std::string userName;

  std::cout << "Enter username to find: ";
  std::cin >> userName;
  userName = toLowerCase(userName);

  for (const auto &acc : accounts) {
    if (acc.getUserName() == userName) {
      std::cout << "User found:" << std::endl;
      acc.display();
      return acc;
    }
  }

  std::cout << "User not found." << std::endl;
  return Account(-1, "", "", "",
                 0); // Return a default account indicating not found
}

bool Accounts::isUserExist(int userId) const {
  for (const auto &account : accounts) {
    if (account.getUserId() == userId) {
      return true;
    }
  }
  return false;
}

bool Accounts::loadFromFile(const std::string &filename) {
  if (!ensureDirectoryExists(filename)) {
    return false; // If the directory doesn't exist and couldn't be
                  // created, return false
  }
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    std::ofstream createFile(filename);
    if (!createFile) {
      std::cerr << "Create File failed!" << std::endl;
      return false;
    } else {
      std::cerr << "Create File successfully!" << std::endl;
    }
  }

  if (isEmpty(inFile)) {
    std::cerr << "File is empty!";
    return false;
  }

  accounts.clear();
  std::string line;
  while (std::getline(inFile, line)) {
    std::stringstream ss(line);

    Account newAccount;
    newAccount.readFromFile(ss);

    accounts.push_back(newAccount);
  }

  inFile.close();
  return true;
}

bool Accounts::saveToFile(const std::string &filename) const {
  if (!ensureDirectoryExists(filename)) {
    return false; // If the directory doesn't exist and couldn't be
                  // created, return false
  }
  // Mở tệp để ghi
  std::ofstream outFile(filename);

  // Kiểm tra xem tệp có được mở thành công không
  if (!outFile) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;

    // Thử tạo tệp mới
    outFile.clear(); // Đặt lại trạng thái lỗi
    outFile.open(filename, std::ios::out | std::ios::trunc);

    // Kiểm tra nếu vẫn không mở được tệp mới
    if (!outFile) {
      std::cerr << "Error creating file: " << filename << std::endl;
      return false;
    }
  }

  // Ghi thông tin tài khoản vào tệp
  for (const auto &account : accounts) {
    outFile << account.getUserId() << ';' << account.getUserName() << ';'
            << account.getPassWord() << ';' << account.getPhoneNumber() << ';'
            << account.getRole() << '\n';
  }

  // Đóng tệp
  outFile.close();
  return true;
}

const Vector<Account> &Accounts::getAccounts() const { return accounts; }
