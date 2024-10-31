#include "../include/ShowTimes.h"

#include "../include/ShowTime.h"

int ShowTimes::showTimeId() {
    return (showTimes.empty()) ? 1 : (showTimes.back().getShowTimeId() + 1);
}

ShowTimes::ShowTimes(const std::string &filename) { loadFromFile(filename); }

ShowTimes::~ShowTimes() {}

// void ShowTimes::display(int showTimeId) const {
//     for (const ShowTime &showtime : showTimes) {
//         if (showtime.getShowTimeId() == showTimeId) {
//             showtime.display();
//             return;
//         }
//     }
//     std::cout << "ShowTime with Id " << showTimeId << " not found.\n";
// }

bool ShowTimes::addShowTime(ShowTime &newShowTime) {
    if (checkValidShowTime(newShowTime)) {
        newShowTime.setShowTimeId(showTimeId());
        showTimes.push_back(newShowTime);
        newShowTime.saveToFile(SHOWTIME_FILE);
        return true;
    } else
        return false;
}

bool ShowTimes::checkValidShowTime(const ShowTime &showTime) const {
    // Kiểm tra xem lịch chiếu có nằm trong quá khứ không
    if (showTime.getShowDate() < currentDate()) {
        return false;
    }

    // Kiểm tra trùng lặp thời gian chiếu
    for (const auto &existingShowTime : showTimes) {
        if (existingShowTime.getShowDate() == showTime.getShowDate()) {
            // Kiểm tra xem khoảng thời gian có bị trùng lặp không
            if ((showTime.getStartTime() < existingShowTime.getEndTime()) &&
                (showTime.getEndTime() > existingShowTime.getStartTime())) {
                return false;
            }
        }
    }

    return true;  // Không có xung đột nào, thời gian hợp lệ
}

bool ShowTimes::deleteShowTime(int showTimeId) {
    for (auto it = showTimes.begin(); it != showTimes.end(); ++it) {
        if (it->getShowTimeId() == showTimeId) {
            showTimes.erase(it);
            return true;
        }
    }
    return false;
}

bool ShowTimes::saveToFile(const std::string &filename) const {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "Failed to open file for saving.\n";
        return false;
    }

    for (const auto &showtime : showTimes) {
        outFile << showtime.getShowTimeId() << ';' << showtime.getMovieId()
                << ';' << showtime.getHallId() << ';' << showtime.getShowDate()
                << ';' << showtime.getStartTime() << ';'
                << showtime.getEndTime() << '\n';
    }

    outFile.close();
    return true;
}

bool ShowTimes::loadFromFile(const std::string &filename) {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file: " << filename << std::endl;

        std::ofstream createFile(filename);
        if (!createFile) {
            std::cerr << "Failed to create file!" << std::endl;
            return false;
        } else {
            std::cerr << "File created successfully!" << std::endl;
        }
        return false;
    }

    if (isEmpty(inFile)) {
        std::cerr << "File is empty!" << std::endl;
        return false;
    }

    showTimes.clear();

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string item;

        ShowTime newShowTime;
        newShowTime.readFromFile(ss);
        showTimes.push_back(newShowTime);
    }

    inFile.close();
    return true;
}

// void ShowTimes::display() const {
//     for (const ShowTime &showtime : showTimes) {
//         showtime.display();
//     }
// }
