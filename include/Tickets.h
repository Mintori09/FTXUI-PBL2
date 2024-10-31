#ifndef _TICKETS_H
#define _TICKETS_H

#include <string>

#include "Account.h"  // Assuming you have an Account class
#include "ShowTime.h"
#include "Ticket.h"
#include "Vector.h"
#include "my_lib.h"


class Tickets {
   private:
    Vector<Ticket> tickets;

    int findTicketByUserId(const int userId) const;
    int findTicketByTicketId(
        const int ticketId) const;  // Cập nhật kiểu ticketId

   public:
    Vector<Ticket> getTickets() const { return tickets; }

    bool addTicketForUser(const Account &account, const ShowTime &showTime);
    bool addTicketForAdmin(const Ticket &newTicket);

    void deleteTicket(int ticketId);  // Cập nhật kiểu ticketId
    void displayHistoryTicket(int userId) const;

    void updateTicket(const Ticket &updatedTicket);
    void findTicket(int ticketId) const;  // Cập nhật kiểu ticketId

    int revenueMonth(short month, short year) const;
    int revenueQuarter(short year, short quarter) const;
    int revenueYear(short year) const;

    const void display() const;

    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
    void addTicket(const Ticket &ticket);
    int endOfTicketId() const;

        Tickets(const std::string &filename);
    Tickets() {}
    ~Tickets();

};

#endif
