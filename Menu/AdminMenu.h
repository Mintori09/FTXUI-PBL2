#ifndef _ADMINMENU_H_
#define _ADMINMENU_H_
#include "menu.h"
#include "../include/Map.h"

using namespace ftxui;

class AdminMenu : public menu {
public:
    AdminMenu() {
    }

    ~AdminMenu() {
    }

    ftxui::Element displayMonthStatistics(const Vector<Ticket> &tickets);

    bool checkDateByQuarter(const Ticket &ticket, int quarter);

    ftxui::Element displayQuarterStatistics(const Vector<Ticket> &tickets,
                                            const Vector<Movie> &movies,
                                            const Vector<ShowTime> &showtimes);

    ftxui::Element displayYearStatistics(const Vector<Ticket> &tickets,
                                         const Vector<Movie> &movies,
                                         const Vector<ShowTime> &showtimes);

    ftxui::Element DisplayStatistics(const Vector<Ticket> &tickets,
                                     const Vector<Movie> &movies,
                                     const Vector<ShowTime> &showtimes,
                                     int Check);

    ftxui::Element displayMonthStatisticsForMovie(const Vector<Ticket> &tickets, const Vector<Movie> &movies, const Vector<ShowTime> &showtimes);


    void Statistic(const Vector<Ticket> &tickets, const Vector<Movie> &movies,
                   const Vector<ShowTime> &showtimes);

    void run(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
             Tickets &tickets, Account &account);
};

#include "AdminMenu.tpp"
#endif
