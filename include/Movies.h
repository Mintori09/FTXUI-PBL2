#ifndef _MOVIES_H_
#define _MOVIES_H_

#include "Movie.h"
#include "Vector.h"
#include "my_lib.h"


class Movies {
   private:
    Vector<Movie> movies;
    void findByGenre(const std::string& genre) const;
    void findByTitle(const std::string& title) const;
    bool isMovieId(int movieId) const;
    void sortByReleaseDate();

   public:
    Vector<Movie> getMovies();
    Movies(const std::string& filename);
    Movies();
    ~Movies();

    void deleteMovie();
    void displayMovies() const;
    void addMovie();
    bool addMovie(const Movie& source);
    void findMovie(const std::string& find) const;
    bool deleteMovie(int movieId);
    bool updateMovie();
    void display() const;
    void sort();
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif
