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
bool isEscapePressed();
void gotoxy(short x, short y);
std::string inputString(const std::string &prompt);
std::string currentDate();
int currentMonth();
int currentDay();
int currentYear();
std::string toLowerCase(const std::string &str);
int inputInteger(const std::string &description);
std::string inputPassword(const std::string &prompt);
std::string inputPhoneNumber(const std::string);
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
    Date &operator=(const Date &other);
    bool setDate(const std::string &input);
    bool operator==(const Date &other) const;
};

class Time : public Date {
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
};
bool ensureDirectoryExists(const std::string &filepath);
#endif  // _MY_LIB_H_
