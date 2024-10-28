#include "../include/Ticket.h"

#include <iostream>

Ticket::Ticket() {}

Ticket::Ticket(int ticketId, int showtimeId, int userId, short seatNumber,
               int ticketPrice, const Date &bookingDate,
               const std::string &paymentMethod)
    : ticketId(ticketId),
      showtimeId(showtimeId),
      userId(userId),
      seatNumber(seatNumber),
      ticketPrice(ticketPrice),
      bookingDate(bookingDate),
      paymentMethod(paymentMethod) {}

Ticket::Ticket(const Ticket &source)
    : Ticket(source.ticketId, source.showtimeId, source.userId,
             source.seatNumber, source.ticketPrice, source.bookingDate,
             source.paymentMethod) {}

Ticket::~Ticket() {}

void Ticket::setTicketId(int ticketId) {  // Cập nhật kiểu tham số
    this->ticketId = ticketId;
}

void Ticket::setShowTimeId(int showtimeId) { this->showtimeId = showtimeId; }

void Ticket::setUserId(int userId) { this->userId = userId; }

void Ticket::setTicketPrice(int ticketPrice) {
    this->ticketPrice = ticketPrice;
}

void Ticket::setBookingDate(const Date &bookingDate) {
    this->bookingDate = bookingDate;
}

void Ticket::setPaymentMethod(const std::string &paymentMethod) {
    this->paymentMethod = paymentMethod;
}

int Ticket::getTicketId() const { return ticketId; }  // Cập nhật kiểu trả về

int Ticket::getShowTimeId() const { return showtimeId; }

int Ticket::getUserId() const { return userId; }

short Ticket::getSeatNumber() const { return seatNumber; }

int Ticket::getTicketPrice() const { return ticketPrice; }

Date Ticket::getBookingDate() const { return bookingDate; }

std::string Ticket::getPaymentMethod() const { return paymentMethod; }

void Ticket::display() const {
    std::cout << ticketId << "\t" << showtimeId << "\t" << userId << "\t"
              << seatNumber << "\t" << ticketPrice << "\t" << bookingDate
              << "\t" << paymentMethod << std::endl;
}

void Ticket::displayCol() const {
    int count = 1;
    std::cout << count++ << ". Ticket Id: " << ticketId << std::endl;
    std::cout << count++ << ". Showtime Id: " << showtimeId << std::endl;
    std::cout << count++ << ". User Id: " << userId << std::endl;
    std::cout << count++ << ". Seat Number: " << seatNumber << std::endl;
    std::cout << count++ << ". Ticket Price: " << ticketPrice << " VND"
              << std::endl;
    std::cout << count++ << ". Booking Date: " << bookingDate << std::endl;
    std::cout << count++ << ". Payment Method: " << paymentMethod << std::endl;
    std::cout << count++ << std::endl;
}

bool Ticket::operator==(const Ticket &other) const {
    return (this->ticketId == other.ticketId &&
            this->showtimeId == other.showtimeId &&
            this->userId == other.userId &&
            this->seatNumber == other.seatNumber &&
            this->ticketPrice == other.ticketPrice &&
            this->bookingDate == other.bookingDate &&
            this->paymentMethod == other.paymentMethod);
}

void Ticket::readFromFile(std::stringstream &ss) {
    std::string item;

    std::getline(ss, item, ';');
    ticketId = std::stoi(item);  // Chuyển đổi từ chuỗi sang int
    std::getline(ss, item, ';');
    showtimeId = std::stoi(item);
    std::getline(ss, item, ';');
    userId = std::stoi(item);
    std::getline(ss, item, ';');
    seatNumber = std::stoi(item);
    std::getline(ss, item, ';');
    ticketPrice = std::stoi(item);
    std::getline(ss, item, ';');
    Date bookingDate(item);
    std::getline(ss, paymentMethod, ';');
}

bool Ticket::saveToFile(const std::string &filename) const {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile) {
        std::cout << "Failed to open file for saving.\n";
    }

    outFile << ticketId << ';' << showtimeId << ';' << userId << ';'
            << seatNumber << ';' << ticketPrice << ';' << bookingDate << ';'
            << paymentMethod << std::endl;

    outFile.close();
    return true;
}
