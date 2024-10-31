#ifndef _MY_LIB_H_
#define _MY_LIB_H_

#include <conio.h>
#include <windows.h>

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

// #include <thread>

#include "Accounts.h"
#include "Vector.h"
#define MOVIE_FILE "data\\movies.txt"
#define SHOWTIME_FILE "data\\showtimes.txt"
#define TICKET_FILE "data\\tickets.txt"
#define ACCOUNT_FILE "data\\accounts.txt"
std::string currentTime();
int currentHour();
int currentMinute();
bool isValidString(const std::string &input);
bool isValidStringNoSymbol(const std::string &input);
bool isEscapePressed();
std::string inputString(const std::string &prompt);
std::string currentDate();
int currentMonth();
int currentDay();
int currentYear();
std::string toLowerCase(const std::string &str);
bool validInteger(const std::string &value);
std::string inputPassword(const std::string &prompt);
bool isValidPhoneNumber(const std::string &phoneNumber);
bool isWordInString(const std::string &str, const std::string &word);
bool isAllWordsInString(const std::string &source, const std::string &target);
bool isLeapYear(short year);
short getDaysInMonth(short month, short year);

bool isEmpty(std::ifstream &pFile);

class Date {
   public:
    short day;
    short month;
    short year;
    Date(const std::string &date);
    Date(short d = 1, short m = 1, short y = 2000)
        : day(d), month(m), year(y) {}
    short getMonth();
    short getDay();
    short getYear();

    friend std::ostream &operator<<(std::ostream &os, const Date &date);
    friend std::istream &operator>>(std::istream &is, Date &date);
    bool operator>(const Date &other) const;
    bool operator<(const Date &other) const;
    Date &operator=(const Date &other);
    bool setDate(const std::string &input);
    bool operator==(const Date &other) const;
    std::string toString() const {
        std::ostringstream os;
        os << std::setw(2) << std::setfill('0') << day << "-" << std::setw(2)
           << std::setfill('0') << month << "-" << std::setw(4)
           << year;  // Năm hiển thị 4 chữ số
        return os.str();
    }
};

class Time {
   public:
    short hour;
    short minute;

    Time(short hour = 0, short minute = 0);
    Time(const Time &other);
    Time(const std::string &time);

    ~Time();

    bool setTime(const std::string &input);
    friend std::ostream &operator<<(std::ostream &os, const Time &time);
    friend std::istream &operator>>(std::istream &is, Time &time);
    bool operator>(const Time &other) const {
        if (hour > other.hour) {
            return true;  // So sánh giờ
        }
        if (hour == other.hour) {
            return minute > other.minute;  // So sánh phút
        }
        return false;  // Nếu giờ nhỏ hơn
    }
    bool operator<(const Time &other) const {
        if (hour < other.hour) {
            return true;  // So sánh gi��
        }
        if (hour == other.hour) {
            return minute < other.minute;  // So sánh phút
        }
        return false;  // Nếu gi�� l��n hơn
    }
    std::string toString() const {
        return std::to_string(hour) + ":" + std::to_string(minute);
    }
   Time add(int minutesToAdd) const {
        Time newTime = *this; // Tạo một bản sao của đối tượng hiện tại

        // Cộng phút
        newTime.minute += minutesToAdd;

        // Xử lý trường hợp phút vượt quá 60
        if (newTime.minute >= 60) {
            newTime.hour += newTime.minute / 60; // Cộng giờ
            newTime.minute %= 60; // Cập nhật phút
        }

        // Xử lý trường hợp giờ vượt quá 24
        if (newTime.hour >= 24) {
            newTime.hour %= 24; // Vòng lại giờ
        }

        return newTime; // Trả về đối tượng Time mới
    }
};
bool ensureDirectoryExists(const std::string &filepath);
#endif  // _MY_LIB_H_
