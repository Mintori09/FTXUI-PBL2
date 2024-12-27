#include "Menu/menu.h"
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
                   book_ticket_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_movies_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_showtimes_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_booked_tickets_button->Render() |
                       size(WIDTH, EQUAL, 50) | center,
                   update_account_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   logout_button->Render() | size(WIDTH, EQUAL, 50) | center,
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
    auto show_movies_button = Button("Movie", [&] { addMovie(movies); });
    auto show_showtimes_button =
        Button("Add ShowTime", [&] { addShowTime(movies, showtimes); });
    auto show_booked_tickets_button = Button("Display Tickets", [&] {
        showTicket(accounts.getAccounts(), tickets.getTickets(),
                   movies.getMovies(), showtimes.getShowTimes());
    });
    // auto update_account_button = Button("Update Account", [&] {});
    auto statistic_button = Button("statistics", [&] {
        Statistic(tickets.getTickets(), movies.getMovies(),
                  showtimes.getShowTimes());
    });
    auto logout_button = Button("Log out", [&] { screen.Exit(); });

    // Container chứa các nút
    auto container = Container::Vertical({
        book_ticket_button,
        show_movies_button,
        show_showtimes_button,
        show_booked_tickets_button,
        // update_account_button,
        statistic_button,
        logout_button,
    });

    // Renderer cho giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   text("Admin Menu") | bold | center,
                   separator(),
                   book_ticket_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_movies_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_showtimes_button->Render() | size(WIDTH, EQUAL, 50) |
                       center,
                   show_booked_tickets_button->Render() |
                       size(WIDTH, EQUAL, 50) | center,
                   //    update_account_button->Render() | center |
                   //    size(WIDTH, EQUAL, 50),
                   statistic_button->Render() | size(WIDTH, EQUAL, 50) | center,
                   logout_button->Render() | size(WIDTH, EQUAL, 50) | center,
               }) |
               border | size(WIDTH, EQUAL, 100) | center;
    });

    screen.Loop(renderer);
}
int main() {

    while (true) {
        Movies movies(MOVIE_FILE);
        ShowTimes showtimes(SHOWTIME_FILE);
        Accounts accounts(ACCOUNT_FILE);
        Tickets tickets(TICKET_FILE);
        Account account = showMenu(accounts);
        if (account.getRole() == 1) {
            AdminMenu(movies, showtimes, accounts, tickets, account);
        } else if (account.getRole() == 2) {
            UserMenu(movies, showtimes, accounts, tickets, account);
        }
    }

    return 0;
}
