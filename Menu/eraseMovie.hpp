#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/screen/color.hpp>
#include <ftxui/util/ref.hpp>  // for Ref
#include <string>

#include "../include/Movies.h"
#include "../include/ShowTimes.h"
#include "../include/my_lib.h"
#include "DisplayTable.hpp"


void eraseMovie(Movies &movies, Movie movie) {
    system("cls");
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    // The data:
    std::string message;

    auto confirm_button = Button("Confirm", [&] {
        movies.deleteMovie(movie);
        system("cls");
    });

    auto back_button = Button("Back", [&] {
        system("cls");
        screen.Exit();
    });
    // The component tree:
    auto component = Container::Vertical({confirm_button, back_button});
    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        return vbox({
                   text("Are you sure about delete this movie? ") | center,
                   separator(),
                   hbox(vbox(paragraph(" Title : " + movie.getTitle()),
                             paragraph(" Description  : " +
                                       movie.getDescription()),
                             text(" Genre :" + movie.getGenre()),
                             text(" Director :" + movie.getDirector()),
                             text(" Cast :" + movie.getCast()),
                             text(" Duration :" +
                                  std::to_string(movie.getDuration())),
                             text(" releaseDate :" +
                                  movie.getReleaseDate().toString())) |
                            size(WIDTH, GREATER_THAN, 50),
                        separator(),
                        vbox(confirm_button->Render(), back_button->Render()) |
                            color(Color::Blue)),
                   //    separator(),
                   //    text("Title : " + title),
                   //    text("Description :" + description),
                   //    text("Genre : " + genre),
                   //    text("Director : " + director),
                   //    text("Cast : " + cast),
                   //    text("Duration : " + duration),
                   //    text("Release Date :" + releaseDate),
               }) |
               border;
    });

    screen.Loop(renderer);
}

int main() {
    Movies movies(MOVIE_FILE);
    ShowTimes showTimes(SHOWTIME_FILE);
    while (true) {
        int select = selectMovie(movies.getMovies(), showTimes.getShowTimes());
        if (select != -1) {
            eraseMovie(movies, getMovie(select, movies.getMovies()));
        } else if (select == -1) {
            break;
        }
    }
    return 0;
}