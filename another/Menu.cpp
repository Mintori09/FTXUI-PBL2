#include "..\include\Menu.h"

#include <cstdlib>
#include <memory>
#include <string>
void deleteShowTime(ShowTimes &showTimes, Movies movies) {
    Date today(
        currentDate());  // Giả sử currentDate() trả về định dạng "dd-mm-yyyy"

    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    Time currentTime(localTime->tm_hour, localTime->tm_min);

    std::cout << "Future Show Times:\n";
    bool found = false;
    Vector<ShowTime> times;
    for (const auto &showtime : showTimes.getShowTimes()) {
        Date showDate(showtime.getShowDate());

        // Kiểm tra xem có phải là lịch chiếu tương lai không
        if (showDate > today ||
            (showDate == today && showtime.getStartTime() > currentTime)) {
            // Nếu movieId không được chỉ định hoặc trùng với showtime.movieId
            bool movieFound = false;
            for (const auto &movie : movies.getMovies()) {
                if (movie.getMovieId() == showtime.getMovieId()) {
                    std::cout
                        << "Movie: " << movie.getTitle() << "\t"
                        << "Showtime ID: " << showtime.getShowTimeId() << "\t"
                        << "Show Date: " << showtime.getShowDate().day << "-"
                        << showtime.getShowDate().month << "-"
                        << showtime.getShowDate().year << "\t"
                        << "Start Time: " << showtime.getStartTime().hour << ":"
                        << (showtime.getStartTime().minute < 10 ? "0" : "")
                        << showtime.getStartTime().minute << "\n";
                    times.push_back(showtime);
                    found = true;
                    movieFound = true;
                    break;  // Không cần kiểm tra thêm các phim khác
                }
            }
        }
    }

    if (!times.empty()) {
        std::cout << "Choose a showtime ID to delete: ";
        int showTimeId;
        std::cin >> showTimeId;

        // Kiểm tra xem showTimeId có trong danh sách không
        for (const auto &showtime : times) {
            if (showtime.getShowTimeId() == showTimeId) {
                // Xác nhận xóa
                char confirm;
                std::cout << "Are you sure you want to delete ShowTime ID "
                          << showTimeId << "? (y/n): ";
                std::cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    showTimes.deleteShowTime(showTimeId);
                    std::cout << "ShowTime ID " << showTimeId
                              << " has been deleted.\n";
                } else {
                    std::cout << "Deletion cancelled.\n";
                }
                return;
            }
        }
    }

    if (!found) {
        std::cout << "No future showtimes available." << std::endl;
    }
}

void updateShowTime(ShowTimes &showTimes, Movies &movies) {
    Date today(
        currentDate());  // Giả sử currentDate() trả về định dạng "dd-mm-yyyy"

    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    Time currentTime(localTime->tm_hour, localTime->tm_min);

    std::cout << "Future Show Times:\n";
    bool found = false;
    Vector<ShowTime> times;
    for (const auto &showtime : showTimes.getShowTimes()) {
        Date showDate(showtime.getShowDate());

        // Kiểm tra xem có phải là lịch chiếu tương lai không
        if (showDate > today ||
            (showDate == today && showtime.getStartTime() > currentTime)) {
            // Nếu movieId không được chỉ định hoặc trùng với showtime.movieId
            bool movieFound = false;
            for (const auto &movie : movies.getMovies()) {
                if (movie.getMovieId() == showtime.getMovieId()) {
                    std::cout
                        << "Movie: " << movie.getTitle() << "\t"
                        << "Showtime ID: " << showtime.getShowTimeId() << "\t"
                        << "Show Date: " << showtime.getShowDate() << "\t"
                        << "Start Time: " << showtime.getStartTime() << "\n";
                    times.push_back(showtime);
                    found = true;
                    movieFound = true;
                    break;  // Không cần kiểm tra thêm các phim khác
                }
            }
        }
    }
    if (!times.empty()) {
        std::cout << "Choose a showtime ID to update: ";
        int showTimeId;
        std::cin >> showTimeId;
        for (const auto &showtime : times) {
            if (showtime.getShowTimeId() == showTimeId) {
                showTimes.updateShowTime(showTimeId);
                return;
            }
        }

        if (!found) {
            std::cout << "No future showtimes available." << std::endl;
        }
    }
}
void addShowTime(Movies movies, ShowTimes showTimes) {
    int movieId;
    std::cout << "Available Movies:\n";

    // Display the available movies (for example purposes)

    // Display the movies
    for (const auto &movie : movies.getMovies()) {
        movie.display();
    }

    std::cout << "Enter Movie ID to add ShowTime: ";
    std::cin >> movieId;

    // Check if movieId is valid
    bool movieExists = false;
    for (const auto &movie : movies.getMovies()) {
        if (movie.getMovieId() == movieId) {
            movieExists = true;
            break;
        }
    }

    if (!movieExists) {
        std::cout << "Invalid Movie ID.\n";
        return;
    }

    // Proceed to gather showtime details
    int showTimeId = (showTimes.getShowTimes().empty())
                         ? 1
                         : showTimes.getShowTimes().back().getShowTimeId() +
                               1;  // Auto-increment ID
    int hallId;
    Date showDate;
    Time startTime, endTime;

    std::cout << "Enter Hall ID: ";
    std::cin >> hallId;

    // Enter show date in dd-mm-yyyy format
    std::cout << "Enter Show Date (dd-mm-yyyy): ";
    std::cin >> showDate;

    // Enter start time in hh:mm format
    std::cout << "Enter Start Time (hh:mm): ";
    std::cin >> startTime;

    // Enter end time in hh:mm format
    std::cout << "Enter End Time (hh:mm): ";
    std::cin >> endTime;

    ShowTime newShowTime(showTimeId, movieId, hallId, showDate, startTime,
                         endTime);
    if (showTimes.addShowTime(newShowTime)) {
        std::cout << "ShowTime added successfully with ID: " << showTimeId
                  << "\n";
    }
}
void showTicketHistory(const Tickets &tickets, int userId) {
    std::cout << "Ticket History for User ID: " << userId << "\n";

    // Lấy danh sách vé đã đặt
    Vector<Ticket> userTickets =
        tickets.getTickets();  // Giả sử có hàm getTickets() trả về danh sách vé

    bool hasTickets = false;  // Biến kiểm tra có vé hay không

    for (const auto &ticket : userTickets) {
        if (ticket.getUserId() ==
            userId) {  // Kiểm tra vé thuộc về userId đã cho
            hasTickets = true;
            std::cout << "Ticket ID: " << ticket.getTicketId() << "\n"
                      << "Showtime ID: " << ticket.getShowTimeId() << "\n"
                      << "Seat Number: " << ticket.getSeatNumber() << "\n"
                      << "Booking Date: " << ticket.getBookingDate()
                      << "\n"  // Giả sử Date có operator<< overload
                      << "Price: " << ticket.getTicketPrice() << "\n"
                      << "Payment Method: " << ticket.getPaymentMethod() << "\n"
                      << "------------------------------------\n";
        }
    }

    if (!hasTickets) {
        std::cout << "No tickets found for this user.\n";
    }
}

int generateTicketId(const Vector<Ticket> &tickets) {
    if (tickets.empty()) {
        return 1;  // Nếu không có vé nào, bắt đầu từ 1
    }
    // Lấy ID lớn nhất hiện tại và cộng thêm 1
    int maxId = 0;
    for (const auto &ticket : tickets) {
        if (ticket.getTicketId() > maxId) {
            maxId = ticket.getTicketId();
        }
    }
    return maxId + 1;
}

void displayFutureShowtimes(const Vector<ShowTime> &showTimes,
                            const Vector<Movie> &movies, int movieId) {
    Date today(
        currentDate());  // Giả sử currentDate() trả về định dạng "dd-mm-yyyy"

    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    Time currentTime(localTime->tm_hour, localTime->tm_min);

    std::cout << "Future Show Times:\n";
    bool found = false;

    for (const auto &showtime : showTimes) {
        Date showDate(showtime.getShowDate());

        // Kiểm tra xem có phải là lịch chiếu tương lai không
        if (showDate > today ||
            (showDate == today && showtime.getStartTime() > currentTime)) {
            // Nếu movieId không được chỉ định hoặc trùng với showtime.movieId
            if (movieId == -1 || showtime.getMovieId() == movieId) {
                // Tìm kiếm thông tin phim tương ứng
                bool movieFound = false;
                for (const auto &movie : movies) {
                    if (movie.getMovieId() == showtime.getMovieId()) {
                        std::cout << "Movie: " << movie.getTitle() << "\t"
                                  << "Showtime ID: " << showtime.getShowTimeId()
                                  << "\t"
                                  << "Show Date: " << showtime.getShowDate()
                                  << "\t"
                                  << "Start Time: " << showtime.getStartTime()
                                  << "\n";
                        found = true;
                        movieFound = true;
                        break;  // Không cần kiểm tra thêm các phim khác
                    }
                }
            }
        }
    }

    if (!found) {
        std::cout << "No future showtimes available." << std::endl;
    }
}

void bookTicketByShowtime(const Vector<ShowTime> &showTimes, Tickets &tickets,
                          int userId) {
    Date today(
        currentDate());  // Giả sử currentDate() trả về định dạng "dd-mm-yyyy"

    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    Time currentTime(localTime->tm_hour, localTime->tm_min);

    std::cout << "Available Show Times:\n";
    bool found = false;

    // Danh sách các showtimeId có sẵn
    Vector<int> availableShowTimeIds;

    for (const auto &showtime : showTimes) {
        Date showDate(showtime.getShowDate());

        // So sánh ngày và thời gian
        if (showDate > today ||
            (showDate == today && showtime.getStartTime() > currentTime)) {
            std::cout << "Showtime ID: " << showtime.getShowTimeId()
                      << ", Show Date: " << showDate
                      << ", Start Time: " << showtime.getStartTime().hour << ":"
                      << showtime.getStartTime().minute << "\n";
            availableShowTimeIds.push_back(
                showtime.getShowTimeId());  // Thêm showtimeId vào danh sách
            found = true;
        }
    }

    if (!found) {
        std::cout << "No available showtimes at the moment." << std::endl;
        return;  // Thoát hàm nếu không có lịch chiếu nào có sẵn
    }

    // Cho phép người dùng chọn một showtime bằng showtimeId
    int selectedShowTimeId;
    std::cout << "Enter showtimeId to select a showtime: ";
    std::cin >> selectedShowTimeId;

    // Kiểm tra tính hợp lệ của showtimeId
    bool isValidShowTimeId = false;
    for (const auto &id : availableShowTimeIds) {
        if (id == selectedShowTimeId) {
            isValidShowTimeId = true;
            break;
        }
    }

    if (!isValidShowTimeId) {
        std::cout << "Invalid showtimeId. Please try again." << std::endl;
        return;
    }

    // Hiển thị các ghế có sẵn
    const int totalSeats = 10;                         // Tổng số ghế
    std::vector<bool> seatsBooked(totalSeats, false);  // Khởi tạo ghế chưa đặt

    // Đánh dấu ghế đã đặt từ vé đã có
    for (const auto &ticket : tickets.getTickets()) {
        if (ticket.getShowTimeId() == selectedShowTimeId) {
            seatsBooked[ticket.getSeatNumber() - 1] =
                true;  // Giả sử số ghế bắt đầu từ 1
        }
    }

    std::cout << "Available seats:\n";
    for (int i = 1; i <= totalSeats; ++i) {
        if (!seatsBooked[i - 1]) {
            std::cout << "Seat " << i << " is available.\n";
        }
    }

    // Tiến hành đặt vé
    std::vector<short> selectedSeats;
    char continueBooking;
    do {
        short seatNumber;
        std::cout << "Enter seat number to book: ";
        std::cin >> seatNumber;

        if (seatNumber < 1 || seatNumber > totalSeats) {
            std::cout << "Invalid seat number. Please try again." << std::endl;
            continue;
        }

        if (seatsBooked[seatNumber - 1]) {
            std::cout << "Seat " << seatNumber
                      << " is already booked. Please choose another seat."
                      << std::endl;
            continue;
        }

        selectedSeats.push_back(seatNumber);
        seatsBooked[seatNumber - 1] = true;  // Đánh dấu ghế đã đặt
        std::cout << "Seat " << seatNumber << " has been successfully booked."
                  << std::endl;
        std::cout << "Do you want to book another seat? (Y/N): ";
        std::cin >> continueBooking;

    } while (continueBooking == 'Y' || continueBooking == 'y');

    // Nhập phương thức thanh toán
    std::string paymentMethod;
    std::cout << "Enter payment method (e.g., credit, cash): ";
    std::cin >> paymentMethod;

    // Lấy ngày đặt vé (ngày hiện tại)
    Date bookingDate = today;

    // Giá vé
    int ticketPrice = 50000;                              // Giá mỗi ghế
    int totalPrice = ticketPrice * selectedSeats.size();  // Tính tổng tiền

    // Hiển thị thông tin đặt vé cho khách hàng xác nhận
    std::cout << "\nPlease confirm your booking:\n";
    std::cout << "Showtime ID: " << selectedShowTimeId << "\n";
    std::cout << "Seats booked: ";
    for (const auto &seat : selectedSeats) {
        std::cout << seat << " ";
    }
    std::cout << "\nTotal Tickets: " << selectedSeats.size() << "\n";
    std::cout << "Total Price: " << totalPrice << "\n";

    char confirm;
    std::cout << "Do you confirm this booking? (Y/N): ";
    std::cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        // Tạo và lưu vé cho mỗi ghế đã đặt
        for (const auto &seat : selectedSeats) {
            int ticketId = tickets.endOfTicketId() + 1;
            Ticket newTicket(ticketId, selectedShowTimeId, userId, seat,
                             ticketPrice, bookingDate, paymentMethod);
            tickets.addTicket(newTicket);
        }

        std::cout << "Your tickets have been successfully booked!\n";
        std::cout << "Ticket Details:\n";

        // Hiển thị thông tin vé đã đặt
        for (const auto &seat : selectedSeats) {
            int ticketId =
                tickets.endOfTicketId() + 1;  // Giả sử có hàm tạo ticketId
            std::cout << "Ticket ID: " << ticketId
                      << ", Showtime ID: " << selectedShowTimeId
                      << ", Seat Number: " << seat << ", Price: " << ticketPrice
                      << ", Payment Method: " << paymentMethod << "\n";
        }
    } else {
        std::cout << "Booking cancelled.\n";
    }
}

void displayFutureMovies(const Vector<Movie> movies,
                         const Vector<ShowTime> showTimes, Tickets &tickets) {
    Date today(
        currentDate());  // Giả sử currentDate() trả về định dạng "dd-mm-yyyy"

    // Lấy thời gian hiện tại
    std::time_t now = std::time(nullptr);
    std::tm *localTime = std::localtime(&now);
    Time currentTime(localTime->tm_hour, localTime->tm_min);

    std::cout << "Movies with Future Show Times:\n";
    bool found = false;

    // Danh sách các movieIds có sẵn
    Vector<int> availableMovieIds;

    for (const auto &movie : movies) {
        for (const auto &showtime : showTimes) {
            if (showtime.getMovieId() == movie.getMovieId()) {
                Date showDate(showtime.getShowDate());

                // So sánh ngày và thời gian
                if (showDate > today ||
                    (showDate == today &&
                     showtime.getStartTime() > currentTime)) {
                    movie.display();  // Hiển thị thông tin phim
                    availableMovieIds.push_back(
                        movie.getMovieId());  // Thêm movieId vào danh sách
                    found = true;
                    break;  // Không cần kiểm tra thêm showtime cho bộ phim này
                }
            }
        }
    }

    if (!found) {
        std::cout << "No movies with future showtimes available." << std::endl;
        return;  // Thoát hàm nếu không có phim nào có sẵn
    }

    // Cho phép người dùng chọn một bộ phim bằng movieId
    int selectedMovieId;
    std::cout << "Enter movieId to select a movie: ";
    std::cin >> selectedMovieId;

    // Kiểm tra xem movieId có hợp lệ không
    bool isValidMovieId = false;
    for (const auto &id : availableMovieIds) {
        if (id == selectedMovieId) {
            isValidMovieId = true;
            break;
        }
    }

    if (!isValidMovieId) {
        std::cout << "Invalid movieId. Please try again." << std::endl;
        return;
    }

    // Hiển thị các lịch chiếu cho bộ phim đã chọn
    std::cout << "Available Show Times for Movie ID " << selectedMovieId
              << ":\n";
    Vector<int> availableShowTimeIds;
    for (const auto &showtime : showTimes) {
        if (showtime.getMovieId() == selectedMovieId) {
            Date showDate(showtime.getShowDate());

            // So sánh để kiểm tra lịch chiếu tương lai
            if (showDate > today ||
                (showDate == today && showtime.getStartTime() > currentTime)) {
                std::cout << "Showtime ID: " << showtime.getShowTimeId()
                          << ", Show Date: " << showDate
                          << ", Start Time: " << showtime.getStartTime().hour
                          << ":" << showtime.getStartTime().minute << "\n";
                availableShowTimeIds.push_back(showtime.getShowTimeId());
            }
        }
    }

    if (availableShowTimeIds.empty()) {
        std::cout << "No available showtimes for this movie." << std::endl;
        return;
    }

    // Cho phép người dùng chọn một showtime
    int selectedShowTimeId;
    std::cout << "Enter showtimeId to select a showtime: ";
    std::cin >> selectedShowTimeId;

    // Kiểm tra tính hợp lệ của showtimeId
    bool isValidShowTimeId = false;
    for (const auto &id : availableShowTimeIds) {
        if (id == selectedShowTimeId) {
            isValidShowTimeId = true;
            break;
        }
    }

    if (!isValidShowTimeId) {
        std::cout << "Invalid showtimeId. Please try again." << std::endl;
        return;
    }

    // Hiển thị các ghế có sẵn
    const int totalSeats = 10;                         // Tổng số ghế
    std::vector<bool> seatsBooked(totalSeats, false);  // Khởi tạo ghế chưa đặt

    // Đánh dấu ghế đã đặt từ vé đã có
    for (const auto &ticket : tickets.getTickets()) {
        if (ticket.getShowTimeId() == selectedShowTimeId) {
            seatsBooked[ticket.getSeatNumber() - 1] =
                true;  // Giả sử số ghế bắt đầu từ 1
        }
    }

    std::cout << "Available seats:\n";
    for (int i = 1; i <= totalSeats; ++i) {
        if (!seatsBooked[i - 1]) {
            std::cout << "Seat " << i << " is available.\n";
        }
    }

    // Tiến hành đặt vé
    std::vector<short> selectedSeats;
    char continueBooking;
    do {
        short seatNumber;
        std::cout << "Enter seat number to book: ";
        std::cin >> seatNumber;

        if (seatNumber < 1 || seatNumber > totalSeats) {
            std::cout << "Invalid seat number. Please try again." << std::endl;
            continue;
        }

        if (seatsBooked[seatNumber - 1]) {
            std::cout << "Seat " << seatNumber
                      << " is already booked. Please choose another seat."
                      << std::endl;
            continue;
        }

        selectedSeats.push_back(seatNumber);
        seatsBooked[seatNumber - 1] = true;  // Đánh dấu ghế đã đặt
        std::cout << "Seat " << seatNumber << " has been successfully booked."
                  << std::endl;
        std::cout << "Do you want to book another seat? (Y/N): ";
        std::cin >> continueBooking;

    } while (continueBooking == 'Y' || continueBooking == 'y');

    // Nhập phương thức thanh toán
    std::string paymentMethod;
    std::cout << "Enter payment method (e.g., credit, cash): ";
    std::cin >> paymentMethod;

    // Lấy ngày đặt vé (ngày hiện tại)
    Date bookingDate = today;

    // Giá vé
    int ticketPrice = 50000;                              // Giá mỗi ghế
    int totalPrice = ticketPrice * selectedSeats.size();  // Tính tổng tiền

    // Hiển thị thông tin đặt vé cho khách hàng xác nhận
    std::cout << "\nPlease confirm your booking:\n";
    std::cout << "Movie ID: " << selectedMovieId << "\n";
    std::cout << "Showtime ID: " << selectedShowTimeId << "\n";
    std::cout << "Seats booked: ";
    for (const auto &seat : selectedSeats) {
        std::cout << seat << " ";
    }
    std::cout << "\nTotal Tickets: " << selectedSeats.size() << "\n";
    std::cout << "Total Price: " << totalPrice << "\n";

    char confirm;
    std::cout << "Do you confirm this booking? (Y/N): ";
    std::cin >> confirm;

    if (confirm == 'Y' || confirm == 'y') {
        // Tạo và lưu vé cho mỗi ghế đã đặt
        for (const auto &seat : selectedSeats) {
            std::cout << "Are you sure you want to";
            int ticketId = tickets.endOfTicketId() + 1 + 1;
            std::cout << "Are you sure you want to";
            Ticket newTicket(ticketId, selectedShowTimeId, 1, seat, ticketPrice,
                             bookingDate, paymentMethod);  // Giả sử userId = 1
            tickets.addTicket(newTicket);
        }

        std::cout << "Your tickets have been successfully booked!\n";
        std::cout << "Ticket Details:\n";

        // Hiển thị thông tin vé đã đặt
        for (const auto &seat : selectedSeats) {
            int ticketId =
                tickets.endOfTicketId() + 1 + 1;  // Giả sử có hàm tạo ticketId
            std::cout << "Ticket ID: " << ticketId
                      << ", Showtime ID: " << selectedShowTimeId
                      << ", Seat Number: " << seat << ", Price: " << ticketPrice
                      << ", Payment Method: " << paymentMethod << "\n";
        }
    } else {
        std::cout << "Booking cancelled.\n";
    }
}

MenuItem::MenuItem(const std::string &id, const std::string &name)
    : identifier(id), name(name), expanded(false) {}

void MenuItem::addSubItem(std::shared_ptr<MenuItem> subItem) {
    subItems.push_back(subItem);
}

void MenuItem::toggleExpanded() { expanded = !expanded; }
bool MenuItem::isLeaf() const { return subItems.empty(); }

const std::string &MenuItem::getIdentifier() const { return identifier; }

const std::string &MenuItem::getName() const { return name; }

bool MenuItem::isExpanded() const { return expanded; }

void MenuItem::display(const std::string &highlight, int indent) const {
    std::string indentStr(indent * 4, ' ');
    std::string highlightStr(indent * 4, ' ');
    highlightStr.insert(2, "*");

    if (highlight == identifier) {
        std::cout << highlightStr << identifier << ". " << name << std::endl;
    } else {
        std::cout << indentStr << identifier << ". " << name << std::endl;
    }

    if (expanded) {
        for (const auto &subItem : subItems) {
            subItem->display(highlight, indent + 1);
        }
    }
}

void MenuItem::getIdentifiers(Vector<std::string> &input) {
    input.push_back(identifier);
    if (expanded) {
        for (const auto &subItem : subItems) {
            subItem->getIdentifiers(input);
        }
    }
}

MenuItem *MenuItem::findItem(const std::string &id) {
    if (identifier == id) {
        return this;
    }
    for (auto &subItem : subItems) {
        MenuItem *result = subItem->findItem(id);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

void Menu::addItem(std::shared_ptr<MenuItem> item) { items.push_back(item); }

void Menu::display(const std::string &highlight) const {
    for (const auto &item : items) {
        item->display(highlight, 1);
    }
}

void Menu::getIdentifiers(Vector<std::string> &input) {
    for (auto &item : items) {
        item->getIdentifiers(input);
    }
}

float Menu::run() {
    std::string highlight;
    int index = 0;
    int prevIndex = -1;
    bool needsRedraw = true;

    while (true) {
        Vector<std::string> highlights;
        getIdentifiers(highlights);

        if (!highlights.empty()) {
            highlight = highlights[index];
        }

        if (needsRedraw || index != prevIndex) {
            system("cls");
            display(highlight);
            needsRedraw = false;
            prevIndex = index;
        }

        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            index = (index > 0) ? index - 1 : highlights.size() - 1;
            needsRedraw = true;
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            index = (index + 1) % highlights.size();
            needsRedraw = true;
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
            MenuItem *item = findItem(highlight);
            if (item) {
                if (!item->isLeaf()) {
                    item->toggleExpanded();
                    needsRedraw = true;
                } else {
                    std::cout << "You have selected " << item->getIdentifier()
                              << ". " << item->getName() << std::endl;
                    return std::stof(item->getIdentifier());
                }
            }
            Sleep(150);
        }
    }

    return 0;
}

MenuItem *Menu::findItem(const std::string &id) {
    for (auto &item : items) {
        MenuItem *result = item->findItem(id);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

std::shared_ptr<Menu> buildMenuAdmin() {
    auto menu = std::make_shared<Menu>();

    auto item1 = std::make_shared<MenuItem>("1", "Booking Ticket");

    auto item2 = std::make_shared<MenuItem>("2", "Movie Management");
    item2->addSubItem(std::make_shared<MenuItem>("2.1", "Add movie"));
    item2->addSubItem(std::make_shared<MenuItem>("2.2", "Update Movie"));
    item2->addSubItem(std::make_shared<MenuItem>("2.3", "Delete movie"));

    auto item3 = std::make_shared<MenuItem>("3", "ShowTime Management");
    item3->addSubItem(std::make_shared<MenuItem>("3.1", "Add showtime"));
    item3->addSubItem(std::make_shared<MenuItem>("3.2", "Update showtime"));
    item3->addSubItem(std::make_shared<MenuItem>("3.3", "Delete showtime"));

    auto item4 = std::make_shared<MenuItem>("4", "Show Tickets");

    auto item5 = std::make_shared<MenuItem>("5", "Total Revenue");
    item5->addSubItem(std::make_shared<MenuItem>("5.1", "Month"));
    item5->addSubItem(std::make_shared<MenuItem>("5.2", "Quarter"));
    item5->addSubItem(std::make_shared<MenuItem>("5.3", "Year"));

    auto exit = std::make_shared<MenuItem>("6", "Exit");

    menu->addItem(item1);
    menu->addItem(item2);
    menu->addItem(item3);
    menu->addItem(item4);
    menu->addItem(item5);
    menu->addItem(exit);

    return menu;
}

std::shared_ptr<Menu> buildMenuUser() {
    auto menu = std::make_shared<Menu>();

    auto item1 = std::make_shared<MenuItem>("1", "Booking Ticket");
    item1->addSubItem(std::make_shared<MenuItem>("1.1", "Show Movie"));
    item1->addSubItem(std::make_shared<MenuItem>("1.2", "Show Show Time"));

    auto item2 = std::make_shared<MenuItem>("2", "Show Ticket");

    auto item3 = std::make_shared<MenuItem>("3", "Update Account");

    auto exit = std::make_shared<MenuItem>("4", "Exit");

    menu->addItem(item1);
    menu->addItem(item2);
    menu->addItem(item3);
    menu->addItem(exit);

    return menu;
}

std::shared_ptr<Menu> buildMainMenu() {
    auto menu = std::make_shared<Menu>();

    auto item1 = std::make_shared<MenuItem>("1", "Login");

    auto item2 = std::make_shared<MenuItem>("2", "Sign in");

    auto exit = std::make_shared<MenuItem>("3", "Exit");

    menu->addItem(item1);
    menu->addItem(item2);
    menu->addItem(exit);

    return menu;
}
void displayHistoryTicket(Tickets &input, const Account &account) {
    bool check = false;
    Vector<Ticket> tickets = input.getTickets();
    if (tickets.empty()) return;
    for (const Ticket &ticket : tickets) {
        if (ticket.getUserId() == account.getUserId()) {
            ticket.display();
            check = true;
        }
    }
    if (!check) {
        std::cout << "Empty!" << std::endl;
    }
}

void adminMenu(Account currentAccount, Accounts accounts, ShowTimes showTimes,
               Tickets tickets, Movies movies) {
    auto adminMenu = buildMenuAdmin();
    while (true) {
        float select = adminMenu->run();
        if (select == 1.0) {
            Vector<Movie> vecmovies = movies.getMovies();
            Vector<ShowTime> vecShowTimes = showTimes.getShowTimes();
            displayFutureMovies(vecmovies, vecShowTimes, tickets);
        }  // booking ticket
        else if (select == 2.1) {
            movies.addMovie();
        }  // add movie
        else if (select == 2.2) {
            movies.updateMovie();
        }  // Update movie
        else if (select == 2.3) {
            movies.deleteMovie();
        }  // Delete movie
        else if (select == 3.1) {
            addShowTime(movies, showTimes);
        }  // Add showtime
        else if (select == 3.2) {
            updateShowTime(showTimes, movies);
        }  // Update showtime
        else if (select == 3.3) {
            deleteShowTime(showTimes, movies);
        }  // Delete showtime
        else if (select == 4) {
            displayHistoryTicket(tickets, currentAccount);
        }  // show tickets
        else if (select == 5.1) {
            short month, year;
            std::cout << "Enter month: ";
            std::cin >> month;
            std::cout << "Enter year";
            std::cin >> year;
            tickets.revenueMonth(month, year);
        }  // total revenue month
        else if (select == 5.2) {
            short quarter, year;
            std::cout << "Enter quarter: ";
            std::cin >> quarter;
            std::cout << "Enter year: ";
            std::cin >> year;  // total revenue quarter
            tickets.revenueQuarter(year, quarter);
        }  // total revenue quarter
        else if (select == 5.3) {
            short year;
            std::cout << "Enter year: ";
            std::cin >> year;
            tickets.revenueYear(year);  // total revenue year
        }                               // total revenue year
        if (select == 6.0) {
            return;
        }  // return
        std::cout << select << std::endl;
    }
}
void pause() { std::system("pause"); }

void userMenu(Account currentAccount, Accounts accounts, ShowTimes showTimes,
              Tickets tickets, Movies movies) {
    auto userMenu = buildMenuUser();
    while (true) {
        float select = userMenu->run();
        if (areEqual(select, 1.1)) {
            Vector<Movie> vecmovies = movies.getMovies();
            Vector<ShowTime> vecShowTimes = showTimes.getShowTimes();
            displayFutureMovies(vecmovies, vecShowTimes, tickets);
        }  // show movie
        else if (areEqual(select, 1.2)) {
            bookTicketByShowtime(showTimes.getShowTimes(), tickets,
                                 currentAccount.getUserId());
        }  // book ticket by showtime
        else if (areEqual(select, 2.0)) {
            // Hiển thị lịch sử vé
            showTicketHistory(tickets, currentAccount.getUserId());
        }  // show ticket history
        else if (areEqual(select, 3.0)) {
            // Cập nhật tài khoản
            accounts.updateInformation(currentAccount);
        }  // update account
        else if (areEqual(select, 4.0)) {
            std::cout << "select == 4.0" << std::endl;
            return;  // Exit the menu
        }
        std::system("cls");  // Clear the screen after each iteration
    }
}

Account mainMenu(Accounts accounts) {
    Account account;
    auto mainMenu = buildMainMenu();

    while (true) {
        int select = int(mainMenu->run());
        if (select == 1) {
            if (accounts.login(account)) {
                return account;
            } else {
                continue;
            }
        }
        if (select == 2) {
            accounts.signIn();
        }
        if (select == 3) {
            exit(1);
        }
    }
}

void menu() {
    Movies movies(MOVIE_FILE);
    Tickets tickets(TICKET_FILE);
    ShowTimes showTimes(SHOWTIME_FILE);
    Accounts accounts(ACCOUNT_FILE);
    while (true) {
        Account currentAccount = mainMenu(accounts);

        if (currentAccount.isAdmin()) {
            std::cin.ignore();
            adminMenu(currentAccount, accounts, showTimes, tickets, movies);
        } else {
            std::cin.ignore();
            userMenu(currentAccount, accounts, showTimes, tickets, movies);
        }
    }
    return;
}
