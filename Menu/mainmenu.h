#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/util/ref.hpp>
#include <string>

#include "../include/Accounts.h"
#include "../include/my_lib.h"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
using namespace ftxui;

bool login(Accounts &accounts, Account &account, std::string &login_message);

void registerAccount(Accounts &accounts, Account &account,
                     std::string &login_message);
// void forgotPassword(Accounts &accounts, Account &account);
void forgotPassword(Accounts &accounts);

void showLoginMenu(Accounts &accounts, Account &account,
                   std::string &login_message, bool &switch_to_register,
                   ScreenInteractive &screen, bool &mainMenu);

void showRegisterMenu(Accounts &accounts, Account &account,
                      std::string &login_message, bool &switch_to_register,
                      ScreenInteractive &screen);

Account showMenu(Accounts &accounts);

#include"mainmenu.tpp"

#endif
