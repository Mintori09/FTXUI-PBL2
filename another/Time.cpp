#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>


void writeTimeToFile(const std::string &filename,
                     std::chrono::system_clock::time_point access_time) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Can't read file." << std::endl;
        exit(1);
    }

    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    ofs << "Now time: "
        << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S")
        << std::endl;

    auto access_time_t = std::chrono::system_clock::to_time_t(access_time);
    ofs << "Time need to access: "
        << std::put_time(std::localtime(&access_time_t), "%Y-%m-%d %H:%M:%S")
        << std::endl;
    ofs.close();
}

std::chrono::system_clock::time_point readTimeFromFile(
    const std::string &filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "can't read file." << std::endl;
        exit(1);
    }

    std::string line;
    std::getline(ifs, line);
    std::getline(ifs, line);
    ifs.close();

    std::istringstream ss(line);
    std::string date_time_str;
    std::getline(ss, date_time_str, ':');
    std::getline(ss, date_time_str);

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
            std::cout << "Wait : " << seconds_left << " sec" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

// int main()
// {
//     std::string filename = "time.txt";

//     int wait_seconds;
//     std::cout << "Enter time need to start program: ";
//     std::cin >> wait_seconds;

//     auto now = std::chrono::system_clock::now();
//     auto access_time = std::chrono::system_clock::now() + std::chrono::seconds(wait_seconds);

//     writeTimeToFile(filename, access_time);

//     checkAndPrintRemainingTime(filename);

//     return 0;
// }
