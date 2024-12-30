#include "UserMenu.h"

void UserMenu::run(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
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
    ShowTicketInfoForAccount(account, tickets.getTickets(), movies.getMovies(),
                             showtimes.getShowTimes());
  });
  auto update_account_button = Button(
      "Change information Account", [&] { UpdateAccount(accounts, account); });
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
  auto renderer = ftxui::Renderer(container, [&] {
    return vbox({
               text("User Menu") | bold | center,
               separator(),
               book_ticket_button->Render() | size(WIDTH, EQUAL, 50) | center,
               show_movies_button->Render() | size(WIDTH, EQUAL, 50) | center,
               show_showtimes_button->Render() | size(WIDTH, EQUAL, 50) |
                   center,
               show_booked_tickets_button->Render() | size(WIDTH, EQUAL, 50) |
                   center,
               update_account_button->Render() | size(WIDTH, EQUAL, 50) |
                   center,
               logout_button->Render() | size(WIDTH, EQUAL, 50) | center,
           }) |
           border | size(WIDTH, EQUAL, 100) | center;
  });

  screen.Loop(renderer);
}
