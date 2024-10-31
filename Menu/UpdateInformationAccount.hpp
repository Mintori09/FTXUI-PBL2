#include <ftxui/component/component.hpp>           // for buttons, inputs, etc.
#include <ftxui/component/component.hpp>           // for Container
#include <ftxui/component/screen_interactive.hpp>  // for ScreenInteractive
#include <ftxui/dom/elements.hpp>                  // for text, vbox, etc.
#include <ftxui/util/ref.hpp>                      // for ref
#include <string>
#include "../include/Accounts.h"

using namespace ftxui;

// Hàm hiển thị menu cập nhật tài khoản
void showUpdateAccountMenu(Accounts& accounts, Account& account) {
    std::string update_message;
    ScreenInteractive screen = ScreenInteractive::Fullscreen();
    std::string new_username, new_password, new_phoneNumber;

    auto new_username_input = Input(&new_username, "New Username");
    auto new_password_input = Input(&new_password, "New Password");
    auto new_phoneNumber_input = Input(&new_phoneNumber, "New Phone Number");

    auto update_button = Button("Update Account", [&] {
        account.setUserName(new_username);
        account.setPassWord(new_password);
        account.setPhoneNumber(new_phoneNumber);
        if (accounts.updateAccount(account)) {
            update_message = "Account updated successfully!";
        } else {
            update_message = "Account not found!";
        }
    });

    auto back_button = Button("Back", [&] { screen.Exit(); });

    auto container = Container::Vertical(Components{
        new_username_input,
        new_password_input,
        new_phoneNumber_input,
        update_button,
        back_button,
        Renderer(
            [&] { return text(update_message) | bold | color(Color::Red); }),
    });

    auto renderer = Renderer(container, [&] {
        return vbox({
                   new_username_input->Render() | border |
                       size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   new_password_input->Render() | border |
                       size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   new_phoneNumber_input->Render() | border |
                       size(ftxui::WIDTH, ftxui::EQUAL, 50),
                   text(update_message) | bold | color(Color::Red),
                   update_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
                   back_button->Render() |
                       size(ftxui::WIDTH, ftxui::EQUAL, 25) | center,
               }) |
               border | center;
    });

    screen.Loop(renderer);
}

//    showUpdateAccountMenu(accounts, account);

