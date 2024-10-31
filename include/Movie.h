#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <string>

#include "my_lib.h"

class Movie {
   private:
    int movieId;
    std::string title;
    std::string description;
    std::string genre;
    std::string director;
    std::string cast;
    int duration;  // Duration in minutes
    Date releaseDate;
    int rating;  // Rating out of 10

   public:
    Movie(int id, const std::string& title, const std::string& description,
          const std::string& genre, const std::string& director,
          const std::string& cast, int duration, const Date& releaseDate,
          int rating);
    Movie(int id, const std::string& title, const std::string& genre,
          const std::string& director, int duration,
          const std::string& releaseDate, double rating);
    Movie(int id, const std::string& title, const std::string& description,
          const std::string& genre, const std::string& director, int duration,
          const std::string& releaseDate, double rating);
    Movie(const std::string& title, const std::string& description,
          const std::string& genre, const std::string& director,
          const std::string& cast, int duration, Date& releaseDate);
    Movie(const Movie& source);
    Movie();
    ~Movie();

    Movie& operator=(const Movie& source);

    void setMovieId(int movieId);
    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setGenre(const std::string& genre);
    void setDirector(const std::string& director);
    void setCast(const std::string& cast);
    void setDuration(int duration);
    void setReleaseDate(const std::string& releaseDate);
    void setRating(int rating);

    int getMovieId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getGenre() const;
    std::string getDirector() const;
    std::string getCast() const;
    int getDuration() const;
    Date getReleaseDate() const;
    int getRating() const;

    bool displayGenre(const std::string& genres) const;
    bool checkTitle(const std::string& titles) const;
    void display() const;
    void displayCol() const;
    void readFromFile(std::stringstream& ss);
    bool saveToFile(std::string filename);
    bool operator==(const Movie& other) const {
        return this->movieId == other.movieId;
    }
};

#endif  // _MOVIE_H_
