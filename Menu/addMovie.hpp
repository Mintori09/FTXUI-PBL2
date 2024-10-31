#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/screen/color.hpp>
#include <ftxui/util/ref.hpp>  // for Ref

#include "../include/Movies.h"
#include "../include/my_lib.h"

int addMovie(Movies &movies) {
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
            // Thực hiện hành động khi tất cả các giá trị đều trống
            message = "Please enter information!";
        } else {
            Date release;
            if (release.setDate(releaseDate)) {
                if (validInteger(duration)) {
                    // Thêm phim vào danh sách phim
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
    // The component tree:
    auto component =
        Container::Vertical({input_title, input_description, input_genre,
                             input_director, input_cast, input_duration,
                             input_releaseDate, confirm_button, exit_button});
    // Tweak how the component tree is rendered:
    auto renderer = Renderer(component, [&] {
        return vbox({
                   text("Adding new Movie") | center, separator(),
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
