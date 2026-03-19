#pragma once

#include <memory>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include "Movie.h"

using namespace std;

class Session {
private:
    time_t startTime;
    shared_ptr<Movie> movie;
    int hallNumber;
    bool isAvailable;

public:
    // Конструкторы
    Session(time_t time, shared_ptr<Movie> movie, int hall);
    Session(const string& title, const string& timeStr, int duration);
    Session(const string& title, const string& timeStr, int duration,
        const string& genre, int ageRestriction);  // Новый конструктор

    // Геттеры
    time_t getStartTime() const { return startTime; }
    string getMovieTitle() const;
    string getMovieGenre() const;        // Новый метод
    int getAgeRestriction() const;       // Новый метод
    int getHallNumber() const { return hallNumber; }
    bool getAvailability() const { return isAvailable; }
    int getDuration() const;

    // Методы
    void book() { isAvailable = false; }
    bool isSameTime(const string& timeStr) const;
    void print() const;
};