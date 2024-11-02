#include "../include/Account.h"

#include <iostream>
#include <string>

#include "../include/my_lib.h"

// int Account::userId = 0; // Initialize static member

Account::Account(int userId, std::string userName, std::string passWord,
                 std::string phoneNumber, int role)
    : userId(userId), userName(userName), passWord(passWord),
      phoneNumber(phoneNumber), role(role) {}

Account::Account(const Account &source)
    : Account(source.userId, source.userName, source.passWord,
              source.phoneNumber, source.role) {}

Account::Account(const std::string &username, const std::string &password)
    : Account(0, username, password, "", 2) {}

Account::Account(const std::string &username, const std::string &password,
                 const std::string &phoneNumber)
    : Account(0, username, password, phoneNumber, 2) {}

Account::Account() {}

Account::~Account() {}

void Account::setUserId(int userId) { this->userId = userId; }
void Account::setUserName(const std::string &userName) {
  this->userName = userName;
}
void Account::setPassWord(const std::string &passWord) {
  this->passWord = passWord;
}
void Account::setPhoneNumber(const std::string &phoneNumber) {
  this->phoneNumber = phoneNumber;
}
void Account::setRole(int role) { this->role = role; }

int Account::getUserId() const { return userId; }
std::string Account::getUserName() const { return userName; }
std::string Account::getPassWord() const { return passWord; }
std::string Account::getPhoneNumber() const { return phoneNumber; }
int Account::getRole() const { return role; }

bool Account::isAdmin() const { return role == 1; }

void Account::display() const {
  std::cout << userId << "\t";
  std::cout << userName << "\t";
  std::cout << passWord << "\t";
  std::cout << phoneNumber << "\n";
}

void Account::displayCol() const {
  int count = 1;
  std::cout << count++ << ".userid: " << userId << "\n";
  std::cout << count++ << ".username: " << userName << "\n";
  std::cout << count++ << ".password: " << passWord << "\n";
  std::cout << count << ".phone number: " << phoneNumber << "\n";
}

void Account::readFromFile(std::stringstream &ss) {

  std::string item;
  std::getline(ss, item, ';');
  userId = std::stoi(item);
  std::getline(ss, userName, ';');
  std::getline(ss, passWord, ';');
  std::getline(ss, phoneNumber, ';');
  std::getline(ss, item);
  role = std::stoi(item);
}

bool Account::saveToFile(const std::string &filename) const {
  if (!ensureDirectoryExists(filename)) {
    return false; // If the directory doesn't exist and couldn't be
                  // created, return false
  }
  // Mở tệp với chế độ append để ghi tiếp vào cuối tệp
  std::ofstream outFile(filename, std::ios::app);
  if (!outFile) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return false; // Không cần thử mở lại với chế độ tạo mới
  }

  // Ghi thông tin tài khoản vào tệp
  outFile << userId << ';' << userName << ';' << passWord << ';' << phoneNumber
          << ';' << role << '\n';

  outFile.close(); // Đóng tệp sau khi ghi
  return true;     // Trả về true khi ghi thành công
}
