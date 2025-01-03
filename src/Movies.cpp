#include "../include/Movies.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool UpdateValue(Movie &movie, int choice);

Movies::Movies() {}
Movies::Movies(const std::string &filename) { loadFromFile(filename); }
Movies::~Movies() {}
Vector<Movie> Movies::getMovies() const { return movies; }
bool Movies::addMovie(Movie &newMovie) {
  for (const Movie &movie : movies) {
    if (movie.getTitle() == newMovie.getTitle() &&
        movie.getDirector() == newMovie.getDirector()) {
      return false;
    }
  }
  int movieId = (movies.empty())
                    ? 10000
                    : movies.back().getMovieId() + 1; // Auto-increment movieId
  newMovie.setMovieId(movieId);
  movies.push_back(newMovie); // Add movie to the vector
  newMovie.saveToFile(MOVIE_FILE);
  std::cout << "Movie added with ID: " << movieId << "\n";
  return true;
}

void Movies::display() const {
  std::system("cls");
  if (movies.empty()) {
    std::cout << "Sorry, no movies to display." << std::endl;
  } else {
    for (const auto &movie : movies) {
      movie.display();
    }
  }
}

bool Movies::updateMovie(const Movie &movie) {
  for (auto &m : movies) {
    if (m.getMovieId() == movie.getMovieId()) {
      m = movie;
      saveToFile(MOVIE_FILE);
      return true;
    }
  }
  return false;
}

bool Movies::isMovieId(int movieId) const {
  for (const Movie &movie : movies) {
    if (movieId == movie.getMovieId()) {
      return true;
    }
  }
  return false;
}

bool Movies::deleteMovie(const Movie &movie) {
  for (auto it = movies.begin(); it != movies.end(); ++it) {
    if (it->getMovieId() == movie.getMovieId()) {
      movies.erase(it);
      saveToFile(MOVIE_FILE);
      return true;
    }
  }
  return false;
}

bool Movies::saveToFile(const std::string &filename) const {
  if (!ensureDirectoryExists(filename)) {
    return false; // If the directory doesn't exist and couldn't be
                  // created, return false
  }
  std::ofstream outFile(filename);
  if (!outFile) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return false;
  }
  for (const auto &movie : movies) {
    outFile << movie.getMovieId() << ';' << movie.getTitle() << ';'
            << movie.getDescription() << ';' << movie.getGenre() << ';'
            << movie.getDirector() << ';' << movie.getCast() << ';'
            << movie.getDuration() << ';' << movie.getReleaseDate() << ';'
            << movie.getRating() << '\n';
  }
  outFile.close();
  return true;
}

bool Movies::loadFromFile(const std::string &filename) {
  if (!ensureDirectoryExists(filename)) {
    return false; // If the directory doesn't exist and couldn't be
                  // created, return false
  }
  std::ifstream inFile(filename);
  if (!inFile) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    std::ofstream createFile(filename);
    if (!createFile) {
      std::cerr << "Create File failed!" << std::endl;
      return false;
    } else {
      std::cerr << "Create File successfully!" << std::endl;
    }
  }
  if (isEmpty(inFile)) {
    std::cerr << "File is empty!";
    return false;
  }
  movies.clear();
  std::string line;
  while (std::getline(inFile, line)) {
    std::stringstream ss(line);

    Movie newMovie;
    newMovie.readFromFile(ss);
    movies.push_back(newMovie);
  }
  inFile.close();
  return true;
}

void Movies::addMovie() {
  int movieId = (movies.empty())
                    ? 1
                    : movies.back().getMovieId() + 1; // Auto-increment movieId
  std::string title, description, genre, director, cast;
  int duration;
  short day, month, year;
  int rating;

  std::cout << "Enter Title: ";
  std::cin.ignore(); // To clear any leftover newline character
  std::getline(std::cin, title);

  std::cout << "Enter Description: ";
  std::getline(std::cin, description);

  std::cout << "Enter Genre: ";
  std::getline(std::cin, genre);

  std::cout << "Enter Director: ";
  std::getline(std::cin, director);

  std::cout << "Enter Cast: ";
  std::getline(std::cin, cast);

  std::cout << "Enter Duration (in minutes): ";
  std::cin >> duration;

  std::cout << "Enter Release Date (day month year): ";
  std::cin >> day >> month >> year;
  Date releaseDate(day, month, year); // Create release date

  std::cout << "Enter Rating (out of 10): ";
  std::cin >> rating;

  Movie newMovie(movieId, title, description, genre, director, cast, duration,
                 releaseDate, rating);
  movies.push_back(newMovie); // Add movie to the vector
  newMovie.saveToFile(MOVIE_FILE);
  std::cout << "Movie added with ID: " << movieId << "\n";
}

void Movies::displayMovies() const {
  int count = 0;
  for (auto it = movies.begin(); it != movies.end() && count < 50;
       ++it, ++count) {
    it->display();
  }
}
