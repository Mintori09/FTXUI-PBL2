#ifndef _MENU_H_
#define _MENU_H_
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/util/ref.hpp>
#include <iostream>
#include <string>

#include "../include/Accounts.h"
#include "../include/Movies.h"
#include "../include/ShowTimes.h"
#include "../include/Tickets.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
Vector<int> getLast6Months() {
  int current_month = currentMonth();
  Vector<int> last6Months;

  for (int i = 0; i < 6; ++i) {
    last6Months.push_back(current_month); // Store the calculated month
    if (--current_month <= 0) {
      current_month += 12; // Wrap around if the month goes below 1
    }
  }

  return last6Months;
}
bool checkShowTimeExistInTicket(const Ticket &ticket,
                                const Vector<ShowTime> &showtimes) {
  for (const auto &st : showtimes) { // Use const reference
    if (st.getShowTimeId() == ticket.getShowTimeId()) {
      return true;
    }
  }
  return false;
}

ShowTime getShowTimeFromTicket(const Ticket &ticket,
                               const Vector<ShowTime> &showtimes) {
  for (const auto &st : showtimes) { // Use const reference
    if (st.getShowTimeId() == ticket.getShowTimeId()) {
      return st; // Return found showtime
    }
  }
  throw std::runtime_error(
      "Showtime not found for the given ticket."); // Handle not found case
}

void addMovie(Movies &movies) {
  system("cls");
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  std::string message;
  std::string title;
  std::string description;
  std::string genre;
  std::string director;
  std::string cast;
  std::string duration;
  std::string releaseDate;

  Component input_title = Input(&title, "title");
  Component input_description = Input(&description, "description");
  Component input_genre = Input(&genre, "genre");
  Component input_director = Input(&director, "director");
  Component input_cast = Input(&cast, "cast");
  Component input_duration = Input(&duration, "duration");
  Component input_releaseDate = Input(&releaseDate, "releaseDate");
  auto confirm_button = Button("confirm", [&] {
    if (title.empty() && description.empty() && genre.empty() &&
        director.empty() && cast.empty() && duration.empty() &&
        releaseDate.empty()) {
      message = "Please enter information!";
    } else {
      Date release;
      if (release.setDate(releaseDate)) {
        if (validInteger(duration)) {
          Movie newMovie(title, description, genre, director, cast,
                         std::stoi(duration), release);
          movies.addMovie(newMovie);
          message = "Movie added successfully!";
          title = description = genre = director = cast = duration =
              releaseDate = "";
          system("cls");
        }
      }
    }
  });

  auto exit_button = Button("Exit", [&] { screen.Exit(); });

  auto component = Container::Vertical(
      {input_title, input_description, input_genre, input_director, input_cast,
       input_duration, input_releaseDate, confirm_button, exit_button});

  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Adding new Movie") | center,
               separator(),
               hbox(text(" Title : "), input_title->Render()),
               hbox(text(" Description  : "), input_description->Render()),
               hbox(text(" Genre :"), input_genre->Render()),
               hbox(text(" Director :"), input_director->Render()),
               hbox(text(" Cast :"), input_cast->Render()),
               hbox(text(" Duration :"), input_duration->Render()),
               hbox(text(" releaseDate :"), input_releaseDate->Render()),
               separator(),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message) | center | hcenter | color(Color::Blue)),

           }) |
           border;
  });

  screen.Loop(renderer);
}

void addShowTime(const Movie &movie, ShowTimes &showtimes) {
  system("cls");
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  std::string message;

  int movieId = movie.getMovieId();
  std::string hallId;
  std::string showDate;
  std::string startTime;
  std::string duration;
  Date show;

  Component hall_id_input = Input(&hallId, "hall id");
  Component show_date_input = Input(&showDate, "dd-mm-yyyy");
  Component start_time_input = Input(&startTime, "hh:mm");

  auto confirm_button = Button("confirm", [&] {
    if (hallId.empty() || showDate.empty() || startTime.empty() ||
        !isValidDateFormat(showDate)) {
      message = "Please add information!";
    } else {
      if (validInteger(hallId)) {
        int hallID = std::stoi(hallId);
        if (hallID < 4 && hallID > 0) {
          Time start(startTime);
          Time endTime = start.add(movie.getDuration());
          ShowTime newShowTime(movieId, hallID, showDate, startTime, endTime);
          if (showtimes.addShowTime(newShowTime)) {
            message = "ShowTime added successfully!";
          } else {
            message = "ShowTime already exists!";
          }
        }
      }
    }
    hallId = showDate = startTime = "";
  });

  auto exit_button = Button("Exit", [&] { screen.Exit(); });

  auto component =
      Container::Vertical({hall_id_input, show_date_input, start_time_input,
                           confirm_button, exit_button});

  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Adding new ShowTime ...."),
               separator(),
               text("Title :" + movie.getTitle()),
               text("Duration :" + std::to_string(movie.getDuration())),
               hbox(text("Hall ID :"), hall_id_input->Render()),
               hbox(text("Show Date :"), show_date_input->Render()),
               hbox(text("Start Time :"), start_time_input->Render()),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message) | center | hcenter | color(Color::Blue)),

           }) |
           border;
  });

  screen.Loop(renderer);
}

void UpdateAccount(Accounts &accounts, Account account) {
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  std::string username = account.getUserName();
  std::string password;
  std::string phoneNumber;
  std::string newPassword;
  std::string message;

  InputOption password_option;
  password_option.password = true;
  Component input_password =
      Input(&newPassword, "new password", password_option);
  Component input_current_password =
      Input(&password, "confirm password", password_option);

  Component input_phone_number = Input(&phoneNumber, account.getPhoneNumber());
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  auto confirm_button = Button("confirm", [&] {
    if (password == account.getPassWord()) {
      if (newPassword != password) {
        account.setPassWord(newPassword);
      }
      if (phoneNumber != account.getPhoneNumber()) {
        account.setPhoneNumber(phoneNumber);
      }
    } else {
      message = "Failed";
    }
  });
  auto exit_button = Button("Exit", [&] { screen.Exit(); });

  auto component = Container::Vertical({input_password, input_phone_number,
                                        input_current_password, confirm_button,
                                        exit_button});

  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Hello " + username), separator(),
               hbox(text(" Password : "), input_password->Render()),
               hbox(text(" phone number  : "), input_phone_number->Render()),
               hbox(text(" Confirm password :"),
                    input_current_password->Render()),
               separator(),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message)),
               //    separator(),
               //    text(account.getPassWord()),
               //    text(account.getUserName()),
               //    text(account.getPhoneNumber()),
           }) |
           border;
  });

  screen.Loop(renderer);
  accounts.updateAccount(account);
}

std::string getMovieTitle(int movieId, const Vector<Movie> &movies) {
  for (const auto &movie : movies) {
    if (movie.getMovieId() == movieId) {
      return movie.getTitle();
    }
  }
  return "";
}

bool checkMovieId(int movieId, const Vector<Movie> &movies) {
  for (const auto &movie : movies) {
    if (movie.getMovieId() == movieId) {
      return true;
    }
  }
  return false;
}

Movie getMovie(int movieId, const Vector<Movie> &movies) {
  for (const auto &movie : movies) {
    if (movie.getMovieId() == movieId) {
      return movie;
    }
  }
  return Movie();
}

bool checkShowTimeId(int showTimeId, const Vector<ShowTime> &showTimes) {
  for (const auto &showtime : showTimes) {
    if (showtime.getShowTimeId() == showTimeId) {
      return true;
    }
  }
  return false;
}

Vector<ShowTime> getFutureShowTimes(const Vector<ShowTime> &showTimes,
                                    const Vector<Movie> &movies) {
  Date today(currentDate());
  Time timeNow(currentTime());
  Vector<ShowTime> futureShowTimes;

  for (const auto &showtime : showTimes) {
    Date showDate(showtime.getShowDate());

    if (showDate > today ||
        (showDate == today && showtime.getStartTime() > timeNow)) {
      for (const auto &movie : movies) {
        if (movie.getMovieId() == showtime.getMovieId()) {
          futureShowTimes.push_back(showtime);
          break;
        }
      }
    }
  }
  return futureShowTimes;
}
Vector<ShowTime> getFutureAndCurrentShowTimes(const Vector<ShowTime> &showTimes,
                                              const Vector<Movie> &movies) {
  Date today(currentDate());
  Time timeNow(currentTime());
  Vector<ShowTime> futureShowTimes;

  for (const auto &showtime : showTimes) {
    Date showDate(showtime.getShowDate());

    if (showDate > today ||
        (showDate == today && showtime.getEndTime() > timeNow)) {
      for (const auto &movie : movies) {
        if (movie.getMovieId() == showtime.getMovieId()) {
          futureShowTimes.push_back(showtime);
          break;
        }
      }
    }
  }
  return futureShowTimes;
}

Vector<Movie> getMoviesWithFutureShowTimes(const Vector<ShowTime> &showTimes,
                                           const Vector<Movie> &movies) {
  Vector<ShowTime> futureShowTimes = getFutureShowTimes(showTimes, movies);
  Vector<Movie> moviesWithFutureShowTimes;
  Vector<int> movieIdsAdded;

  for (const auto &showtime : futureShowTimes) {
    int movieId = showtime.getMovieId();

    if (std::find(movieIdsAdded.begin(), movieIdsAdded.end(), movieId) ==
        movieIdsAdded.end()) {
      for (const auto &movie : movies) {
        if (movie.getMovieId() == movieId) {
          moviesWithFutureShowTimes.push_back(movie);
          movieIdsAdded.push_back(movieId);
          break;
        }
      }
    }
  }
  return moviesWithFutureShowTimes;
}

ftxui::Element DisplayMovies(const Vector<Movie> &movies,
                             const std::string &search_query) {
  using namespace ftxui;
  Elements movie_rows;

  movie_rows.push_back(hbox(
      text("ID") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 5) | hcenter,
      separator(),
      text("Title") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 20) |
          hcenter,
      separator(),
      text("Genre") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 10) |
          hcenter,
      separator(),
      text("Description") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 30) |
          hcenter,
      separator(),
      text("Director") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 15) |
          hcenter,
      separator(),
      text("Duration (mins)") | bold | color(Color::Blue) |
          size(WIDTH, EQUAL, 15) | hcenter,
      separator(),
      text("Release Date") | bold | color(Color::Blue) |
          size(WIDTH, EQUAL, 12) | hcenter,
      separator(),
      text("Rating") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 10) |
          hcenter));

  for (const auto &movie : movies) {
    if (search_query.empty() ||
        isAllWordsInString(search_query, movie.getTitle())) {
      movie_rows.push_back(separator());
      movie_rows.push_back(hbox(
          text(std::to_string(movie.getMovieId())) | size(WIDTH, EQUAL, 5),
          separator(), paragraph(movie.getTitle()) | size(WIDTH, EQUAL, 20),
          separator(), paragraph(movie.getGenre()) | size(WIDTH, EQUAL, 10),
          separator(),
          paragraph(movie.getDescription()) | bold | size(WIDTH, EQUAL, 30) |
              hcenter,
          separator(), paragraph(movie.getDirector()) | size(WIDTH, EQUAL, 15),
          separator(),
          text(std::to_string(movie.getDuration())) | size(WIDTH, EQUAL, 15),
          separator(),
          text(movie.getReleaseDate().toString()) | size(WIDTH, EQUAL, 12),
          separator(),
          text(std::to_string(movie.getRating())) | size(WIDTH, EQUAL, 10)));
    }
  }

  return vbox(movie_rows) | border | center;
}

ftxui::Element DisplayShowTimes(const Vector<ShowTime> &showTimes,
                                const Vector<Movie> &movies,
                                const std::string &search_query) {
  std::cin.ignore();
  using namespace ftxui;
  Elements showtime_rows;

  showtime_rows.push_back(hbox(text("Showtime ID") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter,
                               separator(),
                               text("Movie Title") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 20) | hcenter,
                               separator(),
                               text("Show Date") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("Start Time") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("End Time") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("Hall ID") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter));

  for (const auto &showtime : showTimes) {
    std::string movieTitle = getMovieTitle(showtime.getMovieId(), movies);

    if (search_query.empty() || isAllWordsInString(search_query, movieTitle)) {
      showtime_rows.push_back(separator());
      showtime_rows.push_back(hbox(
          text(std::to_string(showtime.getShowTimeId())) |
              size(WIDTH, EQUAL, 10),
          separator(), paragraph(movieTitle) | size(WIDTH, EQUAL, 20),
          separator(),
          text(showtime.getShowDate().toString()) | size(WIDTH, EQUAL, 12),
          separator(),
          text(showtime.getStartTime().toString()) | size(WIDTH, EQUAL, 12),
          separator(),
          text(showtime.getEndTime().toString()) | size(WIDTH, EQUAL, 12),
          separator(),
          text(std::to_string(showtime.getHallId())) | size(WIDTH, EQUAL, 10)));
    }
  }

  return vbox(showtime_rows) | border | center;
}
int selectMovie(const Vector<Movie> &movie, const Vector<ShowTime> &showtime) {
  Vector<Movie> movies = getMoviesWithFutureShowTimes(showtime, movie);
  using namespace ftxui;
  std::string movieId;
  std::string search_query;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
  auto movie_input = Input(&movieId, "Enter Movie ID...");
  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;
  auto select_movie_button = Button("Enter", [&] {
    if (!movieId.empty() && validInteger(movieId)) {
      int id = std::stoi(movieId);
      if (checkMovieId(id, movies)) {
        system("cls");
        selected = true;
        screen.Exit();
      } else {
        search_message = "Movie not found!";
      }
    } else {
      search_message = "Invalid Movie ID!";
    }
  });

  auto container = Container::Vertical(Components{
      search_input,
      movie_input,
      select_movie_button,
      exit_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               hbox({
                   movie_input->Render() | border | size(WIDTH, EQUAL, 25) |
                       center,
                   select_movie_button->Render() | center,
                   text(search_message) | center | color(Color::Red),

               }),
               DisplayMovies(movies, search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }
  if (exit) {
    return -1;
  }

  return std::stoi(movieId);
}

int selectMovieForShowTime(const Vector<Movie> &movie,
                           const Vector<ShowTime> &showtime) {
  Vector<Movie> movies = getMoviesWithFutureShowTimes(showtime, movie);
  using namespace ftxui;
  std::string movieId;
  std::string search_query;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
  auto movie_input = Input(&movieId, "Enter Movie ID...");
  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;
  auto select_movie_button = Button("Enter", [&] {
    if (!movieId.empty() && validInteger(movieId)) {
      int id = std::stoi(movieId);
      if (checkMovieId(id, movies)) {
        system("cls");
        selected = true;
        screen.Exit();
      } else {
        search_message = "Movie not found!";
      }
    } else {
      search_message = "Invalid Movie ID!";
    }
  });

  auto container = Container::Vertical(Components{
      search_input,
      movie_input,
      select_movie_button,
      exit_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               hbox({
                   movie_input->Render() | border | size(WIDTH, EQUAL, 25) |
                       center,
                   select_movie_button->Render() | center,
                   text(search_message) | center | color(Color::Red),

               }),
               DisplayMovies(movies, search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }
  if (exit) {
    return -1;
  }

  return std::stoi(movieId);
}

int selectShowTime(Vector<Movie> &movies, Vector<ShowTime> &showtimes) {
  Vector<ShowTime> showTimes = getFutureShowTimes(showtimes, movies);
  using namespace ftxui;
  std::string search_query;
  std::string search_message;
  std::string showTimeId;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
  auto showtime_input = Input(&showTimeId, "Enter showtime id...");

  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;
  auto select_showtime_button = Button("Enter", [&] {
    if (!showTimeId.empty() && validInteger(showTimeId)) {
      int id = std::stoi(showTimeId);
      if (checkShowTimeId(id, showTimes)) {
        system("cls");
        selected = true;
        screen.Exit();
      } else {
        search_message = "Showtime not found!";
      }
    } else {
      search_message = "Invalid ShowTime ID!";
    }
  });

  auto container = Container::Vertical(Components{
      search_input,
      exit_button,
      showtime_input,
      select_showtime_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               hbox({showtime_input->Render() | size(WIDTH, EQUAL, 25) | center,
                     select_showtime_button->Render() | center,
                     text(search_message) | center | color(Color::Red)}),
               DisplayShowTimes(showTimes, movies, search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }
  if (exit) {
    return -1;
  }
  return std::stoi(showTimeId);
}
int selectShowTimeCurrentAndFuture(const Vector<Movie> &movies,
                                   const Vector<ShowTime> &showtimes) {
  Vector<ShowTime> showTimes = getFutureShowTimes(showtimes, movies);
  using namespace ftxui;
  std::string search_query;
  std::string search_message;
  std::string showTimeId;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
  auto showtime_input = Input(&showTimeId, "Enter showtime id...");

  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;
  auto select_showtime_button = Button("Enter", [&] {
    if (!showTimeId.empty() && validInteger(showTimeId)) {
      int id = std::stoi(showTimeId);
      if (checkShowTimeId(id, showTimes)) {
        system("cls");
        selected = true;
        screen.Exit();
      } else {
        search_message = "Showtime not found!";
      }
    } else {
      search_message = "Invalid ShowTime ID!";
    }
  });

  auto container = Container::Vertical(Components{
      search_input,
      exit_button,
      showtime_input,
      select_showtime_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               hbox({showtime_input->Render() | size(WIDTH, EQUAL, 25) | center,
                     select_showtime_button->Render() | center,
                     text(search_message) | center | color(Color::Red)}),
               DisplayShowTimes(showTimes, movies, search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }
  if (exit) {
    return -1;
  }
  return std::stoi(showTimeId);
}

std::string getUserName(const Ticket &ticket, const Vector<Account> accounts) {
  for (const auto &account : accounts) {
    if (account.getUserId() == ticket.getUserId()) {
      return account.getUserName();
    }
  }
  return "";
}
int getMovieId(const Ticket &ticket, const Vector<Movie> movies,
               const Vector<ShowTime> showtimes) {
  for (const auto &showtime : showtimes) {
    if (showtime.getShowTimeId() == ticket.getShowTimeId()) {
      int movieId = showtime.getMovieId();
      for (const auto &movie : movies) {
        if (movie.getMovieId() == movieId) {
          return movie.getMovieId();
        }
      }
    }
  }
  return -1;
}
ShowTime getShowTime(int showTimeId, const Vector<ShowTime> &showtimes) {
  for (const auto &showtime : showtimes) {
    if (showtime.getShowTimeId() == showTimeId) {
      return showtime;
    }
  }
  return ShowTime();
}
ftxui::Element DisplayTicketsAll(const Vector<Account> accounts,
                                 const Vector<Ticket> tickets,
                                 const ShowTime &showtime,
                                 const Vector<Movie> &movies,
                                 const std::string &search_query) {
  std::cin.ignore();
  using namespace ftxui;
  Elements showtime_rows;

  showtime_rows.push_back(hbox(text("Ticket Id") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 15) | hcenter,
                               separator(),
                               text("Username") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 20) | hcenter,
                               separator(),
                               text("Booking Date") | bold |
                                   color(Color::Blue) | size(WIDTH, EQUAL, 15) |
                                   hcenter,
                               separator(),
                               text("Show Date") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 15) | hcenter,
                               separator(),
                               text("Seat") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter));

  for (const auto &ticket : tickets) {
    std::string UserName = getUserName(ticket, accounts);

    if (search_query.empty() || isAllWordsInString(search_query, UserName)) {
      showtime_rows.push_back(separator());
      showtime_rows.push_back(hbox(
          text(std::to_string(ticket.getTicketId())) | size(WIDTH, EQUAL, 15),
          separator(), paragraph(UserName) | size(WIDTH, EQUAL, 20),
          separator(),
          text(ticket.getBookingDate().toString()) | size(WIDTH, EQUAL, 15),
          separator(),
          text(showtime.getShowDate().toString()) | size(WIDTH, EQUAL, 15),
          separator(),
          text(std::to_string(ticket.getSeatNumber())) |
              size(WIDTH, EQUAL, 10)));
    }
  }

  return vbox(showtime_rows) | border | center;
}
ftxui::Element DisplayTicketsAll(const Vector<Ticket> tickets,
                                 const Vector<ShowTime> &showtimes,
                                 const Vector<Movie> &movies,
                                 const Account account) {
  using namespace ftxui;
  Elements showtime_rows;

  // Header row
  showtime_rows.push_back(hbox(text("Ticket Id") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 15) | hcenter,
                               separator(),
                               text("Title") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 20) | hcenter,
                               separator(),
                               text("Booking Date") | bold |
                                   color(Color::Blue) | size(WIDTH, EQUAL, 15) |
                                   hcenter,
                               separator(),
                               text("Show Date") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 15) | hcenter,
                               separator(),
                               text("Seat") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter));

  for (const auto &ticket : tickets) {
    if (account.getUserId() == ticket.getUserId()) {
      // Check if the showtime exists for the ticket
      ShowTime showtime = getShowTimeFromTicket(ticket, showtimes);
      Movie movie = getMovie(showtime.getMovieId(), movies);
      showtime_rows.push_back(separator());
      showtime_rows.push_back(hbox(
          text(std::to_string(ticket.getTicketId())) | size(WIDTH, EQUAL, 15),
          separator(), paragraph(movie.getTitle()) | size(WIDTH, EQUAL, 20),
          separator(),
          text(ticket.getBookingDate().toString()) | size(WIDTH, EQUAL, 15),
          separator(),
          text(showtime.getShowDate().toString()) | size(WIDTH, EQUAL, 15),
          separator(),
          text(std::to_string(ticket.getSeatNumber())) |
              size(WIDTH, EQUAL, 10)));
    }
  }

  return vbox(showtime_rows) | border |
         center; // Move return outside of the loop
}

void ShowTicketInfo(const Vector<Account> accounts,
                    const Vector<Ticket> &tickets, const Vector<Movie> &movies,
                    const ShowTime &showtime) {
  using namespace ftxui;
  std::string search_query;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search username for...");
  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;

  auto container = Container::Vertical(Components{
      search_input,
      exit_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,

               DisplayTicketsAll(accounts, tickets, showtime, movies,
                                 search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }

  return;
}
void ShowTicketInfoForAccount(const Account account,
                              const Vector<Ticket> &tickets,
                              const Vector<Movie> &movies,
                              const Vector<ShowTime> &showtimes) {
  using namespace ftxui;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto exit_button = Button("Exit", [&] {
    system("cls");
    screen.Exit();
  });

  auto container = Container::Vertical(Components{
      exit_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               text("History tickets") | center,
               separator(),

               DisplayTicketsAll(tickets, showtimes, movies, account),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  screen.Loop(renderer);

  return;
}
void showTicket(const Vector<Account> accounts, const Vector<Ticket> &tickets,
                const Vector<Movie> &movies,
                const Vector<ShowTime> &showtimes) {
  while (true) {
    int select = selectShowTimeCurrentAndFuture(movies, showtimes);
    if (select == -1) {
      return;
    }
    ShowTicketInfo(accounts, tickets, movies, getShowTime(select, showtimes));
  }
}

using namespace ftxui;
void clear() { system("cls"); }

bool login(Accounts &accounts, Account &account, std::string &login_message) {
  std::string username = account.getUserName();
  std::string password = account.getPassWord();
  if (accounts.authenticate(username, password, account)) {
    clear();
    login_message = "Login successful!";
    username.clear();
    password.clear();
    return true;
  } else {
    login_message = "Login failed. Please try again.";
    return false;
  }
}

void registerAccount(Accounts &accounts, Account &account,
                     std::string &login_message) {
  std::string username = account.getUserName(),
              password = account.getPassWord(),
              phoneNumber = account.getPhoneNumber();
  if (username.empty() || password.empty() || phoneNumber.empty()) {
    login_message = "username/ password/ phonenumber cannot be empty";
  } else if (!isValidStringNoSymbol(username)) {
    login_message = "Username is not Valid.";
  } else if (!isValidString(password)) {
    login_message = "password is not Valid.";
  } else if (!isValidString(password)) {
    login_message = "phonenumber is not Valid.";
  } else if (accounts.isUsernameTaken(username)) {
    login_message =
        "This username is already in use. Please choose a different one.";
  } else {
    system("cls");
    accounts.addAccount(account);
    login_message = "Account created successfully! You can now log in.";
    username.clear();
    password.clear();
  }
}

void showLoginMenu(Accounts &accounts, Account &account,
                   std::string &login_message, bool &switch_to_register,
                   ScreenInteractive &screen, bool &mainMenu) {
  std::string username, password;
  auto username_input = Input(&username, "Username");
  auto password_input = Input(&password, "Password");

  auto login_button = Button("Login", [&] {
    account.setPassWord(password);
    account.setUserName(username);
    if (login(accounts, account, login_message)) {
      mainMenu = false;
      screen.Exit();
    }
  });

  auto switch_button = Button("Switch to Register", [&] {
    switch_to_register = true;
    screen.Exit();
  });
  auto exit_button = Button("Exit", [&] { exit(1); });

  auto container = Container::Vertical(Components{
      username_input,
      password_input,
      login_button,
      switch_button,
      exit_button,
      Renderer([&] { return text(login_message) | bold | color(Color::Red); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               username_input->Render() | borderStyled(ROUNDED) |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               password_input->Render() | borderStyled(ROUNDED) |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               text(login_message) | bold | color(Color::Red),
               hbox({
                   login_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                   switch_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
               }),
               exit_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,

           }) |
           border | center | hcenter;
  });

  screen.Loop(renderer);
}

void showRegisterMenu(Accounts &accounts, Account &account,
                      std::string &login_message, bool &switch_to_register,
                      ScreenInteractive &screen) {
  std::string phoneNumber, username, password;

  auto username_input = Input(&username, "Username");
  auto password_input = Input(&password, "Password");
  auto phoneNumber_input = Input(&phoneNumber, "phone number");
  phoneNumber_input |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  phoneNumber_input |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  auto register_button = Button("Register", [&] {
    account.setPassWord(password);
    account.setPhoneNumber(phoneNumber);
    account.setUserName(username);
    registerAccount(accounts, account, login_message);
  });

  auto switch_button = Button("Back to Login", [&] {
    switch_to_register = false;
    screen.Exit();
  });
  auto exit_button = Button("Exit", [&] { exit(1); });
  auto container = Container::Vertical(Components{
      username_input,
      password_input,
      phoneNumber_input,
      register_button,
      switch_button,
      exit_button,
      Renderer([&] { return text(login_message) | bold | color(Color::Red); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               username_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               password_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               phoneNumber_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               text(login_message) | bold | color(Color::Red),
               hbox({register_button->Render() |
                         size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                     switch_button->Render() |
                         size(ftxui::WIDTH, ftxui::EQUAL, 25) | center}),
               exit_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,

           }) |
           hcenter | border | center;
  });

  screen.Loop(renderer);
}

Account showMenu(Accounts &accounts) {
  std::string login_message;
  bool switch_to_register = false;
  Account account;
  auto screen = ScreenInteractive::TerminalOutput();
  bool mainMenu = true;

  while (mainMenu) {
    if (switch_to_register) {
      clear();
      showRegisterMenu(accounts, account, login_message, switch_to_register,
                       screen);
    } else {
      clear();
      showLoginMenu(accounts, account, login_message, switch_to_register,
                    screen, mainMenu);
    }

    if (!switch_to_register && login_message == "Login successful!") {
      clear();
      break;
    }
  }

  return account;
}

using namespace ftxui;

void showUpdateAccountMenu(Accounts &accounts, Account &account) {
  std::string update_message;
  ScreenInteractive screen = ScreenInteractive::Fullscreen();
  std::string new_username, new_password, new_phoneNumber;

  auto new_username_input = Input(&new_username, "New Username");
  auto new_password_input = Input(&new_password, "New Password");
  auto new_phoneNumber_input = Input(&new_phoneNumber, "New Phone Number");

  auto update_button = Button("Update Account", [&] {
    account.setUserName(new_username);
    account.setPassWord(new_password);
    account.setPhoneNumber(new_phoneNumber);
    if (accounts.updateAccount(account)) {
      update_message = "Account updated successfully!";
    } else {
      update_message = "Account not found!";
    }
  });

  auto back_button = Button("Back", [&] { screen.Exit(); });

  auto container = Container::Vertical(Components{
      new_username_input,
      new_password_input,
      new_phoneNumber_input,
      update_button,
      back_button,
      Renderer([&] { return text(update_message) | bold | color(Color::Red); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               new_username_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               new_password_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               new_phoneNumber_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               text(update_message) | bold | color(Color::Red),
               update_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,
               back_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,
           }) |
           border | center;
  });

  screen.Loop(renderer);
}

void updateMovie(Movies &movies, Movie movie) {
  system("cls");
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  std::string message;
  std::string title;
  std::string description;
  std::string genre;
  std::string director;
  std::string cast;
  std::string duration;
  std::string releaseDate;

  Component input_title = Input(&title, movie.getTitle());
  Component input_description = Input(&description, movie.getDescription());
  Component input_genre = Input(&genre, movie.getGenre());
  Component input_director = Input(&director, movie.getDirector());
  Component input_cast = Input(&cast, movie.getCast());
  Component input_duration =
      Input(&duration, std::to_string(movie.getDuration()));
  Component input_releaseDate =
      Input(&releaseDate, movie.getReleaseDate().toString());
  input_duration |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  auto confirm_button = Button("confirm", [&] {
    if (title.empty() && description.empty() && genre.empty() &&
        director.empty() && cast.empty() && duration.empty() &&
        releaseDate.empty()) {
      message = "Please enter information!";
    } else {
      bool hasChanged = false;
      Date release;
      Movie updatedMovie = movie;
      if (title != movie.getTitle() && !title.empty()) {
        updatedMovie.setTitle(title);
        hasChanged = true;
      }

      if (description != movie.getDescription() && !description.empty()) {
        updatedMovie.setDescription(description);
        hasChanged = true;
      }

      if (genre != movie.getGenre() && !genre.empty()) {
        updatedMovie.setGenre(genre);
        hasChanged = true;
      }

      if (director != movie.getDirector() && !director.empty()) {
        updatedMovie.setDirector(director);
        hasChanged = true;
      }

      if (cast != movie.getCast() && !cast.empty()) {
        updatedMovie.setCast(cast);
        hasChanged = true;
      }

      if (duration != std::to_string(movie.getDuration()) &&
          !duration.empty()) {
        if (validInteger(duration)) {
          updatedMovie.setDuration(std::stoi(duration));

          hasChanged = true;
        }
      }

      if (releaseDate != movie.getReleaseDate() &&
          release.setDate(releaseDate)) {
        updatedMovie.setReleaseDate(releaseDate);
        hasChanged = true;
      }
      if (hasChanged) {
        movie = updatedMovie;
        movies.updateMovie(updatedMovie);
      }
      title = description = genre = director = cast = duration = releaseDate =
          "";
      system("cls");
    }
  });

  auto exit_button = Button("Exit", [&] { screen.Exit(); });

  auto component = Container::Vertical(
      {input_title, input_description, input_genre, input_director, input_cast,
       input_duration, input_releaseDate, confirm_button, exit_button});

  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Update Movie") | center,
               separator(),
               hbox(text(" Title : "), input_title->Render()),
               hbox(text(" Description  : "), input_description->Render()),
               hbox(text(" Genre :"), input_genre->Render()),
               hbox(text(" Director :"), input_director->Render()),
               hbox(text(" Cast :"), input_cast->Render()),
               hbox(text(" Duration :"), input_duration->Render()),
               hbox(text(" releaseDate :"), input_releaseDate->Render()),
               separator(),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message) | center | hcenter | color(Color::Blue)),

           }) |
           border;
  });

  screen.Loop(renderer);
}
void displayMovie(const Vector<Movie> &movie,
                  const Vector<ShowTime> &showtime) {
  Vector<Movie> movies = getMoviesWithFutureShowTimes(showtime, movie);
  using namespace ftxui;
  std::string search_query;
  auto screen = ScreenInteractive::Fullscreen();

  // Các trường nhập liệu
  auto search_input = Input(&search_query, "Search for movies...");
  bool exit = false;
  // Nút thoát
  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit(); // Thoát màn hình
  });

  // Tạo container cho các thành phần
  auto container = Container::Vertical(Components{
      search_input,
      exit_button,
  });

  // Tạo renderer để hiển thị giao diện
  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               DisplayMovies(movies,
                             search_query), // Hiển thị danh sách phim
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  // Tạo và khởi động màn hình
  while (!exit) { // Chỉ tiếp tục khi chưa chọn phim
    screen.Loop(renderer);
  }
}
void displayShowTimes(const Vector<Movie> &movies,
                      const Vector<ShowTime> &showtimes) {
  Vector<ShowTime> showTimes = getFutureShowTimes(showtimes, movies);
  using namespace ftxui;
  std::string search_query;
  auto screen = ScreenInteractive::Fullscreen();
  // Các trường nhập liệu
  auto search_input = Input(&search_query, "Search for movies...");

  auto exit_button = Button("Exit", [&] {
    system("cls");
    screen.Exit(); // Thoát màn hình
  });

  // Tạo container cho các thành phần
  auto container = Container::Vertical(Components{
      search_input,
      exit_button,
  });

  // Tạo renderer để hiển thị giao diện
  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               DisplayShowTimes(showTimes, movies,
                                search_query), // Hiển thị danh sách phim
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });
  // Tạo và khởi động màn hình

  screen.Loop(renderer);
}
void addShowTime(Movies &movies, ShowTimes &showTimes) {
  while (true) {
    int select = selectMovie(movies.getMovies(), showTimes.getShowTimes());
    if (select != -1) {
      addShowTime(getMovie(select, movies.getMovies()), showTimes);
    } else if (select == -1) {
      return;
    }
  }
}
ftxui::Element DisplayShowTimes(const Vector<ShowTime> &showTimes) {
  std::cin.ignore();
  using namespace ftxui;
  Elements showtime_rows;

  // Thêm tiêu đề bảng với màu xanh và phân cách giữa các cột
  showtime_rows.push_back(hbox(text("Showtime ID") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter,
                               separator(),
                               text("Show Date") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("Start Time") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("End Time") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 12) | hcenter,
                               separator(),
                               text("Hall ID") | bold | color(Color::Blue) |
                                   size(WIDTH, EQUAL, 10) | hcenter));

  // Thêm hàng cho từng ShowTime với phân cách giữa các hàng
  for (const auto &showtime : showTimes) {
    // Kiểm tra nếu tiêu đề phim phù hợp với chuỗi tìm kiếm
    showtime_rows.push_back(separator());
    showtime_rows.push_back(hbox(
        text(std::to_string(showtime.getShowTimeId())) | size(WIDTH, EQUAL, 10),
        separator(),
        text(showtime.getShowDate().toString()) | size(WIDTH, EQUAL, 12),
        separator(),
        text(showtime.getStartTime().toString()) | size(WIDTH, EQUAL, 12),
        separator(),
        text(showtime.getEndTime().toString()) | size(WIDTH, EQUAL, 12),
        separator(),
        text(std::to_string(showtime.getHallId())) | size(WIDTH, EQUAL, 10)));
  }

  return vbox(showtime_rows) | border | center;
}
ftxui::Element DisplayCheckedTicket(const Movie &movie,
                                    const ShowTime &showtime,
                                    const Account &account,
                                    Vector<int> isSelected, Tickets &tickets) {
  using namespace ftxui;
  Elements ticket_rows;

  // Thêm tiêu đề bảng với màu xanh và phân cách giữa các cột
  ticket_rows.push_back(
      hbox(paragraph("User Id") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 10) | hcenter,
           separator(),
           paragraph("Movie Title") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 20) | hcenter,
           separator(),
           paragraph("Show Date") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 12) | hcenter,
           separator(),
           paragraph("Start Time") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 12) | hcenter,
           separator(),
           paragraph("Seat Number") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 12) | hcenter,
           separator(),
           paragraph("Hall Id") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 12) | hcenter,
           separator(),
           paragraph("Cost") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 10) | hcenter));

  // Thêm hàng cho từng vé đã chọn
  for (int ticketId : isSelected) {
    ticket_rows.push_back(separator());
    ticket_rows.push_back(hbox(
        text(std::to_string(account.getUserId())) | size(WIDTH, EQUAL, 10),
        separator(),
        text(movie.getTitle()) |
            size(WIDTH, EQUAL,
                 20), // Giả định có phương thức để lấy tiêu đề phim
        separator(),
        text(showtime.getShowDate().toString()) | size(WIDTH, EQUAL, 12),
        separator(),
        text(showtime.getStartTime().toString()) | size(WIDTH, EQUAL, 12),
        separator(), text(std::to_string(ticketId)) | size(WIDTH, EQUAL, 12),
        separator(),
        text(std::to_string(showtime.getHallId())) | size(WIDTH, EQUAL, 12),
        separator(), text("65000 VND") | size(WIDTH, EQUAL, 10)));
  }

  return vbox(ticket_rows) | border | center;
}

bool Step4_DisplayCheckedTicket(const Movie &movie, const ShowTime &showtime,
                                const Account &account, Vector<int> isSelected,
                                Tickets &tickets) {
  using namespace ftxui;
  auto screen = ScreenInteractive::Fullscreen();
  bool exit = false, selected = false;

  // Nút thoát
  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit(); // Thoát màn hình
  });
  auto confirm_button = Button("Confirm", [&] {
    selected = true;
    for (int selected : isSelected) {
      tickets.addTicket(account, showtime, selected);
    }
    system("cls");
    screen.Exit(); // Thoát màn hình
  });

  // Tạo container cho các thành phần
  auto container = Container::Vertical(Components{
      confirm_button,
      exit_button,
  });

  // Tạo renderer để hiển thị giao diện
  auto renderer = Renderer(container, [&] {
    return vbox({
               text("Are you sure you want to booking it?") | bold,
               separator(),
               DisplayCheckedTicket(movie, showtime, account, isSelected,
                                    tickets), // Hiển thị danh sách phim
               separator(),
               text("Total: " + std::to_string(isSelected.size() * 65000) +
                    "VND"),
               hbox(exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
                    confirm_button->Render() | center | size(WIDTH, EQUAL, 25)),
           }) |
           border | center;
  });

  // Tạo và khởi động màn hình
  while (!selected && !exit) { // Chỉ tiếp tục khi chưa chọn phim
    screen.Loop(renderer);
  }
  if (exit) {
    return false;
  }
  // Trả về ID phim đã chọn
  return true; // Đây là ID hợp lệ đã chọn
}

void removeSeat(Vector<int> &selectedSeats, int seat) {
  Vector<int> updatedSeats; // Vector mới để lưu trữ các ghế còn lại

  for (int s : selectedSeats) {
    if (s != seat) {             // Nếu ghế không phải là ghế cần xóa
      updatedSeats.push_back(s); // Thêm ghế vào vector mới
    }
  }

  selectedSeats = updatedSeats; // Cập nhật selectedSeats bằng vector mới
}

bool Step3_SelectSeat(const Movie &movie, const ShowTime &showtime,
                      const Account &account, Tickets &tickets) {
  bool exit = false, selected = false;
  Vector<int> seats;
  for (int i = 1; i <= 25; ++i) {
    seats.push_back(i); // Gán số ghế từ 1 đến 25
  }

  Vector<bool> seatBooked; // Giả sử 25 ghế trong 5 hàng x 5 cột
  for (int i = 0; i < 25; i++) {
    seatBooked.push_back(false);
  }
  for (int i = 0; i < tickets.getTickets().size(); i++) {
    if (tickets.getTickets().at(i).getShowTimeId() ==
        showtime.getShowTimeId()) {
      int seatNumber = tickets.getTickets().at(i).getSeatNumber();
      if (seatNumber >= 1 && seatNumber <= seatBooked.size()) {
        seatBooked[seatNumber - 1] = true; // Đánh dấu ghế đã đặt
      }
    }
  }

  Vector<int> selectedSeats; // Danh sách ghế đã chọn
  auto screen = ScreenInteractive::Fullscreen();

  const int rows = 5;
  const int cols = 5;
  ftxui::Components seat_buttons;
  std::string message;

  for (int i = 0; i < rows; ++i) {
    ftxui::Components row_buttons;
    for (int j = 0; j < cols; ++j) {
      int seat_number = i * cols + j + 1;
      if (seat_number <= seats.size()) {
        auto &seat = seats[seat_number - 1];
        std::string seat_label = "Seat " + std::to_string(seat);

        if (seatBooked[seat_number - 1]) {
          row_buttons.push_back(Button(seat_label, [] {}) | color(Color::Red) |
                                size(WIDTH, EQUAL, 20));
        } else {
          bool is_selected =
              std::find(selectedSeats.begin(), selectedSeats.end(), seat) !=
              selectedSeats.end();
          row_buttons.push_back(
              Button(seat_label,
                     [&, seat_number, &is_selected] {
                       if (is_selected) {
                         removeSeat(selectedSeats, seat);
                       } else {
                         selectedSeats.push_back(seat);
                       }
                       is_selected = !is_selected;
                       message = "Selected Seats: ";
                       for (int seat : selectedSeats) {
                         message += std::to_string(seat) + " ";
                       }
                     }) |
              color(is_selected ? Color::Green : Color::White) |
              size(WIDTH, EQUAL, 20));
        }
      }
    }
    seat_buttons.push_back(Container::Horizontal(row_buttons));
  }

  auto book_button = Button("Book Selected Seats", [&] {
    if (!selectedSeats.empty()) {
      for (int seat_number : selectedSeats) {
        seatBooked[seat_number - 1] = true; // Đánh dấu ghế đã đặt
      }
      if (Step4_DisplayCheckedTicket(movie, showtime, account, selectedSeats,
                                     tickets)) {
        screen.Exit();
      }
    }
  });

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });
  auto container = Container::Vertical(seat_buttons);
  container->Add(book_button | center | size(WIDTH, EQUAL, 50));
  container->Add(exit_button | center | size(WIDTH, EQUAL, 50));

  auto renderer = Renderer(container, [&] {
    return vbox({
        text("Select Seats") | bold | center,
        separator(),
        container->Render() | center,
        separator(),
        text(message) | color(Color::Yellow) | center,
    });
  });
  screen.Loop(renderer);
  if (exit) {
    return false;
  }
  return true;
}

bool Step2_SelectShowTime(const Movie &movie, const Vector<ShowTime> &showtimes,
                          const Account &account, Tickets &tickets) {
  Vector<ShowTime> showTimes;
  for (auto st : showtimes) {
    if (st.getMovieId() == movie.getMovieId()) {
      showTimes.push_back(st);
    }
  }
  using namespace ftxui;
  std::string search_message;
  std::string showTimeId;
  auto screen = ScreenInteractive::Fullscreen();
  // Các trường nhập liệu
  auto showtime_input = Input(&showTimeId, "Enter showtime id...");

  bool exit = false;
  // Nút thoát
  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit(); // Thoát màn hình
  });

  bool selected = false; // Biến để theo dõi việc chọn phim
  auto select_showtime_button = Button("Enter", [&] {
    if (!showTimeId.empty() && validInteger(showTimeId)) {
      int id = std::stoi(showTimeId); // Chuyển đổi movieId sang số nguyên
      if (checkShowTimeId(id, showTimes)) {
        system("cls");
        ShowTime showtime;
        for (auto st : showtimes) {
          if (st.getShowTimeId() == id) {
            showtime = st;
            break;
          }
        }
        Step3_SelectSeat(movie, showtime, account, tickets);
        screen.Exit();
        exit = true;
      } else {
        search_message =
            "Showtime not found!"; // Thông báo nếu không tìm thấy phim
      }
    } else {
      search_message = "Invalid ShowTime ID!"; // Thông báo nếu ID không hợp lệ
    }
  });

  // Tạo container cho các thành phần
  auto container = Container::Vertical(Components{
      exit_button,
      showtime_input,
      select_showtime_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  // Tạo renderer để hiển thị giao diện
  auto renderer = Renderer(container, [&] {
    return vbox({
               text(movie.getTitle()),
               separator(),
               hbox({showtime_input->Render() | size(WIDTH, EQUAL, 25) | center,
                     select_showtime_button->Render() | center,
                     text(search_message) | center | color(Color::Red)}),
               DisplayShowTimes(showTimes), // Hiển thị danh sách phim
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });
  // Tạo và khởi động màn hình

  screen.Loop(renderer);
  if (exit) {
    return false;
  }
  return true;
}

void Step1_SelectMovie(const Vector<Movie> &movie,
                       const Vector<ShowTime> &showtimes,
                       const Account &accounts, Tickets &tickets) {
  Vector<Movie> movies = getMoviesWithFutureShowTimes(showtimes, movie);
  using namespace ftxui;
  std::string movieId;
  std::string search_query;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
  auto movie_input = Input(&movieId, "Enter Movie ID...");
  bool exit = false;

  auto exit_button = Button("Exit", [&] {
    exit = true;
    system("cls");
    screen.Exit();
  });

  bool selected = false;
  auto select_movie_button = Button("Enter", [&] {
    if (!movieId.empty() && validInteger(movieId)) {
      int id = std::stoi(movieId);
      if (checkMovieId(id, movies)) {
        system("cls");
        Movie movie;
        for (auto m : movies) {
          if (m.getMovieId() == id) {
            movie = m;
            break;
          }
        }
        Step2_SelectShowTime(movie, showtimes, accounts, tickets);
        selected = true;
        screen.Exit();
      } else {
        search_message = "Movie not found!";
      }
    } else {
      search_message = "Invalid Movie ID!";
    }
  });

  auto container = Container::Vertical(Components{
      search_input,
      movie_input,
      select_movie_button,
      exit_button,
      Renderer(
          [&] { return text(search_message) | bold | color(Color::Green); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                   center,
               hbox({
                   movie_input->Render() | border | size(WIDTH, EQUAL, 25) |
                       center,
                   select_movie_button->Render() | center,
                   text(search_message) | center | color(Color::Red),

               }),
               DisplayMovies(movies, search_query),
               exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
           }) |
           border | center;
  });

  while (!selected && !exit) {
    screen.Loop(renderer);
  }
}
void BookingTicket(const Vector<Movie> &movies,
                   const Vector<ShowTime> &showtimes, const Account &account,
                   Tickets &tickets) {
  Step1_SelectMovie(movies, showtimes, account, tickets);
}
bool checkDateByMonthAndYear(const Ticket &ticket, int month) {
  int ticketMonth = ticket.getBookingDate().getMonth();
  int ticketYear = ticket.getBookingDate().getYear();
  int currentYearValue = currentYear();

  if ((month > 5 && ticketMonth == month && ticketYear == currentYearValue) ||
      (month <= 5 && ticketMonth == month &&
       ticketYear == (currentYearValue - 1))) {
    return true;
  }
  return false;
}

int maxOfVectorInt(Vector<int> nums) {
  int max = nums[0];
  for (int i = 1; i < nums.size(); i++) {
    if (max < nums[i]) {
      max = nums[i];
    }
  }
  return max;
}

ftxui::Element displayMonthStatistics(const Vector<Ticket> &tickets) {
  using namespace ftxui;
  Elements month_Statistics;
  Vector<int> months = getLast6Months();
  Vector<int> totals(6);
  for (int i = 0; i < 6; i++) {
    totals[i] = 0;
  }

  totals = {5, 4, 3, 2, 0, 6};

  for (const auto &ticket : tickets) {
    int ticketMonth = ticket.getBookingDate().getMonth();
    int ticketYear = ticket.getBookingDate().getYear();

    // Kiểm tra nếu tháng có trong danh sách
    for (int i = 0; i < months.size(); ++i) {
      if (checkDateByMonthAndYear(ticket, months[i])) {
        totals[i] += ticket.getTicketPrice();
        break; // Dừng vòng lặp khi đã tìm thấy tháng hợp lệ
      }
    }
  }

  month_Statistics.push_back(hbox(text("Month") | bold | color(Color::Blue) |
                                      size(WIDTH, EQUAL, 10) | hcenter,
                                  separator(),
                                  text("Total") | bold | color(Color::Blue) |
                                      size(WIDTH, EQUAL, 12) | hcenter));

  for (int i = 5; i >= 0; --i) {
    month_Statistics.push_back(separator());
    month_Statistics.push_back(hbox(
        text(std::to_string(months.at(i))) | size(WIDTH, EQUAL, 10),
        separator(), text(std::to_string(totals[i])) | size(WIDTH, EQUAL, 12)));
  }

  int max = maxOfVectorInt(totals);
  Elements month_graph;
  month_graph.push_back(text("Graph") | bold | color(Color::Blue));
  month_graph.push_back(separator());

  for (int i = 5; i >= 0; --i) {
    month_graph.push_back(

        hbox(text(std::to_string(months[i])) | size(WIDTH, EQUAL, 3),
             gauge(float(totals[i]) / max) | color(Color::Red) |
                 size(WIDTH, EQUAL, 50)));
  }

  Elements result;
  result.push_back(
      vbox(vbox(month_Statistics) | border | center | size(WIDTH, EQUAL, 50),
           separator(),
           vbox(month_graph) | center | size(WIDTH, EQUAL, 50) | border));
  return vbox(result) | center;
}
bool checkDateByQuarter(const Ticket &ticket, int quarter) {
  int ticketMonth = ticket.getBookingDate().getMonth(); // Lấy tháng của vé
  int ticketYear = ticket.getBookingDate().getYear();   // Lấy năm của vé

  // Kiểm tra tháng của vé thuộc quý nào
  switch (quarter) {
  case 1: // Quý 1 (tháng 1 đến tháng 3)
    if (ticketMonth >= 1 && ticketMonth <= 3) {
      return true;
    }
    break;
  case 2: // Quý 2 (tháng 4 đến tháng 6)
    if (ticketMonth >= 4 && ticketMonth <= 6) {
      return true;
    }
    break;
  case 3: // Quý 3 (tháng 7 đến tháng 9)
    if (ticketMonth >= 7 && ticketMonth <= 9) {
      return true;
    }
    break;
  case 4: // Quý 4 (tháng 10 đến tháng 12)
    if (ticketMonth >= 10 && ticketMonth <= 12) {
      return true;
    }
    break;
  }

  return false;
}

ftxui::Element displayQuarterStatistics(const Vector<Ticket> &tickets,
                                        const Vector<Movie> &movies,
                                        const Vector<ShowTime> &showtimes) {
  using namespace ftxui;
  Elements quarter_Statistics;

  // Các tháng tương ứng với các quý
  Vector<int> quarters = {
      1, 2, 3, 4}; // 1: Quý 1 (tháng 1-3), 2: Quý 2 (tháng 4-6), v.v.
  Vector<int> totals(4,
                     0); // Dự trữ giá trị tổng của từng quý, khởi tạo bằng 0

  int year = currentYear(); // Năm hiện tại

  // Duyệt qua danh sách vé để tính tổng cho từng quý
  for (const auto &ticket : tickets) {
    int ticketMonth = ticket.getBookingDate().getMonth();
    int ticketYear = ticket.getBookingDate().getYear();

    // Kiểm tra nếu tháng có trong quý nào và cộng dồn tổng tiền cho quý đó
    for (int i = 0; i < quarters.size(); ++i) {
      if (checkDateByQuarter(ticket, quarters[i])) {
        totals[i] += ticket.getTicketPrice();
        break; // Dừng vòng lặp khi đã tìm thấy quý hợp lệ
      }
    }
  }

  // Thêm tiêu đề bảng thống kê
  quarter_Statistics.push_back(
      hbox(text("Quarter") | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 10) | hcenter,
           text("Year: " + std::to_string(year)) | bold | color(Color::Blue) |
               size(WIDTH, EQUAL, 20) | hcenter,
           separator(),
           text("Total") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 12) |
               hcenter));

  // Thêm thông tin tổng cho mỗi quý
  for (int i = 0; i < quarters.size(); ++i) {
    quarter_Statistics.push_back(separator());
    quarter_Statistics.push_back(hbox(
        text("Q" + std::to_string(quarters[i])) | size(WIDTH, EQUAL, 10),
        text(" " + std::to_string(year)) |
            size(WIDTH, EQUAL, 6), // Thêm năm vào quý
        separator(), text(std::to_string(totals[i])) | size(WIDTH, EQUAL, 12)));
  }

  // Tính giá trị tối đa để vẽ biểu đồ tỉ lệ
  int max = maxOfVectorInt(totals);
  Elements quarter_graph;
  quarter_graph.push_back(text("Graph") | bold | color(Color::Blue));
  quarter_graph.push_back(separator());

  // Vẽ biểu đồ tỉ lệ cho các quý
  for (int i = 0; i < quarters.size(); ++i) {
    quarter_graph.push_back(
        hbox(text("Q" + std::to_string(quarters[i])) | size(WIDTH, EQUAL, 10),
             gauge(float(totals[i]) / max) | color(Color::Red) |
                 size(WIDTH, EQUAL, 50)));
  }

  // Kết quả cuối cùng
  Elements result;
  result.push_back(
      vbox(vbox(quarter_Statistics) | border | center | size(WIDTH, EQUAL, 50),
           separator(),
           vbox(quarter_graph) | center | size(WIDTH, EQUAL, 50) | border));
  return vbox(result) | center;
}

ftxui::Element displayYearStatistics(const Vector<Ticket> &tickets,
                                     const Vector<Movie> &movies,
                                     const Vector<ShowTime> &showtimes) {
  using namespace ftxui;
  Elements year_Statistics;

  // Danh sách các năm cần thống kê, ví dụ: năm hiện tại và năm trước đó
  Vector<int> years = {currentYear(), currentYear() - 1, currentYear() - 2,
                       currentYear() - 3};
  Vector<int> totals(4, 0); // Dự trữ tổng tiền cho từng năm, khởi tạo bằng 0

  // Duyệt qua danh sách vé và tính tổng cho từng năm
  for (const auto &ticket : tickets) {
    int ticketYear = ticket.getBookingDate().getYear();

    // Kiểm tra xem vé có thuộc năm nào trong danh sách không
    for (int i = 0; i < years.size(); ++i) {
      if (ticketYear == years[i]) {
        totals[i] += ticket.getTicketPrice();
        break; // Dừng vòng lặp khi tìm thấy năm hợp lệ
      }
    }
  }

  // Thêm tiêu đề cho bảng thống kê
  year_Statistics.push_back(hbox(text("Year") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 10) | hcenter,
                                 separator(),
                                 text("Total") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 12) | hcenter));

  // Thêm thông tin tổng cho từng năm
  for (int i = 0; i < years.size(); ++i) {
    year_Statistics.push_back(separator());
    year_Statistics.push_back(hbox(
        text(std::to_string(years[i])) | size(WIDTH, EQUAL, 10), separator(),
        text(std::to_string(totals[i])) | size(WIDTH, EQUAL, 12)));
  }

  // Tính giá trị tối đa để vẽ biểu đồ tỉ lệ
  int max = maxOfVectorInt(totals);
  Elements year_graph;
  year_graph.push_back(text("Graph") | bold | color(Color::Blue));
  year_graph.push_back(separator());

  // Vẽ biểu đồ tỉ lệ cho từng năm
  for (int i = 0; i < years.size(); ++i) {
    year_graph.push_back(
        hbox(text(std::to_string(years[i])) | size(WIDTH, EQUAL, 10) | center,
             gauge(float(totals[i]) / max) | color(Color::Red) |
                 size(WIDTH, EQUAL, 45)));
  }

  // Kết quả cuối cùng
  Elements result;
  result.push_back(
      vbox(vbox(year_Statistics) | border | center | size(WIDTH, EQUAL, 50),
           separator(),
           vbox(year_graph) | center | size(WIDTH, EQUAL, 50) | border));
  return vbox(result) | center;
}

ftxui::Element DisplayStatistics(const Vector<Ticket> &tickets,
                                 const Vector<Movie> &movies,
                                 const Vector<ShowTime> &showtimes, int Check) {
  switch (Check) {
  case 1:
    // Logic for displaying statistics for the month
    return displayMonthStatistics(tickets);
  case 2:
    // Logic for displaying statistics for the quarter
    return displayQuarterStatistics(tickets, movies, showtimes);
  case 3:
    // Logic for displaying statistics for the year
    return displayYearStatistics(tickets, movies, showtimes);
  default:
    return displayMonthStatistics(tickets);
  }
}

void Statistic(const Vector<Ticket> &tickets, const Vector<Movie> &movies,
               const Vector<ShowTime> &showtimes) {
  auto screen = ScreenInteractive::Fullscreen();

  // Initialize 'Check' as a valid boolean (e.g., false or true)
  int Check = 1; // Example: You can set it based on user input or other logic

  // Define buttons
  Component statistics_month =
      Button("1. Statistics Month", [&] { Check = 1; });
  Component statistics_quarter =
      Button("2. Statistics Quarter", [&] { Check = 2; });
  Component statistics_year = Button("3. Statistics Year", [&] { Check = 3; });
  Component back_button = Button("Back", [&] { screen.Exit(); });

  // Create a vertical container for the buttons
  auto container = Container::Vertical({
      statistics_month,
      statistics_quarter,
      statistics_year,
      back_button,
  });

  // Renderer for the entire UI
  auto renderer = Renderer(container, [&] {
    return hbox({
               // Left-side menu
               vbox({text("Statistics") | center | bold, separator(),
                     statistics_month->Render(), statistics_quarter->Render(),
                     statistics_year->Render(), separator(),
                     back_button->Render()}) |
                   border | center |
                   size(WIDTH, EQUAL, 50), // Style for the left panel

               separator(),

               // Right-side panel with statistics display
               DisplayStatistics(tickets, movies, showtimes,
                                 Check) |
                   center // Pass 'Check' to DisplayStatistics
           }) |
           center;
  });

  screen.Loop(renderer);
}

#endif
// bool checkShowTimeExistInTicket(const Ticket &ticket, const Vector<ShowTime>
// &showtimes); ShowTime getShowTimeFromTicket(const Ticket &ticket, const
// Vector<ShowTime> &showtimes); bool checkMovieId(int movieId, const
// Vector<Movie> &movies); Movie getMovie(int movieId, const Vector<Movie>
// &movies); Vector<int> GetTicketsSoldLast6Months();
