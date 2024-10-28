#include "../include/Movie.h"

#include <iostream>


Movie::Movie(int id, const std::string &title, const std::string &description,
             const std::string &genre, const std::string &director,
             const std::string &cast, int duration, const Date &releaseDate,
             int rating)
    : movieId(id),
      title(title),
      description(description),
      genre(genre),
      director(director),
      cast(cast),
      duration(duration),
      releaseDate(releaseDate),
      rating(rating) {}

Movie::Movie(const Movie &source)
    : movieId(source.getMovieId()),
      title(source.getTitle()),
      description(source.getDescription()),
      genre(source.getGenre()),
      director(source.getDirector()),
      cast(source.getCast()),
      duration(source.getDuration()),
      releaseDate(source.getReleaseDate()),
      rating(source.getRating()) {}

Movie::Movie() : movieId(0), duration(0), rating(0) {}

Movie::~Movie() {}

Movie &Movie::operator=(const Movie &source) {
    if (this == &source) return *this;

    movieId = source.movieId;
    title = source.title;
    description = source.description;
    genre = source.genre;
    director = source.director;
    cast = source.cast;
    duration = source.duration;
    releaseDate = source.releaseDate;
    rating = source.rating;

    return *this;
}

void Movie::setMovieId(int movieId) { this->movieId = movieId; }
void Movie::setTitle(const std::string &title) { this->title = title; }
void Movie::setDescription(const std::string &description) {
    this->description = description;
}
void Movie::setGenre(const std::string &genre) { this->genre = genre; }
void Movie::setDirector(const std::string &director) {
    this->director = director;
}
void Movie::setCast(const std::string &cast) { this->cast = cast; }
void Movie::setDuration(int duration) { this->duration = duration; }
void Movie::setReleaseDate(const std::string &releaseDate) {
    this->releaseDate = releaseDate;
}
void Movie::setRating(int rating) { this->rating = rating; }

int Movie::getMovieId() const { return movieId; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getDescription() const { return description; }
std::string Movie::getGenre() const { return genre; }
std::string Movie::getDirector() const { return director; }
std::string Movie::getCast() const { return cast; }
int Movie::getDuration() const { return duration; }
Date Movie::getReleaseDate() const { return releaseDate; }
int Movie::getRating() const { return rating; }

bool Movie::displayGenre(const std::string &genres) const {
    return isAllWordsInString(genre,
                              genres);  // Ensure isAllWordsInString is defined
}

bool Movie::checkTitle(const std::string &titles) const {
    // Implement checkTitle if needed
    return title == titles;
}

void Movie::display() const {
    std::cout << movieId << "\t" << title << "\t" << description << "\t" << genre << "\t"
              << director << "\t" << cast << "\t" << duration << " min\t"
              << releaseDate << "\t" << rating << "/10\n";
}

void Movie::displayCol() const {
    int count = 1;
    std::cout << count++ << ". Movie Id: " << movieId << std::endl;
    std::cout << count++ << ". Title: " << title << std::endl;
    std::cout << count++ << ". Description: " << description << std::endl;
    std::cout << count++ << ". Genre: " << genre << std::endl;
    std::cout << count++ << ". Director: " << director << std::endl;
    std::cout << count++ << ". Cast: " << cast << std::endl;
    std::cout << count++ << ". Duration: " << duration << " minutes"
              << std::endl;
    std::cout << count++ << ". Release Date: " << releaseDate << std::endl;
    std::cout << count++ << ". Rating: " << rating << "/10" << std::endl;
}

void Movie::readFromFile(std::stringstream &ss) {
    std::string item;

    std::getline(ss, item, ';');
    movieId = std::stoi(item);
    std::getline(ss, title, ';');
    std::getline(ss, description, ';');
    std::getline(ss, genre, ';');
    std::getline(ss, director, ';');
    std::getline(ss, cast, ';');
    std::getline(ss, item, ';');
    duration = std::stoi(item);
    std::getline(ss, item, ';');
    Date releaseDate(item);
    std::getline(ss, item, ';');
    rating = std::stoi(item);
}

bool Movie::saveToFile(std::string filename) {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename, std::ios::app | std::ios::trunc);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename
                  << std::endl;
        // Thử mở lại với chế độ tạo mới nếu không mở được
        outFile.open(filename, std::ios::out);
        if (!outFile) {
            std::cerr << "Error creating file: " << filename << std::endl;
            return false;
        }
    }

    outFile << movieId << ';' << title << ';' << description << ';' << genre
            << ';' << director << ';' << cast << ';' << duration << ';'
            << releaseDate << ';' << rating << '\n';
    outFile.close();
    return true;
}