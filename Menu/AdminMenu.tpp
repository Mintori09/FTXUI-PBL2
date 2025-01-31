#include "AdminMenu.h"

ftxui::Element
AdminMenu::displayMonthStatistics(const Vector<Ticket> &tickets) {
    using namespace ftxui;
    Elements month_Statistics;
    Vector<int> months = getLast6Months();
    Vector<int> totals(6);
    for (int i = 0; i < 6; i++) {
        totals[i] = 0;
    }

    totals = {0, 0, 0, 0, 0, 0};

    for (const auto &ticket: tickets) {
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

bool AdminMenu::checkDateByQuarter(const Ticket &ticket, int quarter) {
    int ticketMonth = ticket.getBookingDate().getMonth(); // Lấy tháng của vé
    int ticketYear = ticket.getBookingDate().getYear(); // Lấy năm của vé

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

ftxui::Element
AdminMenu::displayQuarterStatistics(const Vector<Ticket> &tickets,
                                    const Vector<Movie> &movies,
                                    const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
    Elements quarter_Statistics;

    // Các tháng tương ứng với các quý
    Vector<int> quarters = {
        1, 2, 3, 4
    }; // 1: Quý 1 (tháng 1-3), 2: Quý 2 (tháng 4-6), v.v.
    Vector<int> totals(4,
                       0); // Dự trữ giá trị tổng của từng quý, khởi tạo bằng 0

    int year = currentYear(); // Năm hiện tại

    // Duyệt qua danh sách vé để tính tổng cho từng quý
    for (const auto &ticket: tickets) {
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
             separator(),
             text("Total") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 12) |
             hcenter));

    // Thêm thông tin tổng cho mỗi quý
    for (int i = 0; i < quarters.size(); ++i) {
        quarter_Statistics.push_back(separator());
        quarter_Statistics.push_back(hbox(
            text("Q" + std::to_string(quarters[i]) + " " + std::to_string(year)) | size(WIDTH, EQUAL, 10),
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
             text("Year: " + std::to_string(year)) | bold | color(Color::Blue) | center
            );
    result.push_back(
        vbox(vbox(quarter_Statistics) | border | center | size(WIDTH, EQUAL, 50),
             separator(),
             vbox(quarter_graph) | center | size(WIDTH, EQUAL, 50) | border));
    return vbox(result) | center;
}

ftxui::Element
AdminMenu::displayYearStatistics(const Vector<Ticket> &tickets,
                                 const Vector<Movie> &movies,
                                 const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
    Elements year_Statistics;

    // Danh sách các năm cần thống kê, ví dụ: năm hiện tại và năm trước đó
    Vector<int> years = {
        currentYear(), currentYear() - 1, currentYear() - 2,
        currentYear() - 3
    };
    Vector<int> totals(4, 0); // Dự trữ tổng tiền cho từng năm, khởi tạo bằng 0

    // Duyệt qua danh sách vé và tính tổng cho từng năm
    for (const auto &ticket: tickets) {
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

ftxui::Element AdminMenu::displayMonthStatisticsForMovie(const Vector<Ticket> &tickets, const Vector<Movie> &movies, const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
    Elements month_Statistics;
    Map<std::string, int> movieRevenue;

    for (const auto &ticket : tickets) {
        int ticketMonth = ticket.getBookingDate().getMonth();
        int ticketYear = ticket.getBookingDate().getYear();

        // Kiểm tra tháng và thêm doanh thu
        if (ticketMonth == currentMonth() && ticketYear == currentYear()) {
            ShowTime showtime = getShowTimeFromTicket(ticket, showtimes);
            std::string movieName = getMovieTitle(showtime.getMovieId(), movies);
            movieRevenue.insert(movieName, ticket.getTicketPrice());
        }
    }

    // Thêm tiêu đề bảng thống kê
    month_Statistics.push_back(
        hbox(text("index") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 5) | hcenter, separator(), text("Movie") | bold | color(Color::Blue) |
             size(WIDTH, EQUAL, 25) | hcenter,
             separator(),
             text("Total Revenue") | bold | color(Color::Blue) |
             size(WIDTH, EQUAL, 15) | hcenter));
    int index = 1;

    // Duyệt qua các phim và hiển thị doanh thu
    for (auto it = movieRevenue.begin(); it != movieRevenue.end(); ++it) {
        auto [movieName, totalRevenue] = *it; // Structured binding to get the key-value pair
        month_Statistics.push_back(
                 separator());
        month_Statistics.push_back(
            hbox(text(std::to_string(index++)) | color(Color::White) | size(WIDTH, EQUAL, 5), separator(),text(movieName) | color(Color::White) | size(WIDTH, EQUAL, 25),
                 separator(),
                 text(std::to_string(totalRevenue)) | color(Color::White) | size(WIDTH, EQUAL, 15)));
    }

    // Tạo phần thống kê
    Elements result;

    result.push_back(text("Month: " + std::to_string(currentMonth()) + "  Year: " + std::to_string(currentYear())));
    result.push_back(vbox(vbox(month_Statistics) | border | center | size(WIDTH, EQUAL, 50)));
    return vbox(result) | center;
}

ftxui::Element AdminMenu::displayQuarterStatisticsForMovie(const Vector<Ticket> &tickets, const Vector<Movie> &movies, const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
    Elements quarter_Statistics;
    Map<std::string, int> movieRevenue;

    // Define the quarters
    // Quarter 1: Jan, Feb, Mar | Quarter 2: Apr, May, Jun | Quarter 3: Jul, Aug, Sep | Quarter 4: Oct, Nov, Dec
    auto getQuarter = [](int month) -> int {
        if (month >= 1 && month <= 3) return 1;
        if (month >= 4 && month <= 6) return 2;
        if (month >= 7 && month <= 9) return 3;
        return 4; // October, November, December
    };

    // Get the current year and quarter
    int currentQuarter = getQuarter(currentMonth()); // Determine current quarter based on current month

    for (const auto &ticket : tickets) {
        int ticketMonth = ticket.getBookingDate().getMonth();
        int ticketYear = ticket.getBookingDate().getYear();

        // Only consider tickets for the current year and current quarter
        int ticketQuarter = getQuarter(ticketMonth);
        if (ticketYear == currentYear() && ticketQuarter == currentQuarter) {
            ShowTime showtime = getShowTimeFromTicket(ticket, showtimes);
            std::string movieName = getMovieTitle(showtime.getMovieId(), movies);

            // Create a key that combines the movie name and the quarter
            std::string quarterKey = movieName;

            // Insert revenue for this movie in the respective quarter
            movieRevenue.insert(quarterKey, ticket.getTicketPrice());
        }
    }

    // Add table header
    quarter_Statistics.push_back(
        hbox(text("index") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 5) | hcenter, separator(),
             text("Movie - Quarter") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 25) | hcenter,
             separator(), text("Total Revenue") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 15) | hcenter));
    int index = 1;

    // Loop through the movieRevenue map and display revenue for each quarter
    for (auto it = movieRevenue.begin(); it != movieRevenue.end(); ++it) {
        auto [quarterKey, totalRevenue] = *it; // Structured binding to get the key-value pair
        quarter_Statistics.push_back(separator());
        quarter_Statistics.push_back(
            hbox(text(std::to_string(index++)) | color(Color::White) | size(WIDTH, EQUAL, 5), separator(),
                 text(quarterKey) | color(Color::White) | size(WIDTH, EQUAL, 25),
                 separator(),
                 text(std::to_string(totalRevenue)) | color(Color::White) | size(WIDTH, EQUAL, 15)));
    }

    // Create the final statistics element
    Elements result;
    result.push_back(text("Quarter: " + std::to_string(currentQuarter) + "  Year: " + std::to_string(currentYear())));
    result.push_back(vbox(vbox(quarter_Statistics) | border | center | size(WIDTH, EQUAL, 50)));
    return vbox(result) | center;
}

ftxui::Element AdminMenu::displayYearStatisticsForMovie(const Vector<Ticket> &tickets, const Vector<Movie> &movies, const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
    Elements month_Statistics;
    Map<std::string, int> movieRevenue;

    for (const auto &ticket : tickets) {
        int ticketYear = ticket.getBookingDate().getYear();

        // Kiểm tra tháng và thêm doanh thu
        if (ticketYear == currentYear()) {
            ShowTime showtime = getShowTimeFromTicket(ticket, showtimes);
            std::string movieName = getMovieTitle(showtime.getMovieId(), movies);
            movieRevenue.insert(movieName, ticket.getTicketPrice());
        }
    }

    // Thêm tiêu đề bảng thống kê
    month_Statistics.push_back(
        hbox(text("index") | bold | color(Color::Blue) | size(WIDTH, EQUAL, 5) | hcenter, separator(), text("Movie") | bold | color(Color::Blue) |
             size(WIDTH, EQUAL, 25) | hcenter,
             separator(),
             text("Total Revenue") | bold | color(Color::Blue) |
             size(WIDTH, EQUAL, 15) | hcenter));
    int index = 1;

    // Duyệt qua các phim và hiển thị doanh thu
    for (auto it = movieRevenue.begin(); it != movieRevenue.end(); ++it) {
        auto [movieName, totalRevenue] = *it; // Structured binding to get the key-value pair
        month_Statistics.push_back(
                 separator());
        month_Statistics.push_back(
            hbox(text(std::to_string(index++)) | color(Color::White) | size(WIDTH, EQUAL, 5), separator(),text(movieName) | color(Color::White) | size(WIDTH, EQUAL, 25),
                 separator(),
                 text(std::to_string(totalRevenue)) | color(Color::White) | size(WIDTH, EQUAL, 15)));
    }

    // Tạo phần thống kê
    Elements result;

    result.push_back(text("Year: " + std::to_string(currentYear())));
    result.push_back(vbox(vbox(month_Statistics) | border | center | size(WIDTH, EQUAL, 50)));
    return vbox(result) | center;
}
ftxui::Element AdminMenu::DisplayStatistics(const Vector<Ticket> &tickets,
                                            const Vector<Movie> &movies,
                                            const Vector<ShowTime> &showtimes,
                                            int Check) {
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

ftxui::Element AdminMenu::DisplayStatisticsForMovie(const Vector<Ticket> &tickets,
                                            const Vector<Movie> &movies,
                                            const Vector<ShowTime> &showtimes,
                                            int Check) {
    switch (Check) {
        case 1:
            // Logic for displaying statistics for the month
            return displayMonthStatisticsForMovie(tickets, movies, showtimes);
        case 2:
            // Logic for displaying statistics for the quarter
            return displayQuarterStatisticsForMovie(tickets, movies, showtimes);
        case 3:
            // Logic for displaying statistics for the year
            return displayYearStatisticsForMovie(tickets, movies, showtimes);
        default:
            return displayMonthStatisticsForMovie(tickets, movies, showtimes);
    }
}

void AdminMenu::Statistic(const Vector<Ticket> &tickets,
                          const Vector<Movie> &movies,
                          const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
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
                   vbox({
                       text("Statistics") | center | bold, separator(),
                       statistics_month->Render(), statistics_quarter->Render(),
                       statistics_year->Render(),
                       separator(),
                       back_button->Render()

                   }) |
                   border | center |
                   size(WIDTH, EQUAL, 40), // Style for the left panel

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

void AdminMenu::StatisticForMovie(const Vector<Ticket> &tickets,
                          const Vector<Movie> &movies,
                          const Vector<ShowTime> &showtimes) {
    using namespace ftxui;
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
                   vbox({
                       text("Statistics") | center | bold, separator(),
                       statistics_month->Render(), statistics_quarter->Render(),
                       statistics_year->Render(),
                       separator(),
                       back_button->Render()

                   }) |
                   border | center |
                   size(WIDTH, EQUAL, 40), // Style for the left panel

                   separator(),

                   // Right-side panel with statistics display
                   DisplayStatisticsForMovie(tickets, movies, showtimes,
                                     Check) |
                   center // Pass 'Check' to DisplayStatistics
               }) |
               center;
    });

    screen.Loop(renderer);
}
void AdminMenu::StatisticMenu(const Vector<Ticket> &tickets, const Vector<Movie> &movies,
                     const Vector<ShowTime> &showtimes){
    using namespace ftxui;    
    auto screen = ScreenInteractive::TerminalOutput();
    auto statistic_button = Button("Statistic Money", [&] {
            Statistic(tickets, movies, showtimes);
            });
    auto statistic_for_movie_button = Button("Statistic Movie", [&] {
            StatisticForMovie(tickets, movies, showtimes);
            });

    auto exit_button = Button("Exit", [&] { screen.Exit(); });

    auto container = Container::Vertical(Components{
        statistic_button,
        exit_button,
        statistic_for_movie_button
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   statistic_button->Render() | 
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   statistic_for_movie_button->Render() | 
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   exit_button->Render() | 
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   // exit_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   // center,

               }) |
               border | center | hcenter;
    });

    screen.Loop(renderer);
}



void AdminMenu::run(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
                    Tickets &tickets, Account &account) {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    // Tạo các nút cho từng tính năng
    auto book_ticket_button = Button("Booking Ticket", [&] {
        BookingTicket(movies.getMovies(), showtimes.getShowTimes(), account,
                      tickets);
    });
    auto show_movies_button = Button("Add Movie", [&] { addMovie(movies); });
    auto show_showtimes_button =
            Button("Add ShowTime", [&] { addShowTime(movies, showtimes); });
    auto show_booked_tickets_button = Button("Display Tickets History", [&] {
        showTicket(accounts.getAccounts(), tickets.getTickets(), movies.getMovies(),
                   showtimes.getShowTimes());
    });
    // auto update_account_button = Button("Update Account", [&] {});
    auto statistic_button = Button("Statistics", [&] {
        StatisticMenu(tickets.getTickets(), movies.getMovies(),
                  showtimes.getShowTimes());
    });
    auto logout_button = Button("Log out", [&] { screen.Exit(); });

    // Container chứa các nút
    auto container = Container::Vertical({
        book_ticket_button,
        show_movies_button,
        show_showtimes_button,
        show_booked_tickets_button,
        // update_account_button,
        statistic_button,
        logout_button,
    });

    // Renderer cho giao diện
    auto renderer = ftxui::Renderer(container, [&] {
        return vbox({
                   text("Admin Menu") | bold | center,
                   separator(),
                   book_ticket_button->Render() | size(WIDTH, EQUAL, 50) | center,
                   show_movies_button->Render() | size(WIDTH, EQUAL, 50) | center,
                   show_showtimes_button->Render() | size(WIDTH, EQUAL, 50) |
                   center,
                   show_booked_tickets_button->Render() | size(WIDTH, EQUAL, 50) |
                   center,
                   //    update_account_button->Render() | center |
                   //    size(WIDTH, EQUAL, 50),
                   statistic_button->Render() | size(WIDTH, EQUAL, 50) | center,
                   logout_button->Render() | size(WIDTH, EQUAL, 50) | center,
               }) |
               border | size(WIDTH, EQUAL, 100) | center;
    });

    screen.Loop(renderer);
}
