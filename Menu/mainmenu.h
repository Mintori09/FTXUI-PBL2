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

void showLoginMenu(Accounts &accounts, Account &account,
                   std::string &login_message, bool &switch_to_register,
                   ScreenInteractive &screen, bool &mainMenu);

void showRegisterMenu(Accounts &accounts, Account &account,
                      std::string &login_message, bool &switch_to_register,
                      ScreenInteractive &screen);

Account showMenu(Accounts &accounts);

bool login(Accounts &accounts, Account &account, std::string &login_message) {
  std::string username = account.getUserName();
  std::string password = account.getPassWord();
  if (accounts.authenticate(username, password, account)) {
    clearScreen();
    login_message = "Login successful!";
    username.clear();
    password.clear();
    return true;
  } else {
    login_message = "Login failed. Please try again.";
    return false;
  }
}

void registerAccount(Accounts &accounts, Account &account,
                     std::string &login_message) {
  std::string username = account.getUserName(),
              password = account.getPassWord(),
              phoneNumber = account.getPhoneNumber();
  if (username.empty() || password.empty() || phoneNumber.empty()) {
    login_message = "username/ password/ phonenumber cannot be empty";
  } else if (!isValidStringNoSymbol(username)) {
    login_message = "Username is not Valid.";
  } else if (!isValidString(password)) {
    login_message = "password is not Valid.";
  } else if (!isValidString(password)) {
    login_message = "phonenumber is not Valid.";
  } else if (accounts.isUsernameTaken(username)) {
    login_message =
        "This username is already in use. Please choose a different one.";
  } else {
    system("cls");
    accounts.addAccount(account);
    login_message = "Account created successfully! You can now log in.";
    username.clear();
    password.clear();
  }
}

void showLoginMenu(Accounts &accounts, Account &account,
                   std::string &login_message, bool &switch_to_register,
                   ScreenInteractive &screen, bool &mainMenu) {
  std::string username, password;
  auto username_input = Input(&username, "Username");
  auto password_input = Input(&password, "Password");

  auto login_button = Button("Login", [&] {
    account.setPassWord(password);
    account.setUserName(username);
    if (login(accounts, account, login_message)) {
      mainMenu = false;
      screen.Exit();
    }
  });

  auto switch_button = Button("Switch to Register", [&] {
    switch_to_register = true;
    screen.Exit();
  });
  auto exit_button = Button("Exit", [&] { exit(1); });

  auto container = Container::Vertical(Components{
      username_input,
      password_input,
      login_button,
      switch_button,
      exit_button,
      Renderer([&] { return text(login_message) | bold | color(Color::Red); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               username_input->Render() | borderStyled(ROUNDED) |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               password_input->Render() | borderStyled(ROUNDED) |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               text(login_message) | bold | color(Color::Red),
               hbox({
                   login_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                   switch_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
               }),
               exit_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,

           }) |
           border | center | hcenter;
  });

  screen.Loop(renderer);
}

void showRegisterMenu(Accounts &accounts, Account &account,
                      std::string &login_message, bool &switch_to_register,
                      ScreenInteractive &screen) {
  std::string phoneNumber, username, password;

  auto username_input = Input(&username, "Username");
  auto password_input = Input(&password, "Password");
  auto phoneNumber_input = Input(&phoneNumber, "phone number");
  phoneNumber_input |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  phoneNumber_input |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  auto register_button = Button("Register", [&] {
    account.setPassWord(password);
    account.setPhoneNumber(phoneNumber);
    account.setUserName(username);
    registerAccount(accounts, account, login_message);
  });

  auto switch_button = Button("Back to Login", [&] {
    switch_to_register = false;
    screen.Exit();
  });
  auto exit_button = Button("Exit", [&] { exit(1); });
  auto container = Container::Vertical(Components{
      username_input,
      password_input,
      phoneNumber_input,
      register_button,
      switch_button,
      exit_button,
      Renderer([&] { return text(login_message) | bold | color(Color::Red); }),
  });

  auto renderer = Renderer(container, [&] {
    return vbox({
               username_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               password_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               phoneNumber_input->Render() | border |
                   size(ftxui::WIDTH, ftxui::EQUAL, 50),
               text(login_message) | bold | color(Color::Red),
               hbox({register_button->Render() |
                         size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                     switch_button->Render() |
                         size(ftxui::WIDTH, ftxui::EQUAL, 25) | center}),
               exit_button->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 25) |
                   center,

           }) |
           hcenter | border | center;
  });

  screen.Loop(renderer);
}

Account showMenu(Accounts &accounts) {
  std::string login_message;
  bool switch_to_register = false;
  Account account;
  auto screen = ScreenInteractive::TerminalOutput();
  bool mainMenu = true;

  while (mainMenu) {
    if (switch_to_register) {
      clearScreen();
      showRegisterMenu(accounts, account, login_message, switch_to_register,
                       screen);
    } else {
      clearScreen();
      showLoginMenu(accounts, account, login_message, switch_to_register,
                    screen, mainMenu);
    }

    if (!switch_to_register && login_message == "Login successful!") {
      clearScreen();
      break;
    }
  }

  return account;
}
#endif
