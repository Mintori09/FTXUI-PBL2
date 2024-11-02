#include <iostream>

#include "Menu/menu.h"
#include "include/Account.h"
#include "include/Movies.h"
#include "include/ShowTime.h"
#include "include/Vector.h"

void UserMenu(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
              Tickets &tickets, Account &account) {
    auto screen = ScreenInteractive::Fullscreen();

    // Tạo các nút cho từng tính năng
    auto book_ticket_button = Button("Booking Ticket", [&] {
        BookingTicket(movies.getMovies(), showtimes.getShowTimes(), account,
                      tickets);
    });
    auto show_movies_button = Button("Show Movie", [&] {
        displayMovie(movies.getMovies(), showtimes.getShowTimes());
    });
    auto show_showtimes_button = Button("Show Show Time", [&] {
        displayShowTimes(movies.getMovies(), showtimes.getShowTimes());
    });
    auto show_booked_tickets_button = Button("Show Ticket history", [&] {
        ShowTicketInfoForAccount(account, tickets.getTickets(),
                                 movies.getMovies(), showtimes.getShowTimes());
    });
    auto update_account_button = Button("Change information Account", [&] {
        UpdateAccount(accounts, account);
    });
    auto logout_button = Button("Log out", [&] { screen.Exit(); });

    // Container chứa các nút
    auto container = Container::Vertical({
        book_ticket_button,
        show_movies_button,
        show_showtimes_button,
        show_booked_tickets_button,
        update_account_button,
        logout_button,
    });

    // Renderer cho giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("User Menu") | bold | center,
                   separator(),
                   book_ticket_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_movies_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_showtimes_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_booked_tickets_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   update_account_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   logout_button->Render() | center | size(WIDTH, EQUAL, 50),
               }) |
               border | size(WIDTH, EQUAL, 100) | center;
    });

    screen.Loop(renderer);
}
void AdminMenu(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
               Tickets &tickets, Account &account) {
    auto screen = ScreenInteractive::Fullscreen();

    // Tạo các nút cho từng tính năng
    auto book_ticket_button = Button("Booking Ticket", [&] {
        BookingTicket(movies.getMovies(), showtimes.getShowTimes(), account,
                      tickets);
    });
    auto show_movies_button = Button("Movie", [&] {
        displayMovie(movies.getMovies(), showtimes.getShowTimes());
    });
    auto show_showtimes_button =
        Button("Add ShowTime", [&] { addShowTime(movies, showtimes); });
    auto show_booked_tickets_button = Button("Display Tickets", [&] {
        showTicket(accounts.getAccounts(), tickets.getTickets(),
                   movies.getMovies(), showtimes.getShowTimes());
    });
    // auto update_account_button = Button("Update Account", [&] {});
    // auto statistiic_button = Button("statistics", [&] {
    //     // Statistic(tickets);
    // });
    auto logout_button = Button("Log out", [&] { screen.Exit(); });

    // Container chứa các nút
    auto container = Container::Vertical({
        book_ticket_button,
        show_movies_button,
        show_showtimes_button,
        show_booked_tickets_button,
        // update_account_button,
        // statistiic_button,
        logout_button,
    });

    // Renderer cho giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("Admin Menu") | bold | center,
                   separator(),
                   book_ticket_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_movies_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_showtimes_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   show_booked_tickets_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   //    update_account_button->Render() | center |
                   //    size(WIDTH, EQUAL, 50),
                //    statistiic_button->Render() | center |
                //        size(WIDTH, EQUAL, 50),
                   logout_button->Render() | center | size(WIDTH, EQUAL, 50),
               }) |
               border | size(WIDTH, EQUAL, 100) | center;
    });

    screen.Loop(renderer);
}
int main() {
    Movies movies(MOVIE_FILE);
    ShowTimes showtimes(SHOWTIME_FILE);
    Accounts accounts(ACCOUNT_FILE);
    Tickets tickets(TICKET_FILE);
    while (true) {
        Account account = showMenu(accounts);

        if (account.getRole() == 1) {
            AdminMenu(movies, showtimes, accounts, tickets, account);
        } else if (account.getRole() == 2) {
            UserMenu(movies, showtimes, accounts, tickets, account);
        }
    }
    return 0;
}
