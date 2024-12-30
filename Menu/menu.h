#ifndef _MENU_H_
#define _MENU_H_
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/deprecated.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/util/ref.hpp>
#include <iostream>
#include <string>

#include "../include/Accounts.h"
#include "../include/Movies.h"
#include "../include/ShowTimes.h"
#include "../include/Tickets.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
class menu {
public:
  menu() {}
  ~menu() {}

protected:
  Vector<int> getLast6Months();
  bool checkShowTimeExistInTicket(const Ticket &ticket,
                                  const Vector<ShowTime> &showtimes);
  ShowTime getShowTimeFromTicket(const Ticket &ticket,
                                 const Vector<ShowTime> &showtimes);
  void addMovie(Movies &movies);
  void addShowTime(const Movie &movie, ShowTimes &showtimes);
  void UpdateAccount(Accounts &accounts, Account account);
  std::string getMovieTitle(int movieId, const Vector<Movie> &movies);
  bool checkMovieId(int movieId, const Vector<Movie> &movies);
  Movie getMovie(int movieId, const Vector<Movie> &movies);
  bool checkShowTimeId(int showTimeId, const Vector<ShowTime> &showTimes);
  Vector<ShowTime> getFutureShowTimes(const Vector<ShowTime> &showTimes,
                                      const Vector<Movie> &movies);
  Vector<ShowTime>
  getFutureAndCurrentShowTimes(const Vector<ShowTime> &showTimes,
                               const Vector<Movie> &movies);

  Vector<Movie> getMoviesWithFutureShowTimes(const Vector<ShowTime> &showTimes,
                                             const Vector<Movie> &movies);

  ftxui::Element DisplayMovies(const Vector<Movie> &movies,
                               const std::string &search_query);

  ftxui::Element DisplayShowTimes(const Vector<ShowTime> &showTimes,
                                  const Vector<Movie> &movies,
                                  const std::string &search_query);

  int selectMovie(const Vector<Movie> &movie, const Vector<ShowTime> &showtime);

  int selectMovieForAddShowTime(const Vector<Movie> &movies);

  int selectShowTime(Vector<Movie> &movies, Vector<ShowTime> &showtimes);

  int selectShowTimeCurrentAndFuture(const Vector<Movie> &movies,
                                     const Vector<ShowTime> &showtimes);

  std::string getUserName(const Ticket &ticket, const Vector<Account> accounts);

  int getMovieId(const Ticket &ticket, const Vector<Movie> movies,
                 const Vector<ShowTime> showtimes);

  ShowTime getShowTime(int showTimeId, const Vector<ShowTime> &showtimes);

  ftxui::Element DisplayTicketsAll(const Vector<Account> accounts,
                                   const Vector<Ticket> tickets,
                                   const ShowTime &showtime,
                                   const Vector<Movie> &movies,
                                   const std::string &search_query);

  ftxui::Element DisplayTicketsAll(const Vector<Ticket> tickets,
                                   const Vector<ShowTime> &showtimes,
                                   const Vector<Movie> &movies,
                                   const Account account);

  void ShowTicketInfo(const Vector<Account> accounts,
                      const Vector<Ticket> &tickets,
                      const Vector<Movie> &movies, const ShowTime &showtime);

  void ShowTicketInfoForAccount(const Account account,
                                const Vector<Ticket> &tickets,
                                const Vector<Movie> &movies,
                                const Vector<ShowTime> &showtimes);

  void showTicket(const Vector<Account> accounts, const Vector<Ticket> &tickets,
                  const Vector<Movie> &movies,
                  const Vector<ShowTime> &showtimes);

  void showUpdateAccountMenu(Accounts &accounts, Account &account);

  void updateMovie(Movies &movies, Movie movie);

  void displayMovie(const Vector<Movie> &movie,
                    const Vector<ShowTime> &showtime);

  void displayShowTimes(const Vector<Movie> &movies,
                        const Vector<ShowTime> &showtimes);

  void addShowTime(Movies &movies, ShowTimes &showTimes);

  ftxui::Element DisplayShowTimes(const Vector<ShowTime> &showTimes);

  ftxui::Element DisplayCheckedTicket(const Movie &movie,
                                      const ShowTime &showtime,
                                      const Account &account,
                                      Vector<int> isSelected, Tickets &tickets);

  bool Step4_DisplayCheckedTicket(const Movie &movie, const ShowTime &showtime,
                                  const Account &account,
                                  Vector<int> isSelected, Tickets &tickets);

  void removeSeat(Vector<int> &selectedSeats, int seat);

  bool Step3_SelectSeat(const Movie &movie, const ShowTime &showtime,
                        const Account &account, Tickets &tickets);

  bool Step2_SelectShowTime(const Movie &movie,
                            const Vector<ShowTime> &showtimes,
                            const Account &account, Tickets &tickets);
  void Step1_SelectMovie(const Vector<Movie> &movie,
                         const Vector<ShowTime> &showtimes,
                         const Account &accounts, Tickets &tickets);

  void BookingTicket(const Vector<Movie> &movies,
                     const Vector<ShowTime> &showtimes, const Account &account,
                     Tickets &tickets);

  bool checkDateByMonthAndYear(const Ticket &ticket, int month);

  int maxOfVectorInt(Vector<int> nums);
};

#endif
