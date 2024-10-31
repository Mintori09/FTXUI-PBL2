
#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace ftxui;

struct Movie {
  int id;
  std::string title;
  Movie(int id, std::string title) : id(id), title(std::move(title)) {}
};

struct ShowTime {
  int id;
  std::string time;
  ShowTime(int id, std::string time) : id(id), time(std::move(time)) {}
};

struct Seat {
  int number;
  Seat(int number) : number(number) {}
};

void ShowMessage(const std::string &message) {
  std::cout << message << std::endl;
}

void ShowMovieMenu(const std::vector<Movie> &movies,
                   std::function<void(int)> onMovieSelected) {
  auto screen = ScreenInteractive::Fullscreen();
  std::vector<Component> movie_buttons;

  for (const auto &movie : movies) {
    movie_buttons.push_back(Button(movie.title, [&] {
      onMovieSelected(movie.id);
      screen.Exit();
    }));
  }

  auto exit_button = Button("Exit", [&] { screen.Exit(); });
  auto container = Container::Vertical(movie_buttons);
  container->Add(exit_button);

  auto renderer = Renderer(container, [&] {
    return vbox({
        text("Select a Movie") | bold | center,
        separator(),
        container->Render(),
    });
  });

  screen.Loop(renderer);
}

void ShowShowTimeMenu(const std::vector<ShowTime> &showTimes,
                      std::function<void(int)> onShowTimeSelected) {
  auto screen = ScreenInteractive::Fullscreen();
  std::vector<Component> showtime_buttons;

  for (const auto &showTime : showTimes) {
    showtime_buttons.push_back(Button(showTime.time, [&] {
      onShowTimeSelected(showTime.id);
      screen.Exit();
    }));
  }

  auto exit_button = Button("Exit", [&] { screen.Exit(); });
  auto container = Container::Vertical(showtime_buttons);
  container->Add(exit_button);

  auto renderer = Renderer(container, [&] {
    return vbox({
        text("Select a Show Time") | bold | center,
        separator(),
        container->Render(),
    });
  });

  screen.Loop(renderer);
}

void ShowSeatMenu(
    std::vector<Seat> &seats, std::vector<bool> &seatBooked,
    std::function<void(const std::vector<int> &)> onSeatsSelected) {
  auto screen = ScreenInteractive::Fullscreen();

  const int rows = 5;
  const int cols = 5;
  std::vector<Component> seat_buttons;
  std::vector<int> selectedSeats;
  std::string message;

  for (int i = 0; i < rows; ++i) {
    std::vector<Component> row_buttons;
    for (int j = 0; j < cols; ++j) {
      int seat_number = i * cols + j + 1;
      if (seat_number <= seats.size()) {
        auto &seat = seats[seat_number - 1];
        std::string seat_label = "Seat " + std::to_string(seat.number);

        if (seatBooked[seat_number - 1]) {
          row_buttons.push_back(Button(seat_label, [] {}) | color(Color::Red) |
                                size(WIDTH, EQUAL, 20));
        } else {
          bool is_selected =
              std::find(selectedSeats.begin(), selectedSeats.end(),
                        seat.number) != selectedSeats.end();
          row_buttons.push_back(
              Button(seat_label,
                     [&, seat_number, &is_selected] {
                       if (is_selected) {
                         selectedSeats.erase(std::remove(selectedSeats.begin(),
                                                         selectedSeats.end(),
                                                         seat.number),
                                             selectedSeats.end());
                       } else {
                         selectedSeats.push_back(seat.number);
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
    if (selectedSeats.empty()) {
      ShowMessage("No seats selected.");
    } else {
      ShowMessage("You have selected " + std::to_string(selectedSeats.size()) +
                  " seat(s).");
      for (int seat_number : selectedSeats) {
        seatBooked[seat_number - 1] = true;
      }
      onSeatsSelected(selectedSeats);
      screen.Exit();
    }
  });

  auto exit_button = Button("Exit", [&] { screen.Exit(); });
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
}

std::vector<int> BookTicket(int movieId, int showTimeId,
                            const std::vector<int> &seatNumbers) {
  ShowMessage("Tickets booked successfully!");
  ShowMessage("Movie ID: " + std::to_string(movieId) +
              ", Show Time ID: " + std::to_string(showTimeId));
  ShowMessage("Seat Numbers: ");
  for (int seat : seatNumbers) {
    std::cout << " - Seat " << seat << std::endl;
  }
  return seatNumbers;
}

int main() {
  std::vector<Movie> movies = {
      {1, "The Shawshank Redemption"},
      {2, "The Godfather"},
      {3, "The Dark Knight"},
  };

  std::vector<ShowTime> showTimes = {
      {1, "10:00 AM"},
      {2, "1:00 PM"},
      {3, "4:00 PM"},
  };

  std::vector<Seat> seats;
  for (int i = 1; i <= 25; ++i) {
    seats.emplace_back(i);
  }

  std::vector<bool> seatBooked(25, false);
  seatBooked[0] = true;
  seatBooked[4] = true;
  seatBooked[10] = true;

  int selectedMovieId = -1;
  int selectedShowTimeId = -1;
  std::vector<int> bookedSeats;

  ShowMovieMenu(movies, [&](int movieId) {
    selectedMovieId = movieId;
    ShowShowTimeMenu(showTimes, [&](int showTimeId) {
      selectedShowTimeId = showTimeId;
      ShowSeatMenu(seats, seatBooked, [&](const std::vector<int> &seatNumbers) {
        bookedSeats =
            BookTicket(selectedMovieId, selectedShowTimeId, seatNumbers);
      });
    });
  });

  for (int seat : bookedSeats) {
    std::cout << "Seat " << seat << " has been booked." << std::endl;
  }

  return 0;
}
