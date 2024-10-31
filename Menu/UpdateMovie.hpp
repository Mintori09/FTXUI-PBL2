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

void updateMovie(Movies &movies, Movie movie) {
    system("cls");
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    // The data:
    std::string message;
    std::string title;
    std::string description;
    std::string genre;
    std::string director;
    std::string cast;
    std::string duration;     // int
    std::string releaseDate;  // Date
    // The password input component:
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
                    updatedMovie.setDuration(
                        std::stoi(duration));  // Chuyển `duration` từ chuỗi
                                               // sang số nguyên
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
            title = description = genre = director = cast = duration =
                releaseDate = "";
            system("cls");
        }
    });

    auto exit_button = Button("Exit", [&] { screen.Exit(); });
    // The component tree:
    auto component =
        Container::Vertical({input_title, input_description, input_genre,
                             input_director, input_cast, input_duration,
                             input_releaseDate, confirm_button, exit_button});
    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        return vbox({
                   text("Update Movie") | center, separator(),
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
    for (auto m : movies.getMovies()) {
        std::cout << "ID: " << m.getMovieId()
                  << ", Title: " << m.getReleaseDate() << std::endl;
        system("pause");
    }
    while (true) {
        int select = selectMovie(movies.getMovies(), showTimes.getShowTimes());
        if (select != -1) {
            updateMovie(movies, getMovie(select, movies.getMovies()));
        } else if (select == -1) {
            break;
        }
    }
    return 0;
}