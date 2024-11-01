#include <ftxui/component/component.hpp>      // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp> // for ComponentBase
#include <ftxui/component/component_options.hpp> // for InputOption
#include <ftxui/component/screen_interactive.hpp> // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp> // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/screen/color.hpp>
#include <ftxui/util/ref.hpp> // for Ref
#include <string>

#include "../include/Movies.h"
#include "../include/ShowTimes.h"
#include "../include/my_lib.h"
#include "DisplayTable.hpp"

// int showTimeId;
// int movieId;
// int hallId;
// Date showDate;
// Time startTime;
// Time endTime;
void addShowTime(const Movie &movie, ShowTimes &showtimes) {
  system("cls");
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();

  // The data:
  std::string message;
  // int movieId = selectMovieForShowTime(movies.getMovies(),
  // showtimes.getShowTimes());
  int movieId = movie.getMovieId();
  std::string hallId;
  std::string showDate;
  std::string startTime;
  std::string duration; // Date
  // The password input component:
  Component hall_id_input = Input(&hallId, "hall id");
  Component show_date_input = Input(&showDate, "dd-mm-yyyy");
  Component start_time_input = Input(&startTime, "hh:mm");

  auto confirm_button = Button("confirm", [&] {
    if (hallId.empty() && showDate.empty() && startTime.empty()) {
      // Thực hiện hành động khi tất cả các giá trị đều trống
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
  // The component tree:
  auto component =
      Container::Vertical({hall_id_input, show_date_input, start_time_input,
                           confirm_button, exit_button});
  // Tweak how the component tree is rendered:
  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Adding new ShowTime ...."), separator(),
               text("Title :" + movie.getTitle()),
               text("Duration :" + std::to_string(movie.getDuration())),
               hbox(text("Hall ID :"), hall_id_input->Render()),
               hbox(text("Show Date :"), show_date_input->Render()),
               hbox(text("Start Time :"), start_time_input->Render()),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message) | center | hcenter | color(Color::Blue)),
               //    separator(),
               //    text("Title : " + title),
               //    text("Description :" +  description),
               //    text("Genre : " + genre),
               //    text("Director : "+director),
               //    text("Cast : " +cast),
               //    text("Duration : "+ duration),
               //    text("Release Date :" +releaseDate),
           }) |
           border;
  });

  screen.Loop(renderer);
}
void addShowTime(Movies &movies, ShowTimes &showTimes) {
  while (true) {
    int select = selectMovie(movies.getMovies(), showTimes.getShowTimes());
    if (select != -1) {
      addShowTime(getMovie(select, movies.getMovies()), showTimes);
    } else if (select == -1) {
      break;
    }
  }
}
int main() {
  Movies movies(MOVIE_FILE);
  ShowTimes showTimes(SHOWTIME_FILE);
  while (true) {
    int select = selectMovie(movies.getMovies(), showTimes.getShowTimes());
    if (select != -1) {
      addShowTime(getMovie(select, movies.getMovies()), showTimes);
    } else if (select == -1) {
      break;
    }
  }
}
