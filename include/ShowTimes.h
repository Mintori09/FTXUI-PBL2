#ifndef _SHOWTIMES_H_
#define _SHOWTIMES_H_

#include <fstream>
#include <iostream>

#include "ShowTime.h"
#include "Vector.h"

class ShowTimes {
   private:
    Vector<ShowTime> showTimes;

   public:
    ShowTimes() {}
    ShowTimes(const std::string &filename);
    ~ShowTimes();

    Vector<ShowTime> getShowTimes() const {
        return showTimes;
    }
    void display(int showTimeId) const;
    void displayAll() const;

    bool addShowTime(const ShowTime &newShowTime);
    bool deleteShowTime(int showTimeId);
    bool updateShowTime(int showTimeId);

    void display() const;

    bool saveToFile(const std::string &filename) const;
    bool loadFromFile(const std::string &filename);
};

#endif
