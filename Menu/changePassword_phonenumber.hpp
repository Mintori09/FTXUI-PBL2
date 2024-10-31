
#include <string> // for char_traits, operator+, string, basic_string

#include "../include/Accounts.h"
#include "ftxui/component/component.hpp"      // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp" // for ComponentBase
#include "ftxui/component/component_options.hpp" // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include <ftxui/dom/elements.hpp> // for text, hbox, separator, Element, operator|, vbox, border
#include <ftxui/util/ref.hpp> // for Ref

void UpdateAccount(Accounts &accounts, Account account) {
  using namespace ftxui;
  auto screen = ScreenInteractive::TerminalOutput();
  // The data:
  std::string username = account.getUserName();
  std::string password;
  std::string phoneNumber;
  std::string newPassword;
  std::string message;

  // The password input component:
  InputOption password_option;
  password_option.password = true;
  Component input_password =
      Input(&newPassword, "new password", password_option);
  Component input_current_password =
      Input(&password, "confirm password", password_option);

  // We are using `CatchEvent` to filter out non-digit characters.
  Component input_phone_number = Input(&phoneNumber, account.getPhoneNumber());
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && !std::isdigit(event.character()[0]);
  });
  input_phone_number |= CatchEvent([&](Event event) {
    return event.is_character() && phoneNumber.size() > 10;
  });

  auto confirm_button = Button("confirm", [&] {
    if (password == account.getPassWord()) {
      if (newPassword != password) {
        account.setPassWord(newPassword);
      }
      if (phoneNumber != account.getPhoneNumber()) {
        account.setPhoneNumber(phoneNumber);
      }
    } else {
      message = "Failed";
    }
  });
  auto exit_button = Button("Exit", [&] { screen.Exit(); });
  // The component tree:
  auto component = Container::Vertical({input_password, input_phone_number,
                                        input_current_password, confirm_button,
                                        exit_button});
  // Tweak how the component tree is rendered:
  auto renderer = Renderer(component, [&] {
    return vbox({
               text("Hello " + username),
               separator(),
               hbox(text(" Password : "), input_password->Render()),
               hbox(text(" phone number  : "), input_phone_number->Render()),
               hbox(text(" Confirm password :"),
                    input_current_password->Render()),
               separator(),
               hbox(confirm_button->Render(), exit_button->Render(),
                    text(message)),
               separator(),
               text(account.getPassWord()),
               text(account.getUserName()),
               text(account.getPhoneNumber()),
           }) |
           border;
  });

  screen.Loop(renderer);
  accounts.updateAccount(account);
}
