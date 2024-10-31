#include "Menu/menu.h"

void UserMenu() {
    auto screen = ScreenInteractive::Fullscreen();

    // Tạo các nút cho từng tính năng
    auto book_ticket_button = Button("Booking Ticket", [&] {});
    auto show_movies_button = Button("Show Movie", [&] {});
    auto show_showtimes_button = Button("Show Show Time", [&] {});
    auto show_booked_tickets_button = Button("Show Ticket history", [&] {});
    auto update_account_button = Button("Change information Account", [&] {});
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
void AdminMenu() {
    auto screen = ScreenInteractive::Fullscreen();

    // Tạo các nút cho từng tính năng
    auto book_ticket_button = Button("Booking Ticket", [&] {});
    auto show_movies_button = Button("Movie", [&] {});
    auto show_showtimes_button = Button("Add ShowTime", [&] {});
    auto show_booked_tickets_button = Button("Display Tickets", [&] {});
    auto update_account_button = Button("Update Account", [&] {});
    auto statistiic_button = Button("statistics", [&] {});
    auto logout_button = Button("Log out", [&] { screen.Exit(); });

    // Container chứa các nút
    auto container = Container::Vertical({
        book_ticket_button,
        show_movies_button,
        show_showtimes_button,
        show_booked_tickets_button,
        update_account_button,
        statistiic_button,
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
                   statistiic_button->Render() | center |
                       size(WIDTH, EQUAL, 50),
                   logout_button->Render() | center | size(WIDTH, EQUAL, 50),
               }) |
               border | size(WIDTH, EQUAL, 100) | center;
    });

    screen.Loop(renderer);
}
int main() {
    Movies moveis(MOVIE_FILE);
    ShowTimes showtimes(SHOWTIME_FILE);
    Accounts accounts(ACCOUNT_FILE);
    Tickets tickets(TICKET_FILE);
    while (true) {
        Account account = showMenu(accounts);
        if (account.getRole() == 1) {
            AdminMenu();
        } else if (account.getRole() == 2) {
            UserMenu();
        }
    }
    return 0;
}