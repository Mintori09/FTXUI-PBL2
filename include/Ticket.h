#ifndef _TICKET_H
#define _TICKET_H

#include <sstream>
#include <string>

#include "my_lib.h"


class Ticket {
   private:
    int ticketId;  // Đổi ticketId từ std::string thành int
    int showtimeId;
    int userId;
    short seatNumber;
    int ticketPrice;
    Date bookingDate;
    std::string paymentMethod;

   public:
    void setTicketId(int ticketId);  // Cập nhật kiểu tham số
    void setShowTimeId(int showtimeId);
    void setUserId(int userId);
    void setHallId(short hallId);
    void setSeatNumber(short seatNumber);
    void setTicketPrice(int ticketPrice);
    void setBookingDate(const Date &bookingDate);
    void setPaymentMethod(const std::string &paymentMethod);

    int getTicketId() const;  // Cập nhật kiểu trả về
    int getShowTimeId() const;
    int getUserId() const;
    short getSeatNumber() const;
    short getHallId() const;
    int getTicketPrice() const;
    Date getBookingDate() const;
    std::string getPaymentMethod() const;

    void display() const;
    void displayCol() const;
    bool operator==(const Ticket &other) const;

    void readFromFile(std::stringstream &ss);
    bool saveToFile(const std::string &filename) const;

    Ticket();
    Ticket(int ticketId, int showTimeId, int userId,  // Cập nhật hàm khởi tạo
           short seatNumber, int ticketPrice, const Date &bookingDate,
           const std::string &paymentMethod);
    Ticket(const Ticket &source);
    ~Ticket();
};

#endif
