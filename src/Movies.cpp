#include "../include/Movies.h"

#include <fstream>
#include <iostream>
#include <sstream>


bool UpdateValue(Movie &movie, int choice);

Movies::Movies() {}
Movies::Movies(const std::string &filename) { loadFromFile(filename); }
Movies::~Movies() {}
Vector<Movie> Movies::getMovies() { return movies; }
bool Movies::addMovie(const Movie &addedMovie) {
    for (const Movie &movie : movies) {
        if (movie.getTitle() == addedMovie.getTitle()) {
            return false;
        }
    }
    movies.push_back(addedMovie);
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

void Movies::findMovie(const std::string &find) const {
    display();
    std::cout << "1. Find by title." << std::endl
              << "2. Find by genre." << std::endl;
    int select = inputInteger("Enter your selection: ");
    while (select < 1 || select > 2) {
        std::cout << "Your selection is invalid. Please try again: ";
        select = inputInteger("Enter your selection: ");
    }
    if (select == 1) {
        findByTitle(find);
    } else {
        findByGenre(find);
    }
}

void Movies::findByGenre(const std::string &genre) const {
    bool found = false;
    for (const Movie &movie : movies) {
        if (isAllWordsInString(genre, movie.getGenre())) {
            movie.display();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No movies found for genre: " << genre << std::endl;
    }
}

void Movies::findByTitle(const std::string &title) const {
    bool found = false;
    for (const Movie &movie : movies) {
        if (isAllWordsInString(title, movie.getTitle())) {
            movie.display();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No movies found with title: " << title << std::endl;
    }
}

bool Movies::updateMovie() {
    display();
    int select = inputInteger("Enter the index of the movie to update: ");
    while (select < 0 || select >= movies.size()) {
        std::cout << "Your selection is invalid. Please try again: ";
        select = inputInteger("Enter the index of the movie to update: ");
    }
    movies[select].displayCol();
    std::cout << "9. Exit" << std::endl;
    while (true) {
        select = inputInteger(
            "What do you want to change (enter the order number): ");
        if (select == 9) {
            break;
        }
        if (select >= 1 && select <= 8) {
            if (UpdateValue(movies[select], select)) {
                std::cout << "\nUpdated movie details:\n";
                movies[select].display();
            } else {
                std::cout << "Update failed.\n";
            }
        } else {
            std::cout << "Your selection is invalid. Please try again.\n";
        }
    }
    return true;
}

bool UpdateValue(Movie &movie, int choice) {
    std::string inputString;
    int inputInt;
    switch (choice) {
        case 1:
            std::cout << "Enter new title: ";
            std::getline(std::cin, inputString);
            movie.setTitle(inputString);
            break;
        case 2:
            std::cout << "Enter new description: ";
            std::getline(std::cin, inputString);
            movie.setDescription(inputString);
            break;
        case 3:
            std::cout << "Enter new genre: ";
            std::getline(std::cin, inputString);
            movie.setGenre(inputString);
            break;
        case 4:
            std::cout << "Enter new director: ";
            std::getline(std::cin, inputString);
            movie.setDirector(inputString);
            break;
        case 5:
            std::cout << "Enter new cast: ";
            std::getline(std::cin, inputString);
            movie.setCast(inputString);
            break;
        case 6:
            std::cout << "Enter new duration: ";
            inputInt = inputInteger("Enter duration: ");
            movie.setDuration(inputInt);
            break;
        case 7:
            std::cout << "Enter new release date: ";
            std::getline(std::cin, inputString);
            movie.setReleaseDate(inputString);
            break;
        case 8:
            std::cout << "Enter new rating: ";
            inputInt = inputInteger("Enter rating: ");
            movie.setRating(inputInt);
            break;
        case 9:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            return false;
    }
    return true;
}

bool Movies::isMovieId(int movieId) const {
    for (const Movie &movie : movies) {
        if (movieId == movie.getMovieId()) {
            return true;
        }
    }
    return false;
}

bool Movies::deleteMovie(int movieId) {
    for (auto it = movies.begin(); it != movies.end(); ++it) {
        if (it->getMovieId() == movieId) {
            movies.erase(it);
            return true;
        }
    }
    return false;
}

bool Movies::saveToFile(const std::string &filename) const {
    if (!ensureDirectoryExists(filename)) {
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file for writing: " << filename
                  << std::endl;
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
        return false;  // If the directory doesn't exist and couldn't be
                       // created, return false
    }
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error opening file for writing: " << filename
                  << std::endl;
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
                      : movies.back().getMovieId() + 1;  // Auto-increment movieId
    std::string title, description, genre, director, cast;
    int duration;
    short day, month, year;
    int rating;

    std::cout << "Enter Title: ";
    std::cin.ignore();  // To clear any leftover newline character
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
    Date releaseDate(day, month, year);  // Create release date

    std::cout << "Enter Rating (out of 10): ";
    std::cin >> rating;

    Movie newMovie(movieId, title, description, genre, director, cast, duration,
                   releaseDate, rating);
    movies.push_back(newMovie);  // Add movie to the vector
    newMovie.saveToFile(MOVIE_FILE);
    std::cout << "Movie added with ID: " << movieId << "\n";
}

void Movies::deleteMovie() {
    // Display the last 50 movies
    std::cout << "\nMovies List (last 50 from bottom):\n";
    displayMovies();

    int movieId;
    std::cout << "Enter the Movie ID to delete: ";
    std::cin >> movieId;

    if (deleteMovie(movieId)) {
        std::cout << "Movie with ID " << movieId << " has been deleted.\n";
    } else {
        std::cout << "Movie with ID " << movieId << " not found.\n";
    }
}

void Movies::displayMovies() const {
    int count = 0;
    for (auto it = movies.begin(); it != movies.end() && count < 50;
         ++it, ++count) {
        it->display();
    }
}