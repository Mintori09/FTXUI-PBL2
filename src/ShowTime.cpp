#include "../include/ShowTime.h"

#include <iostream>

#include "../include/my_lib.h"

ShowTime::ShowTime()
    : showTimeId(0),
      movieId(0),
      hallId(0),
      showDate(),
      startTime(),
      endTime() {}

ShowTime::ShowTime(int showTimeId, int movieId, int hallId,
                   const Date &showDate, const Time &startTime,
                   const Time &endTime)
    : showTimeId(showTimeId),
      movieId(movieId),
      hallId(hallId),
      showDate(showDate),
      startTime(startTime),
      endTime(endTime) {}
ShowTime::ShowTime(int movieId, int hallId, const Date &showDate,
                   const Time &startTime, const Time &endTime)
    : movieId(movieId),
      hallId(hallId),
      showDate(showDate),
      startTime(startTime),
      endTime(endTime) {}

ShowTime::ShowTime(const ShowTime &other)
    : showTimeId(other.showTimeId),
      movieId(other.movieId),
      hallId(other.hallId),
      showDate(other.showDate),
      startTime(other.startTime),
      endTime(other.endTime) {}

ShowTime::~ShowTime() {}

void ShowTime::setShowTimeId(int showTimeId) { this->showTimeId = showTimeId; }
void ShowTime::setMovieId(int movieId) { this->movieId = movieId; }
void ShowTime::setHallId(int hallId) { this->hallId = hallId; }
void ShowTime::setShowDate(const Date &showDate) { this->showDate = showDate; }
void ShowTime::setStartTime(const Time &startTime) {
    this->startTime = startTime;
}
void ShowTime::setEndTime(const Time &endTime) { this->endTime = endTime; }

int ShowTime::getShowTimeId() const { return showTimeId; }
int ShowTime::getMovieId() const { return movieId; }
int ShowTime::getHallId() const { return hallId; }
Date ShowTime::getShowDate() const { return showDate; }
Time ShowTime::getStartTime() const { return startTime; }
Time ShowTime::getEndTime() const { return endTime; }

// void ShowTime::updateDetails() {
//     displayCol();
//     int select;
//     do {
//         select = inputInteger("Enter your selection (1-7): ");
//         if (select >= 1 && select <= 7) {
//             break;
//         }
//         std::cout << "Invalid option: Please enter a number between 1 and 7!"
//                   << std::endl;
//     } while (true);

//     switch (select) {
//         case 1:
//             this->showTimeId = inputInteger("Enter new show time Id: ");
//             break;
//         case 2:
//             this->movieId = inputInteger("Enter new movie Id: ");
//             break;
//         case 3:
//             this->hallId = inputInteger("Enter new hall Id: ");
//             break;
//         case 4:
//             std::cout << "Enter new show date: ";
//             std::cin >> this->showDate;
//             break;
//         case 5:
//             std::cout << "Enter new start time: ";
//             std::cin >> this->startTime;
//             break;
//         case 6:
//             std::cout << "Enter new end time: ";
//             std::cin >> this->endTime;
//             break;
//         default:
//             std::cout << "Invalid choice!" << std::endl;
//             return;
//     }
// }

void ShowTime::display() const {
    std::cout << showTimeId << "\t" << movieId << "\t" << hallId << "\t"
              << showDate << "\t" << startTime << "\t" << endTime << "\t"
              << std::endl;
}

void ShowTime::displayCol() const {
    int count = 1;
    std::cout << count++ << ". Show Time Id: " << showTimeId << std::endl;
    std::cout << count++ << ". Movie Id: " << movieId << std::endl;
    std::cout << count++ << ". Hall Id: " << hallId << std::endl;
    std::cout << count++ << ". Show Date: " << showDate << std::endl;
    std::cout << count++ << ". Start Time: " << startTime << std::endl;
    std::cout << count++ << ". End Time: " << endTime << std::endl;
    std::cout << count++ << ". Booking Date: " << std::endl;
}

void ShowTime::readFromFile(std::stringstream &ss) {
    std::string item;
    std::getline(ss, item, ';');
    showTimeId = std::stoi(item);

    std::getline(ss, item, ';');
    movieId = std::stoi(item);

    std::getline(ss, item, ';');
    hallId = std::stoi(item);

    std::getline(ss, item, ';');
    Date showDate(item);
    this->showDate = showDate;

    std::getline(ss, item, ';');
    Time startTime(item);
    this->startTime = startTime;

    std::getline(ss, item);
    Time endTime(item);
    this->endTime = endTime;
}

bool ShowTime::saveToFile(const std::string &filename) const {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile) {
        std::cout << "Failed to open file for saving.\n";
    }

    outFile << showTimeId << ';' << movieId << ';' << hallId << ';' << showDate
            << ';' << startTime << ';' << endTime << ';' << '\n';

    outFile.close();
    return true;
}
