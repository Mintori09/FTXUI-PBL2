#include <ftxui/component/component.hpp>           // For Renderer
#include <ftxui/component/screen_interactive.hpp>  // For ScreenInteractive
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>  // For text, vbox, etc.

#include "../include/Movie.h"
#include "../include/ShowTime.h"
#include "../include/Vector.h"
#include "../include/my_lib.h"

// Hàm lấy tiêu đề phim từ Movie ID
std::string getMovieTitle(int movieId, const Vector<Movie>& movies) {
    for (const auto& movie : movies) {
        if (movie.getMovieId() == movieId) {
            return movie.getTitle();
        }
    }
    return "";  // Trả về chuỗi rỗng nếu không tìm thấy
}

bool checkMovieId(int movieId, const Vector<Movie>& movies) {
    // Duyệt qua từng bộ phim trong danh sách
    for (const auto& movie : movies) {
        if (movie.getMovieId() == movieId) {
            return true;  // Trả về true nếu tìm thấy movieId
        }
    }
    return false;  // Trả về false nếu không tìm thấy
}

Movie getMovie(int movieId, const Vector<Movie>& movies) {
    // Duyệt qua từng bộ phim trong danh sách
    for (const auto& movie : movies) {
        if (movie.getMovieId() == movieId) {
            return movie;  // Trả về phim nếu tìm thấy movieId
        }
    }
    return Movie();  // Trả về Movie r��ng nếu không tìm thấy
}

bool checkShowTimeId(int showTimeId, const Vector<ShowTime>& showTimes) {
    // Duyệt qua từng lịch chiếu trong danh sách
    for (const auto& showtime : showTimes) {
        if (showtime.getShowTimeId() == showTimeId) {
            return true;  // Trả về true nếu tìm thấy showTimeId
        }
    }
    return false;  // Trả về false nếu không tìm thấyq
}

Vector<ShowTime> getFutureShowTimes(const Vector<ShowTime>& showTimes,
                                    const Vector<Movie>& movies) {
    Date today(currentDate());
    Time timeNow(currentTime());
    Vector<ShowTime> futureShowTimes;

    for (const auto& showtime : showTimes) {
        Date showDate(showtime.getShowDate());

        // Kiểm tra nếu là lịch chiếu tương lai
        if (showDate > today ||
            (showDate == today && showtime.getStartTime() > timeNow)) {
            // Kiểm tra nếu phim của lịch chiếu nằm trong danh sách phim
            for (const auto& movie : movies) {
                if (movie.getMovieId() == showtime.getMovieId()) {
                    futureShowTimes.push_back(showtime);
                    break;  // Dừng kiểm tra thêm các phim khác cho lịch chiếu
                            // này
                }
            }
        }
    }
    return futureShowTimes;
}
Vector<ShowTime> getFutureAndCurrentShowTimes(const Vector<ShowTime>& showTimes,
                                              const Vector<Movie>& movies) {
    Date today(currentDate());
    Time timeNow(currentTime());
    Vector<ShowTime> futureShowTimes;

    for (const auto& showtime : showTimes) {
        Date showDate(showtime.getShowDate());

        // Kiểm tra nếu là lịch chiếu tương lai
        if (showDate > today ||
            (showDate == today && showtime.getEndTime() > timeNow)) {
            // Kiểm tra nếu phim của lịch chiếu nằm trong danh sách phim
            for (const auto& movie : movies) {
                if (movie.getMovieId() == showtime.getMovieId()) {
                    futureShowTimes.push_back(showtime);
                    break;  // Dừng kiểm tra thêm các phim khác cho lịch chiếu
                            // này
                }
            }
        }
    }
    return futureShowTimes;
}

Vector<Movie> getMoviesWithFutureShowTimes(const Vector<ShowTime>& showTimes,
                                           const Vector<Movie>& movies) {
    Vector<ShowTime> futureShowTimes = getFutureShowTimes(showTimes, movies);
    Vector<Movie> moviesWithFutureShowTimes;
    Vector<int> movieIdsAdded;  // Danh sách ID phim đã thêm

    for (const auto& showtime : futureShowTimes) {
        int movieId = showtime.getMovieId();

        // Kiểm tra xem phim đã được thêm chưa
        if (std::find(movieIdsAdded.begin(), movieIdsAdded.end(), movieId) ==
            movieIdsAdded.end()) {
            for (const auto& movie : movies) {
                if (movie.getMovieId() == movieId) {
                    moviesWithFutureShowTimes.push_back(movie);
                    movieIdsAdded.push_back(
                        movieId);  // Thêm ID phim vào danh sách
                    break;         // Dừng kiểm tra thêm các phim khác
                }
            }
        }
    }
    return moviesWithFutureShowTimes;
}

// Hàm hiển thị danh sách các Movie dưới dạng bảng
ftxui::Element DisplayMovies(const Vector<Movie>& movies,
                             const std::string& search_query) {
    using namespace ftxui;
    Elements movie_rows;

    // Thêm tiêu đề bảng với màu xanh và phân cách giữa các cột
    movie_rows.push_back(hbox(text("ID") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 5) | hcenter,
                              separator(),
                              text("Title") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 20) | hcenter,
                              separator(),
                              text("Genre") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 10) | hcenter,
                              separator(),
                              text("Description") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 30) | hcenter,
                              separator(),
                              text("Director") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 15) | hcenter,
                              separator(),
                              text("Duration (mins)") | bold |
                                  color(Color::Blue) | size(WIDTH, EQUAL, 15) |
                                  hcenter,
                              separator(),
                              text("Release Date") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 12) | hcenter,
                              separator(),
                              text("Rating") | bold | color(Color::Blue) |
                                  size(WIDTH, EQUAL, 10) | hcenter));

    // Thêm hàng cho từng Movie với phân cách giữa các hàng
    for (const auto& movie : movies) {
        // Lọc phim theo chuỗi tìm kiếm
        if (search_query.empty() ||
            isAllWordsInString(search_query, movie.getTitle())) {
            movie_rows.push_back(separator());
            movie_rows.push_back(
                hbox(text(std::to_string(movie.getMovieId())) |
                         size(WIDTH, EQUAL, 5),
                     separator(),
                     paragraph(movie.getTitle()) | size(WIDTH, EQUAL, 20),
                     separator(),
                     paragraph(movie.getGenre()) | size(WIDTH, EQUAL, 10),
                     separator(),
                     paragraph(movie.getDescription()) | bold |
                         size(WIDTH, EQUAL, 30) | hcenter,
                     separator(),
                     paragraph(movie.getDirector()) | size(WIDTH, EQUAL, 15),
                     separator(),
                     text(std::to_string(movie.getDuration())) |
                         size(WIDTH, EQUAL, 15),
                     separator(),
                     text(movie.getReleaseDate().toString()) |
                         size(WIDTH, EQUAL, 12),
                     separator(),
                     text(std::to_string(movie.getRating())) |
                         size(WIDTH, EQUAL, 10)));
        }
    }

    return vbox(movie_rows) | border | center;
}

ftxui::Element DisplayShowTimes(const Vector<ShowTime>& showTimes,
                                const Vector<Movie>& movies,
                                const std::string& search_query) {
    std::cin.ignore();
    using namespace ftxui;
    Elements showtime_rows;

    // Thêm tiêu đề bảng với màu xanh và phân cách giữa các cột
    showtime_rows.push_back(
        hbox(text("Showtime ID") | bold | color(Color::Blue) |
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

    // Thêm hàng cho từng ShowTime với phân cách giữa các hàng
    for (const auto& showtime : showTimes) {
        std::string movieTitle = getMovieTitle(showtime.getMovieId(), movies);

        // Kiểm tra nếu tiêu đề phim phù hợp với chuỗi tìm kiếm
        if (search_query.empty() ||
            isAllWordsInString(search_query, movieTitle)) {
            showtime_rows.push_back(separator());
            showtime_rows.push_back(hbox(
                text(std::to_string(showtime.getShowTimeId())) |
                    size(WIDTH, EQUAL, 10),
                separator(), paragraph(movieTitle) | size(WIDTH, EQUAL, 20),
                separator(),
                text(showtime.getShowDate().toString()) |
                    size(WIDTH, EQUAL, 12),
                separator(),
                text(showtime.getStartTime().toString()) |
                    size(WIDTH, EQUAL, 12),
                separator(),
                text(showtime.getEndTime().toString()) | size(WIDTH, EQUAL, 12),
                separator(),
                text(std::to_string(showtime.getHallId())) |
                    size(WIDTH, EQUAL, 10)));
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

    // Các trường nhập liệu
    auto search_input = Input(&search_query, "Search for movies...");
    auto movie_input = Input(&movieId, "Enter Movie ID...");
    bool exit = false;
    // Nút thoát
    auto exit_button = Button("Exit", [&] {
        exit = true;
        system("cls");
        screen.Exit();  // Thoát màn hình
    });

    bool selected = false;  // Biến để theo dõi việc chọn phim
    auto select_movie_button = Button("Enter", [&] {
        // Kiểm tra nếu movieId không rỗng và là số
        if (!movieId.empty() && validInteger(movieId)) {
            int id = std::stoi(movieId);  // Chuyển đổi movieId sang số nguyên
            if (checkMovieId(id, movies)) {
                system("cls");
                selected = true;            // Đánh dấu đã chọn phim
                screen.Exit();              // Thoát màn hình
            } else {
                search_message =
                    "Movie not found!";  // Thông báo nếu không tìm thấy phim
            }
        } else {
            search_message =
                "Invalid Movie ID!";  // Thông báo nếu ID không hợp lệ
        }
    });

    // Tạo container cho các thành phần
    auto container = Container::Vertical(Components{
        search_input,
        movie_input,
        select_movie_button,
        exit_button,
        Renderer(
            [&] { return text(search_message) | bold | color(Color::Green); }),
    });

    // Tạo renderer để hiển thị giao diện
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
                   DisplayMovies(movies,
                                 search_query),  // Hiển thị danh sách phim
                   exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
               }) |
               border | center;
    });

    // Tạo và khởi động màn hình
    while (!selected && !exit) {  // Chỉ tiếp tục khi chưa chọn phim
        screen.Loop(renderer);
    }
    if (exit) {
        return -1;
    }
    // Trả về ID phim đã chọn
    return std::stoi(movieId);  // Đây là ID hợp lệ đã chọn
}

int selectMovieForShowTime(const Vector<Movie>& movie, const Vector<ShowTime>& showtime) {
    Vector<Movie> movies = getMoviesWithFutureShowTimes(showtime, movie);
    using namespace ftxui;
    std::string movieId;
    std::string search_query;
    std::string search_message;
    auto screen = ScreenInteractive::Fullscreen();

    // Các trường nhập liệu
    auto search_input = Input(&search_query, "Search for movies...");
    auto movie_input = Input(&movieId, "Enter Movie ID...");
    bool exit = false;
    // Nút thoát
    auto exit_button = Button("Exit", [&] {
        exit = true;
        system("cls");
        screen.Exit();  // Thoát màn hình
    });

    bool selected = false;  // Biến để theo dõi việc chọn phim
    auto select_movie_button = Button("Enter", [&] {
        // Kiểm tra nếu movieId không rỗng và là số
        if (!movieId.empty() && validInteger(movieId)) {
            int id = std::stoi(movieId);  // Chuyển đổi movieId sang số nguyên
            if (checkMovieId(id, movies)) {
                system("cls");
                selected = true;  // Đánh dấu đã chọn phim
                screen.Exit();    // Thoát màn hình
            } else {
                search_message =
                    "Movie not found!";  // Thông báo nếu không tìm thấy phim
            }
        } else {
            search_message =
                "Invalid Movie ID!";  // Thông báo nếu ID không hợp lệ
        }
    });

    // Tạo container cho các thành phần
    auto container = Container::Vertical(Components{
        search_input,
        movie_input,
        select_movie_button,
        exit_button,
        Renderer(
            [&] { return text(search_message) | bold | color(Color::Green); }),
    });

    // Tạo renderer để hiển thị giao diện
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
                   DisplayMovies(movies,
                                 search_query),  // Hiển thị danh sách phim
                   exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
               }) |
               border | center;
    });

    // Tạo và khởi động màn hình
    while (!selected && !exit) {  // Chỉ tiếp tục khi chưa chọn phim
        screen.Loop(renderer);
    }
    if (exit) {
        return -1;
    }
    // Trả về ID phim đã chọn
    return std::stoi(movieId);  // Đây là ID hợp lệ đã chọn
}

int selectShowTime(Vector<Movie> &movies, Vector<ShowTime> &showtimes) {
    Vector<ShowTime> showTimes = getFutureShowTimes(showtimes, movies);
    using namespace ftxui;
    std::string search_query;
    std::string search_message;
    std::string showTimeId;
    auto screen = ScreenInteractive::Fullscreen();
    // Các trường nhập liệu
    auto search_input = Input(&search_query, "Search for movies...");
    auto showtime_input = Input(&showTimeId, "Enter showtime id...");

    bool exit = false;
    // Nút thoát
    auto exit_button = Button("Exit", [&] {
        exit = true;
        system("cls");
        screen.Exit();  // Thoát màn hình
    });

    bool selected = false;  // Biến để theo dõi việc chọn phim
    auto select_showtime_button = Button("Enter", [&] {
        // Kiểm tra nếu movieId không rỗng và là số
        if (!showTimeId.empty() && validInteger(showTimeId)) {
            int id =
                std::stoi(showTimeId);  // Chuyển đổi movieId sang số nguyên
            if (checkShowTimeId(id, showTimes)) {
                system("cls");
                selected = true;            // Đánh dấu đã chọn phim
                screen.Exit();              // Thoát màn hình
            } else {
                search_message =
                    "Showtime not found!";  // Thông báo nếu không tìm thấy phim
            }
        } else {
            search_message =
                "Invalid ShowTime ID!";  // Thông báo nếu ID không hợp lệ
        }
    });

    // Tạo container cho các thành phần
    auto container = Container::Vertical(Components{
        search_input,
        exit_button,
        showtime_input,
        select_showtime_button,
        Renderer(
            [&] { return text(search_message) | bold | color(Color::Green); }),
    });

    // Tạo renderer để hiển thị giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                       center,
                   hbox({showtime_input->Render() | size(WIDTH, EQUAL, 25) |
                             center,
                         select_showtime_button->Render() | center,
                         text(search_message) | center | color(Color::Red)}),
                   DisplayShowTimes(showTimes, movies,
                                    search_query),  // Hiển thị danh sách phim
                   exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
               }) |
               border | center;
    });
    // Tạo và khởi động màn hình

    while (!selected && !exit) {
        screen.Loop(renderer);
    }
    if (exit) {
        return -1;
    }
    return std::stoi(showTimeId);
}
int selectShowTimeCurrentAndFuture(const Vector<Movie>& movies, const Vector<ShowTime>& showtimes) {
    Vector<ShowTime> showTimes = getFutureShowTimes(showtimes, movies);
    using namespace ftxui;
    std::string search_query;
    std::string search_message;
    std::string showTimeId;
    auto screen = ScreenInteractive::Fullscreen();
    // Các trường nhập liệu
    auto search_input = Input(&search_query, "Search for movies...");
    auto showtime_input = Input(&showTimeId, "Enter showtime id...");

    bool exit = false;
    // Nút thoát
    auto exit_button = Button("Exit", [&] {
        exit = true;
        system("cls");
        screen.Exit();  // Thoát màn hình
    });

    bool selected = false;  // Biến để theo dõi việc chọn phim
    auto select_showtime_button = Button("Enter", [&] {
        // Kiểm tra nếu movieId không rỗng và là số
        if (!showTimeId.empty() && validInteger(showTimeId)) {
            int id =
                std::stoi(showTimeId);  // Chuyển đổi movieId sang số nguyên
            if (checkShowTimeId(id, showTimes)) {
                system("cls");
                selected = true;  // Đánh dấu đã chọn phim
                screen.Exit();    // Thoát màn hình
            } else {
                search_message =
                    "Showtime not found!";  // Thông báo nếu không tìm thấy phim
            }
        } else {
            search_message =
                "Invalid ShowTime ID!";  // Thông báo nếu ID không hợp lệ
        }
    });

    // Tạo container cho các thành phần
    auto container = Container::Vertical(Components{
        search_input,
        exit_button,
        showtime_input,
        select_showtime_button,
        Renderer(
            [&] { return text(search_message) | bold | color(Color::Green); }),
    });

    // Tạo renderer để hiển thị giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                       center,
                   hbox({showtime_input->Render() | size(WIDTH, EQUAL, 25) |
                             center,
                         select_showtime_button->Render() | center,
                         text(search_message) | center | color(Color::Red)}),
                   DisplayShowTimes(showTimes, movies,
                                    search_query),  // Hiển thị danh sách phim
                   exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
               }) |
               border | center;
    });
    // Tạo và khởi động màn hình

    while (!selected && !exit) {
        screen.Loop(renderer);
    }
    if (exit) {
        return -1;
    }
    return std::stoi(showTimeId);
}

// int main() {
//     Movies movies(MOVIE_FILE);
//     ShowTimes showTimes(SHOWTIME_FILE);
//     selectMovie(movies.getMovies(), showTimes.getShowTimes());

//     return 0;
// }
