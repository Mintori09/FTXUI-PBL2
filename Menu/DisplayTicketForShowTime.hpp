#include <ftxui/component/component.hpp>       // for Input, Renderer, Vertical
#include <ftxui/component/component_base.hpp>  // for ComponentBase
#include <ftxui/component/component_options.hpp>  // for InputOption
#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp>  // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/screen/color.hpp>
#include <ftxui/util/ref.hpp>  // for Ref
#include <string>

#include "../include/Accounts.h"
#include "../include/Movies.h"
#include "../include/ShowTimes.h"
#include "../include/Ticket.h"
#include "../include/Tickets.h"
#include "../include/Vector.h"
#include "../include/my_lib.h"
#include "DisplayTable.hpp"

std::string getUserName(const Ticket& ticket, const Vector<Account> accounts) {
    for (const auto& account : accounts) {
        if (account.getUserId() == ticket.getUserId()) {
            return account.getUserName();
        }
    }
    return "";
}
int getMovieId(const Ticket& ticket, const Vector<Movie> movies,
               const Vector<ShowTime> showtimes) {
    for (const auto& showtime : showtimes) {
        if (showtime.getShowTimeId() == ticket.getShowTimeId()) {
            int movieId = showtime.getMovieId();
            for (const auto& movie : movies) {
                if (movie.getMovieId() == movieId) {
                    return movie.getMovieId();
                }
            }
        }
    }
    return -1;
}
ShowTime getShowTime(int showTimeId, const Vector<ShowTime>& showtimes) {
    for (const auto& showtime : showtimes) {
        if (showtime.getShowTimeId() == showTimeId) {
            return showtime;
        }
    }
    return ShowTime();
}
ftxui::Element DisplayTicketsAll(const Vector<Account> accounts,
                                 const Vector<Ticket> tickets,
                                 const ShowTime& showtime,
                                 const Vector<Movie>& movies,
                                 const std::string& search_query) {
    std::cin.ignore();
    using namespace ftxui;
    Elements showtime_rows;

    // Thêm tiêu đề bảng với màu xanh và phân cách giữa các cột
    showtime_rows.push_back(hbox(text("Ticket Id") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 15) | hcenter,
                                 separator(),
                                 text("Username") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 20) | hcenter,
                                 separator(),
                                 text("Booking Date") | bold |
                                     color(Color::Blue) |
                                     size(WIDTH, EQUAL, 15) | hcenter,
                                 separator(),
                                 text("Show Date") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 15) | hcenter,
                                 separator(),
                                 text("Seat") | bold | color(Color::Blue) |
                                     size(WIDTH, EQUAL, 10) | hcenter));

    // Thêm hàng cho từng ShowTime với phân cách giữa các hàng
    for (const auto& ticket : tickets) {
        std::string UserName = getUserName(ticket, accounts);

        // Kiểm tra nếu tiêu đề phim phù hợp với chuỗi tìm kiếm
        if (search_query.empty() ||
            isAllWordsInString(search_query, UserName)) {
            showtime_rows.push_back(separator());
            showtime_rows.push_back(
                hbox(text(std::to_string(ticket.getTicketId())) |
                         size(WIDTH, EQUAL, 15),
                     separator(), paragraph(UserName) | size(WIDTH, EQUAL, 20),
                     separator(),
                     text(ticket.getBookingDate().toString()) |
                         size(WIDTH, EQUAL, 15),
                     separator(),
                     text(showtime.getShowDate().toString()) |
                         size(WIDTH, EQUAL, 15),
                     separator(),
                     text(std::to_string(ticket.getSeatNumber())) |
                         size(WIDTH, EQUAL, 10)));
        }
    }

    return vbox(showtime_rows) | border | center;
}
void ShowTicketInfo(const Vector<Account> accounts,
                    const Vector<Ticket>& tickets, const Vector<Movie>& movies,
                    const ShowTime& showtime) {
    using namespace ftxui;
    std::string search_query;
    std::string search_message;
    auto screen = ScreenInteractive::Fullscreen();

    // Các trường nhập liệu
    auto search_input = Input(&search_query, "Search for movies...");
    bool exit = false;
    // Nút thoát
    auto exit_button = Button("Exit", [&] {
        exit = true;
        system("cls");
        screen.Exit();  // Thoát màn hình
    });

    bool selected = false;  // Biến để theo dõi việc chọn phim

    // Tạo container cho các thành phần
    auto container = Container::Vertical(Components{
        search_input,
        exit_button,
        Renderer(
            [&] { return text(search_message) | bold | color(Color::Green); }),
    });

    // Tạo renderer để hiển thị giao diện
    auto renderer = Renderer(container, [&] {
        return vbox({
                   search_input->Render() | border | size(WIDTH, EQUAL, 50) |
                       center,

                   DisplayTicketsAll(accounts, tickets, showtime, movies,
                                     search_query),  // Hiển thị danh sách phim
                   exit_button->Render() | size(WIDTH, EQUAL, 25) | center,
               }) |
               border | center;
    });

    // Tạo và khởi động màn hình
    while (!selected && !exit) {  // Chỉ tiếp tục khi chưa chọn phim
        screen.Loop(renderer);
    }
    // Trả về ID phim đã chọn
    return;
}
void showTicket(const Vector<Account> accounts, const Vector<Ticket>& tickets,
                const Vector<Movie>& movies,
                const Vector<ShowTime>& showtimes) {
    while (true) {
        int select = selectShowTimeCurrentAndFuture(movies, showtimes);
        if (select == -1) {
            return;
        }
        ShowTicketInfo(accounts, tickets, movies,
                       getShowTime(select, showtimes));
    }
}

int main() {
    Movies movies(MOVIE_FILE);
    ShowTimes showTimes(SHOWTIME_FILE);
    Tickets tickets(TICKET_FILE);
    Accounts accounts(ACCOUNT_FILE);
    showTicket(accounts.getAccounts(), tickets.getTickets(), movies.getMovies(),
               showTimes.getShowTimes());
}