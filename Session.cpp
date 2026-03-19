#define _CRT_SECURE_NO_WARNINGS
#include "Session.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Конструктор с time_t
Session::Session(time_t time, shared_ptr<Movie> movie, int hall)
    : startTime(time), movie(movie), hallNumber(hall), isAvailable(true) {}

// Конструктор со строкой времени (исправлен - создаем Movie с 5 параметрами)
Session::Session(const string& title, const string& timeStr, int duration)
    : movie(make_shared<Movie>(
        title,           // название
        duration,        // длительность
        "Неизвестно",    // жанр (по умолчанию)
        0,               // возрастное ограничение (по умолчанию)
        title + ".mp4"   // медиафайл
    )), hallNumber(1), isAvailable(true) {

    struct tm tm = {};
    stringstream ss(timeStr);
    ss >> get_time(&tm, "%H:%M");
    time_t now = time(nullptr);
    struct tm* today = localtime(&now);
    tm.tm_year = today->tm_year;
    tm.tm_mon = today->tm_mon;
    tm.tm_mday = today->tm_mday;
    startTime = mktime(&tm);
}

// Добавим еще один удобный конструктор для тестирования
Session::Session(const string& title, const string& timeStr, int duration,
    const string& genre, int ageRestriction)
    : movie(make_shared<Movie>(title, duration, genre, ageRestriction, title + ".mp4")),
    hallNumber(1), isAvailable(true) {

    struct tm tm = {};
    stringstream ss(timeStr);
    ss >> get_time(&tm, "%H:%M");
    time_t now = time(nullptr);
    struct tm* today = localtime(&now);
    tm.tm_year = today->tm_year;
    tm.tm_mon = today->tm_mon;
    tm.tm_mday = today->tm_mday;
    startTime = mktime(&tm);
}

string Session::getMovieTitle() const {
    return movie ? movie->getTitle() : "Неизвестный фильм";
}

int Session::getDuration() const {
    return movie ? movie->getDuration() : 0;
}

string Session::getMovieGenre() const {
    return movie ? movie->getGenre() : "Неизвестно";
}

int Session::getAgeRestriction() const {
    return movie ? movie->getAgeRestriction() : 0;
}

bool Session::isSameTime(const string& timeStr) const {
    struct tm tm = {};
    stringstream ss(timeStr);
    ss >> get_time(&tm, "%H:%M");

    struct tm* session_tm = localtime(&startTime);

    return (tm.tm_hour == session_tm->tm_hour &&
        tm.tm_min == session_tm->tm_min);
}

void Session::print() const {
    char buffer[80];
    struct tm* timeinfo = localtime(&startTime);
    strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);

    cout << "Сеанс: " << getMovieTitle()
        << " | Зал: " << hallNumber
        << " | Время: " << buffer
        << " | Длит: " << getDuration() << " мин"
        << " | Жанр: " << getMovieGenre()
        << " | Возраст: " << getAgeRestriction() << "+"
        << " | Статус: " << (isAvailable ? "Свободно" : "Занято") << endl;
}