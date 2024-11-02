#include "../include/Tickets.h"

#include <fstream>
#include <iostream>
#include <string>

int Tickets::findTicketByUserId(int userId) const {
    for (size_t i = 0; i < tickets.size(); ++i) {
        if (tickets.at(i).getUserId() == userId) {
            return i;
        }
    }
    return -1;
}

int Tickets::findTicketByTicketId(
    int ticketId) const {  // Cập nhật kiểu ticketId
    for (size_t i = 0; i < tickets.size(); ++i) {
        if (tickets.at(i).getTicketId() ==
            ticketId) {  // Cập nhật kiểu ticketId
            return i;
        }
    }
    return -1;
}

int seatNumber() {
    int seatNumber;  // Có thể cần nhập giá trị từ người dùng
    return seatNumber;
}

bool Tickets::addTicket(const Account &account,
                               const ShowTime &showTime, int seatNumber) {
    int ticketId;  // ticketId là kiểu int
    if (!tickets.empty()) {
        ticketId = tickets.back().getTicketId() + 1;  // Cập nhật cho ticketId
    } else {
        ticketId = 10000;
    }

    int showTimeId = showTime.getShowTimeId();
    int userId = account.getUserId();
    int seatNumberValue = seatNumber;  // Gọi hàm seatNumber
    int ticketPrice = 65000;             // Giá vé

    Date bookingDate(currentDate());
    std::string paymentMethod = "Cash";  // Phương thức thanh toán (có thể yêu cầu nhập)

    // Đảm bảo rằng kiểu dữ liệu và số lượng tham số khớp với hàm khởi tạo của
    // lớp Ticket
    Ticket newTicket(ticketId, showTimeId, userId, seatNumberValue, ticketPrice,
                     bookingDate, paymentMethod);
    addTicket(newTicket);
    return true;
}

bool Tickets::addTicketForAdmin(const Ticket &newTicket) {
    tickets.push_back(newTicket);
    return true;
}

void Tickets::deleteTicket(int ticketId) {       // Cập nhật kiểu ticketId
    int index = findTicketByTicketId(ticketId);  // Cập nhật kiểu ticketId
    if (index != -1) {
        tickets.erase(tickets.begin() + index);
    } else {
        std::cout << "Ticket not found.\n";
    }
}

void Tickets::displayHistoryTicket(int userId) const {
    for (const auto &ticket : tickets) {
        if (ticket.getUserId() == userId) {
            ticket.display();
        }
    }
}

void Tickets::updateTicket(const Ticket &updatedTicket) {
    int index = findTicketByTicketId(updatedTicket.getTicketId());
    if (index != -1) {
        tickets[index] = updatedTicket;
    } else {
        std::cout << "Ticket not found.\n";
    }
}

void Tickets::findTicket(int ticketId) const {  // Cập nhật kiểu ticketId
    for (size_t index = 0; index < tickets.size(); ++index) {
        if (tickets.at(index).getTicketId() ==
            ticketId) {  // Cập nhật kiểu ticketId
            tickets.at(index).display();
            return;
        }
    }
    std::cout << "Ticket not found." << std::endl;
}

int Tickets::revenueMonth(short month, short year) const {
    int sum = 0;
    for (const auto &ticket : tickets) {
        Date date = ticket.getBookingDate();
        if (date.getYear() == year && date.getMonth() == month) {
            sum += ticket.getTicketPrice();
        }
    }
    return sum;
}

int Tickets::revenueQuarter(short year, short quarter) const {
    int sum = 0;
    for (const auto &ticket : tickets) {
        Date date = ticket.getBookingDate();
        if (date.getYear() == year) {
            bool inQuarter = false;
            switch (quarter) {
                case 1:
                    inQuarter = (date.getMonth() >= 1 && date.getMonth() <= 3);
                    break;
                case 2:
                    inQuarter = (date.getMonth() >= 4 && date.getMonth() <= 6);
                    break;
                case 3:
                    inQuarter = (date.getMonth() >= 7 && date.getMonth() <= 9);
                    break;
                case 4:
                    inQuarter =
                        (date.getMonth() >= 10 && date.getMonth() <= 12);
                    break;
                default:
                    inQuarter = false;
                    break;
            }
            if (inQuarter) {
                sum += ticket.getTicketPrice();
            }
        }
    }
    return sum;
}

int Tickets::revenueYear(short year) const {
    int sum = 0;
    for (const auto &ticket : tickets) {
        if (ticket.getBookingDate().getYear() == year) {
            sum += ticket.getTicketPrice();
        }
    }
    return sum;
}

bool Tickets::saveToFile(const std::string &filename) const {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cout << "Failed to open file for saving.\n";
        return false;
    }

    for (const auto &ticket : tickets) {
        outFile << ticket.getTicketId() << ';' << ticket.getShowTimeId() << ';'
                << ticket.getUserId() << ';' << ticket.getSeatNumber() << ';'
                << ticket.getTicketPrice() << ';' << ticket.getBookingDate()
                << ';' << ticket.getPaymentMethod() << std::endl;
    }

    outFile.close();
    return true;
}

bool Tickets::loadFromFile(const std::string &filename) {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for writing: " << filename
                  << std::endl;
        std::ofstream createFile(filename);
        if (!createFile) {
            std::cerr << "Create File failed!" << std::endl;
            return false;
        } else {
            std::cerr << "Create File successfully!" << std::endl;
        }
    }

    if (inFile.peek() ==
        std::ifstream::traits_type::eof()) {  // Kiểm tra file rỗng
        std::cerr << "File is empty!";
        return false;
    }

    std::string line;
    tickets.clear();
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        Ticket newTicket;
        newTicket.readFromFile(ss);
        tickets.push_back(newTicket);
    }
    inFile.close();
    return true;
}



Tickets::Tickets(const std::string &filename) { loadFromFile(filename); }

Tickets::~Tickets() {}

const void Tickets::display() const {
    if (tickets.empty()) {
        std::cout << "Sorry, no tickets to display." << std::endl;
    } else {
        for (const auto &ticket : tickets) {
            ticket.display();
        }
    }
}

void Tickets::addTicket(const Ticket &ticket) {
    tickets.push_back(ticket);
    ticket.saveToFile(TICKET_FILE);
}
int Tickets::endOfTicketId() const {
    return tickets.end()->getTicketId();
}