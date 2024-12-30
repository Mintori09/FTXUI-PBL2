#ifndef _USERMENU_H_
#define _USERMENU_H_
#include "menu.h"
using namespace ftxui;
class UserMenu : public menu {
public:
  UserMenu() {}
  ~UserMenu() {}
  void run(Movies &movies, ShowTimes &showtimes, Accounts &accounts,
           Tickets &tickets, Account &account);
};
#include "UserMenu.tpp"
#endif
