#ifndef _SHOWTIMES_H_
#define _SHOWTIMES_H_

#include <fstream>
#include <iostream>

#include "ShowTime.h"
#include "Vector.h"

class ShowTimes {
   private:
    Vector<ShowTime> showTimes;
    int showTimeId();

   public:
    ShowTimes() {}
    ShowTimes(const std::string &filename);
    ~ShowTimes();

    Vector<ShowTime> getShowTimes() const { return showTimes; }
    void display(int showTimeId) const;
    void displayAll() const;

    bool addShowTime(ShowTime &newShowTime);
    bool deleteShowTime(int showTimeId);
    bool checkValidShowTime(const ShowTime &showTime) const;
    // bool updateShowTime(int showTimeId);

    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
};

#endif
