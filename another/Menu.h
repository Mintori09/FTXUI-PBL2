#include <windows.h>

#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "Accounts.h"
#include "Movies.h"
#include "ShowTimes.h"
#include "Tickets.h"
#include "Vector.h"
#include "my_lib.h"

const float EPSILON = 1e-6;  // Điều chỉnh độ chính xác theo nhu cầu
int generateTicketId(const Vector<Ticket>& tickets);
void updateShowTime(ShowTimes& showTimes, Movies& movies);
void showTicketHistory(const Tickets& tickets, int userId);
inline bool areEqual(float a, float b) { return std::fabs(a - b) < EPSILON; }
// void bookTicket(Tickets ticket, ShowTime showtime, Account account);
void displayFutureShowtimes(const Vector<ShowTime>& showTimes,
                            const Vector<Movie>& movies, int movieId = -1);
void displayFutureMovies(const Vector<Movie> movies,
                         const Vector<ShowTime> showTimes, Tickets& tickets);
void bookTicketByShowtime(const Vector<ShowTime>& showTimes, Tickets& tickets,
                          int userId);

// void displayHistoryTicket(Tickets& input, const Account& account);
void userMenu(Accounts accounts, ShowTimes showtimes, Tickets tickets,
              Movies movies);
void gotoxy(int x, int y);
void menu();
Account mainMenu();
void adminMenu(Account currentAccount, Accounts accounts, ShowTimes showtimes,
               Tickets tickets, Movies movies);
void userMenu(Account currentAccount, Accounts accounts, ShowTimes showtimes,
              Tickets tickets, Movies movies);
Account mainMenu(Accounts accounts);
class MenuItem : public std::enable_shared_from_this<MenuItem> {
   public:
    void bookTicket(Tickets ticket, ShowTime showtime, Account account);
    MenuItem(const std::string& id, const std::string& name);
    void addSubItem(std::shared_ptr<MenuItem> subItem);
    void toggleExpanded();
    bool isLeaf() const;
    const std::string& getIdentifier() const;
    const std::string& getName() const;
    bool isExpanded() const;
    void display(const std::string&, int) const;
    void getIdentifiers(Vector<std::string>& input);
    MenuItem* findItem(const std::string& id);

   private:
    std::string identifier;
    std::string name;
    bool expanded;
    Vector<std::shared_ptr<MenuItem>> subItems;
};

class Menu {
   public:
    void addItem(std::shared_ptr<MenuItem> item);
    void display(const std::string& highlight) const;
    void getIdentifiers(Vector<std::string>& input);
    float run();

   private:
    Vector<std::shared_ptr<MenuItem>> items;
    MenuItem* findItem(const std::string& id);
};

std::shared_ptr<Menu> buildMenuAdmin();
std::shared_ptr<Menu> buildMenuUser();
std::shared_ptr<Menu> buildMainMenu();

// int main()
// {
//     auto menu = buildMenuAdmin();
//     menu->run();
//     return 0;
// }