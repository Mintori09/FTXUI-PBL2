#ifndef _MOVIES_H_
#define _MOVIES_H_

#include "Movie.h"
#include "Vector.h"
#include "my_lib.h"

class Movies {
   private:
    Vector<Movie> movies;
    bool isMovieId(int movieId) const;
    void sortByReleaseDate();

   public:
    Vector<Movie> getMovies() const;
    Movies(const std::string& filename);
    Movies();
    ~Movies();

    void displayMovies() const;
    void addMovie();
    bool addMovie(Movie& newMovie);
    bool deleteMovie(const Movie& movie);
    bool updateMovie(const Movie& movie);
    void display() const;
    void sort();
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
};

#endif
