#ifndef _SHOWTIME_H_
#define _SHOWTIME_H_

#include <string>

#include "my_lib.h"

class ShowTime {
   private:
    int showTimeId;
    int movieId;
    int hallId;
    Date showDate;
    Time startTime;
    Time endTime;

   public:
    void setShowTimeId(int showTimeId);
    void setMovieId(int movieId);
    void setHallId(int hallId);
    void setShowDate(const Date &showDate);
    void setStartTime(const Time &startTime);
    void setEndTime(const Time &endTime);
    void setTicketPrice(int ticketPrice);
    void setSeatNumber(int seatNumber);

    int getShowTimeId() const;
    int getMovieId() const;
    int getHallId() const;
    Date getShowDate() const;
    Time getStartTime() const;
    Time getEndTime() const;
    int getTicketPrice() const;
    int getSeatNumber() const;

    void display() const;
    void displayCol() const;
    // void updateDetails();
    bool operator==(const ShowTime &other) const {
        return showTimeId == other.showTimeId;
    }
    void readFromFile(std::stringstream &ss);
    bool saveToFile(const std::string &filename) const;
    ShowTime(int movieId, int hallId, const Date &showDate,
             const Time &startTime, const Time &endTime);
    ShowTime(int showTimeId, int movieId, int hallId, const Date &showDate,
             const Time &startTime, const Time &endTime);
    ShowTime(const ShowTime &other);
    ShowTime();
    ~ShowTime();
};

#endif  // _SHOWTIME_H_
