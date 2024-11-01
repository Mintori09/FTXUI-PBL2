
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
bool checkShowTimeExistInTicket(const Ticket &ticket,
                                const Vector<ShowTime> &showtimes) {
  for (auto st : showtimes) {
    if (st.getShowTimeId() == ticket.getShowTimeId()) {
      return true;
    }
  }
  return false;
}
ShowTime getShowTimeFromTicket(const Ticket &ticket,
                               const Vector<ShowTime> &showtimes) {
  for (auto st : showtimes) {
    if (st.getShowTimeId() == ticket.getShowTimeId()) {
      return st;
    }
  }
}
int addMovie(Movies &movies) {
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

  Component hall_id_input = Input(&hallId, "hall id");
  Component show_date_input = Input(&showDate, "dd-mm-yyyy");
  Component start_time_input = Input(&startTime, "hh:mm");

  auto confirm_button = Button("confirm", [&] {
    if (hallId.empty() && showDate.empty() && startTime.empty()) {
      message = "Please add information!";
    } else {
      if (validInteger(hallId)) {
        int hallID = std::stoi(hallId);
        if (hallID < 4 && hallID > 0) {
          message = "Step 1";
          Time start(startTime);
          Time endTime = start.add(movie.getDuration());
          ShowTime newShowTime(movieId, hallID, showDate, startTime, endTime);
          message = "Step 2";
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
               text("Hello " + username),
               separator(),
               hbox(text(" Password : "), input_password->Render()),
               hbox(text(" phone number  : "), input_phone_number->Render()),
               hbox(text(" Confirm password :"),
                    input_current_password->Render()),
               separator(),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message)),
               separator(),
               text(account.getPassWord()),
               text(account.getUserName()),
               text(account.getPhoneNumber()),
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

    if (account.getUserId() == ticket.getUserId()) {
      if (checkShowTimeExistInTicket(ticket, showtimes)) {
        ShowTime showtime = getShowTimeFromTicket(ticket, showtimes);
        showtime_rows.push_back(separator());
        showtime_rows.push_back(hbox(
            text(std::to_string(ticket.getTicketId())) | size(WIDTH, EQUAL, 15),
            separator(),
            paragraph(account.getUserName()) | size(WIDTH, EQUAL, 20),
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
}
void ShowTicketInfo(const Vector<Account> accounts,
                    const Vector<Ticket> &tickets, const Vector<Movie> &movies,
                    const ShowTime &showtime) {
  using namespace ftxui;
  std::string search_query;
  std::string search_message;
  auto screen = ScreenInteractive::Fullscreen();

  auto search_input = Input(&search_query, "Search for movies...");
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
                              const Vector<Account> accounts,
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

#endif
