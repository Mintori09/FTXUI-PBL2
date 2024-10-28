#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <string>

#include "../include/Account.h"
#include "../include/Accounts.h"
#include "../include/my_lib.h"

using namespace ftxui;
void clear() { system("cls"); }

bool login(Accounts& accounts, Account& account, std::string& login_message) {
    std::string username = account.getUserName();
    std::string password = account.getPassWord();
    if (accounts.authenticate(username, password)) {
        clear();
        login_message = "Login successful!";
        username.clear();  // Clear username after login
        password.clear();  // Clear password after login
        return true;       // Successful login
    } else {
        login_message = "Login failed. Please try again.";
        return false;  // Failed login
    }
}

void registerAccount(Accounts& accounts, Account& account,
                     std::string& login_message) {
    std::string username = account.getUserName(),
                password = account.getPassWord(),
                phoneNumber = account.getPhoneNumber();
    if (username.empty() || password.empty()) {
        login_message = "Username and password cannot be empty.";
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

void showLoginMenu(Accounts& accounts, Account& account,
                   std::string& login_message, bool& switch_to_register,
                   ScreenInteractive& screen, bool& mainMenu) {
    std::string username, password;
    auto username_input = Input(&username, "Username");
    auto password_input = Input(&password, "Password");

    auto login_button = Button("Login", [&] {
        account.setPassWord(password);
        account.setUserName(username);
        if (login(accounts, account, login_message)) {
            mainMenu = false;  // Exit the main menu if login is successful
            screen.Exit();     // Exit current menu
        }
    });

    auto switch_button = Button("Switch to Register", [&] {
        switch_to_register = true;  // Set flag to switch to register menu
        screen.Exit();              // Exit current menu
    });

    auto container = Container::Vertical(Components{
        username_input,
        password_input,
        login_button,
        switch_button,
        Renderer(
            [&] { return text(login_message) | bold | color(Color::Red); }),
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   username_input->Render() | borderStyled(ROUNDED) |
                       size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   password_input->Render() | borderStyled(ROUNDED) |
                       size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   text(login_message) | bold | color(Color::Red),
                   login_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                   switch_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,

               }) |
               borderStyled(ROUNDED) | center;
    });

    screen.Loop(renderer);
}

void showRegisterMenu(Accounts& accounts, Account& account,
                      std::string& login_message, bool& switch_to_register,
                      ScreenInteractive& screen) {
    std::string phoneNumber, username, password;

    auto username_input = Input(&username, "Username");
    auto password_input = Input(&password, "Password");
    auto phoneNumber_input = Input(&phoneNumber, "Password");

    auto register_button = Button("Register", [&] {
        account.setPassWord(password);
        account.setPhoneNumber(phoneNumber);
        account.setUserName(username);
        registerAccount(accounts, account, login_message);
    });

    auto switch_button = Button("Back to Login", [&] {
        switch_to_register = false;  // Set flag to switch back to login menu
        screen.Exit();               // Exit current menu
    });

    auto container = Container::Vertical(Components{
        username_input,
        password_input,
        phoneNumber_input,
        register_button,
        switch_button,
        Renderer(
            [&] { return text(login_message) | bold | color(Color::Red); }),
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
                   register_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                   switch_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,

               }) |
               border | center;
    });

    screen.Loop(renderer);
}

Account showMenu(Accounts& accounts) {
    std::string login_message;
    bool switch_to_register = false;
    Account account;
    auto screen = ScreenInteractive::TerminalOutput();
    bool mainMenu = true;

    while (mainMenu) {
        if (switch_to_register) {
            clear();
            showRegisterMenu(accounts, account, login_message,
                             switch_to_register, screen);
        } else {
            clear();
            showLoginMenu(accounts, account, login_message, switch_to_register,
                          screen, mainMenu);
        }

        // Check if main menu should exit
        if (!switch_to_register && login_message == "Login successful!") {
            clear();
            break;  // Exit the menu if login is successful
        }
    }

    return account;
}

    // Accounts accounts(ACCOUNT_FILE);
    // Account currentAccount = showMenu(accounts);
