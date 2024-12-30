#include "Menu/AdminMenu.h"
#include "Menu/UserMenu.h"
#include "Menu/mainmenu.h"

int main() {
  UserMenu usermenu;
  AdminMenu adminmenu;
  while (true) {
    Movies movies(MOVIE_FILE);
    ShowTimes showtimes(SHOWTIME_FILE);
    Accounts accounts(ACCOUNT_FILE);
    Tickets tickets(TICKET_FILE);
    Account account = showMenu(accounts);
    if (account.getRole() == 1) {
      adminmenu.run(movies, showtimes, accounts, tickets, account);
    } else if (account.getRole() == 2) {
      usermenu.run(movies, showtimes, accounts, tickets, account);
    }
  }

  return 0;
}
