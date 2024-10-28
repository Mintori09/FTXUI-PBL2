#include "../include/ShowTimes.h"

#include "../include/ShowTime.h"


ShowTimes::ShowTimes(const std::string &filename) { loadFromFile(filename); }

ShowTimes::~ShowTimes() {}

void ShowTimes::display(int showTimeId) const {
    for (const ShowTime &showtime : showTimes) {
        if (showtime.getShowTimeId() == showTimeId) {
            showtime.display();
            return;
        }
    }
    std::cout << "ShowTime with Id " << showTimeId << " not found.\n";
}

void ShowTimes::displayAll() const {
    if (showTimes.empty()) {
        std::cout << "No showtimes available.\n";
        return;
    }

    for (const auto &showtime : showTimes) {
        showtime.display();
    }
}

bool ShowTimes::addShowTime(const ShowTime &newShowTime) {
    for (const auto &showtime : showTimes) {
        if (showtime.getShowTimeId() == newShowTime.getShowTimeId()) {
            std::cout << "ShowTime with Id " << newShowTime.getShowTimeId()
                      << " already exists.\n";
            return false;
        }
    }

    showTimes.push_back(newShowTime);
    return true;
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

bool ShowTimes::updateShowTime(int showTimeId) {
    for (auto &showtime : showTimes) {
        if (showtime.getShowTimeId() == showTimeId) {
            showtime.updateDetails();
            return true;
        }
    }
    std::cout << "ShowTime with Id " << showTimeId << " not found.\n";
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
                << showtime.getEndTime()
                << '\n';
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

void ShowTimes::display() const {
    for (const ShowTime &showtime : showTimes) {
        showtime.display();
    }
}

