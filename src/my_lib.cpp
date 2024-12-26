#include "../include/my_lib.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>


std::string currentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%d-%m-%Y");  // Định dạng dd-mm-yyyy
    return oss.str();
}

std::string currentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%H:%M");  // Định dạng HH:mm
    return oss.str();
}

int currentYear() { return std::stoi(currentDate().substr(6, 4)); }
int currentMonth() { return std::stoi(currentDate().substr(3, 2)); }
int currentDay() { return std::stoi(currentDate().substr(0, 2)); }
int currentHour() { return std::stoi(currentDate().substr(0, 2)); }
int currentMinute() { return std::stoi(currentDate().substr(3, 2)); }

std::string toLowerCase(const std::string &str) {
    std::string lowerStr = str;
    for (char &c : lowerStr) {
        c = std::tolower(c);
    }
    return lowerStr;
}

bool validInteger(const std::string &value) {
    // Kiểm tra xem chuỗi có rỗng hay không
    if (value.empty()) {
        return false;
    }

    // Kiểm tra từng ký tự trong chuỗi
    for (char c : value) {
        if (!std::isdigit(c)) {
            return false;  // Nếu có ký tự không phải là số, trả về false
        }
    }

    return true;  // Nếu tất cả ký tự là số, trả về true
}
std::string inputString(const std::string &prompt) {
    std::string input;

    while (true) {
        std::cout << prompt;            // Hiển thị yêu cầu
        std::getline(std::cin, input);  // Đọc dòng nhập vào

        if (input.empty()) {
            std::cout << "Input cannot be empty. Please try again."
                      << std::endl;
            continue;  // Yêu cầu nhập lại
        }

        std::regex validStringRegex("^[a-zA-Z0-9_ ]+$");
        if (!std::regex_match(input, validStringRegex)) {
            std::cout << "Input can only contain letters, numbers, "
                         "underscores, and spaces. Please try again."
                      << std::endl;
            continue;
        }
        break;
    }
    return input;
}

bool isValidString(const std::string &input) {
    // Định nghĩa regex cho chuỗi hợp lệ (không có khoảng trắng, chỉ có chữ cái,
    // số, dấu gạch dưới và các ký tự đặc biệt như ., )
    std::regex validStringRegex("^[a-zA-Z0-9_.,]+$");
    return std::regex_match(input, validStringRegex) && input.length() < 20;
}
bool isValidStringNoSymbol(const std::string &input) {
    // Định nghĩa regex cho chuỗi hợp lệ (chỉ có chữ cái và số, số không xuất
    // hiện đầu tiên)
    std::regex validStringRegex("^[a-zA-Z][a-zA-Z0-9]{0,19}$");
    return std::regex_match(input, validStringRegex) && input.length() < 20;
}

bool isValidPhoneNumber(const std::string &phoneNumber) {
    // Kiểm tra độ dài của số điện thoại
    if (phoneNumber.size() != 10) {
        std::cerr
            << "Input is not a valid phone number. It must be 10 digits long."
            << std::endl;
        return false;
    }

    // Kiểm tra từng ký tự trong số điện thoại
    for (char c : phoneNumber) {
        if (!isdigit(c)) {
            std::cerr << "Input is not a valid phone number. It must contain "
                         "only digits."
                      << std::endl;
            return false;
        }
    }

    // Nếu tất cả kiểm tra đều hợp lệ
    return true;
}

bool isWordInString(const std::string &str, const std::string &word) {
    return str.find(word) != std::string::npos;
}

bool isAllWordsInString(const std::string &source, const std::string &target) {
    std::istringstream iss(source);
    std::string word;
    std::string lowerTarget = toLowerCase(target);

    while (iss >> word) {
        if (lowerTarget.find(toLowerCase(word)) == std::string::npos) {
            return false;
        }
    }
    return true;
}

bool isValidDateFormat(const std::string &input) {
    if(input.length() != 10) {return false;}
    return input.length() == 10 && input[2] == '-' && input[5] == '-';
}

bool Date::setDate(const std::string &input) {
    if (!isValidDateFormat(input)) {
        return false;
    }
    
    try {
        day = std::stoi(input.substr(0, 2));
        month = std::stoi(input.substr(3, 2));
        year = std::stoi(input.substr(6, 4));
    } catch (...) {
        return false;
    }

    if (month < 1 || month > 12 || day < 1 ||
        day > getDaysInMonth(month, year)) {
        return false;
    }

    // Kiểm tra nếu ngày lớn hơn ngày hiện tại
    auto now = std::chrono::system_clock::now();
    auto current_time = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&current_time);

    if (year < now_tm->tm_year + 1900 ||
        (year == now_tm->tm_year + 1900 && month < now_tm->tm_mon + 1) ||
        (year == now_tm->tm_year + 1900 && month == now_tm->tm_mon + 1 &&
         day <= now_tm->tm_mday)) {
        return false;
    }

    return true;
}

Date::Date(const std::string &date) { setDate(date); }

std::istream &operator>>(std::istream &is, Date &date) {
    std::string input;
    bool succeed = false;
    do {
        std::cout << "Enter date (dd-mm-yyyy): ";
        is >> input;
        succeed = date.setDate(input);
        if (!succeed) {
            std::cerr << "Invalid date format or value! Please try again.(Date)"
                      << std::endl;
            break;
        }
    } while (!succeed);
    return is;
}

std::ostream &operator<<(std::ostream &os, const Date &date) {
    os << std::setw(2) << std::setfill('0') << date.day << "-" << std::setw(2)
       << std::setfill('0') << date.month << "-" << std::setw(4) << date.year;
    return os;
}

short getDaysInMonth(short month, short year) {
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

bool isLeapYear(short year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

Date &Date::operator=(const Date &other) {
    if (this != &other) {
        this->day = other.day;
        this->month = other.month;
        this->year = other.year;
    }
    return *this;
}

void writeTimeToFile(const std::string &filename,
                     std::chrono::system_clock::time_point access_time) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file for writing");
    }

    auto now =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto access_time_t = std::chrono::system_clock::to_time_t(access_time);

    ofs << "Now time: "
        << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
        << std::endl;
    ofs << "Time need to access: "
        << std::put_time(std::localtime(&access_time_t), "%Y-%m-%d %H:%M:%S")
        << std::endl;
}

std::chrono::system_clock::time_point readTimeFromFile(
    const std::string &filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file for reading");
    }

    std::string line;
    std::getline(ifs, line);  // Skip the first line
    std::getline(ifs, line);  // Read the second line

    std::istringstream ss(line);
    std::string date_time_str;
    ss.ignore(std::numeric_limits<std::streamsize>::max(),
              ':');                   // Ignore till ":"
    std::getline(ss, date_time_str);  // Get the time after ": "

    std::tm tm = {};
    std::istringstream time_ss(date_time_str);
    time_ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    auto time_t = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time_t);
}

void clearScreen() { std::system("cls"); }

void checkAndPrintRemainingTime(const std::string &filename) {
    auto access_time = readTimeFromFile(filename);
    while (true) {
        clearScreen();
        auto now = std::chrono::system_clock::now();
        auto duration = access_time - now;
        if (duration <= std::chrono::seconds(0)) {
            std::cout << "Can access now." << std::endl;
            break;
        } else {
            auto seconds_left =
                std::chrono::duration_cast<std::chrono::seconds>(duration)
                    .count();
            std::cout << "Wait: " << seconds_left << " sec" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

short Date::getDay() { return day; }

short Date::getMonth() { return month; }

short Date::getYear() { return year; }

bool Date::operator==(const Date &other) const {
    // So sánh các thuộc tính của Date
    return (this->day == other.day && this->month == other.month &&
            this->year == other.year);
}

bool isEmpty(std::ifstream &pFile) {
    return pFile.peek() == std::ifstream::traits_type::eof();
}

bool Time::setTime(const std::string &input) {
    if (input.length() != 5 || input[2] != ':') {
        return false;
    }
    try {
        hour = std::stoi(input.substr(0, 2));
        minute = std::stoi(input.substr(3, 2));
    } catch (...) {
        return false;
    }
    if (hour < 0 || hour >= 24) {
        return false;
    }
    if (minute < 0 || minute >= 60) {
        return false;
    }
    return true;
}

Time::Time(short hour, short minute) : hour(hour), minute(minute) {}

Time::Time(const Time &other) : hour(other.hour), minute(other.minute) {}

Time::Time(const std::string &time) { setTime(time); }

Time::~Time() {}

std::istream &operator>>(std::istream &is, Time &time) {
    std::string input;
    bool succeed = false;
    do {
        std::cout << "Enter Time (hh:mm): ";
        is >> input;
        succeed = time.setTime(input);
        if (!succeed) {
            std::cerr << "Invalid time format or value! Please try again."
                      << std::endl;
        }
    } while (!succeed);
    return is;
}
std::ostream &operator<<(std::ostream &os, const Time &time) {
    os << std::setw(2) << std::setfill('0') << time.hour << ":" << std::setw(2)
       << std::setfill('0') << time.minute;
    return os;
}
// bool isEscapePressed() {
//     if (_kbhit()) {          // Kiểm tra nếu có phím nào được nhấn
//         char ch = _getch();  // Lấy mã phím đã nhấn
//         if (ch == 27) {      // 27 là mã ASCII cho phím ESC
//             return true;
//         }
//     }
//     return false;
// }
bool Date::operator>(const Date &other) const {
    if (year != other.year) {
        return year > other.year;  // So sánh năm
    }
    if (month != other.month) {
        return month > other.month;  // So sánh tháng
    }
    return day > other.day;  // So sánh ngày
}
bool Date::operator<(const Date &other) const {
    if (year!= other.year) {
        return year < other.year;  // So sánh năm
    }
    if (month!= other.month) {
        return month < other.month;  // So sánh tháng
    }
    return day < other.day;  // So sánh ngày
}

bool ensureDirectoryExists(const std::string &filepath) {
    // Extract the directory path from the provided file path
    std::filesystem::path dirPath =
        std::filesystem::path(filepath).parent_path();

    // Check if the directory exists
    if (!std::filesystem::exists(dirPath)) {
        // Try to create the directory, and ensure it must be created
        std::error_code errCode;
        if (!std::filesystem::create_directories(dirPath, errCode)) {
            std::cerr << "Error creating directory: " << dirPath << " - "
                      << errCode.message() << std::endl;
            return false;  // Directory creation failed
        }
    }
    return true;  // Directory exists or was successfully created
}
