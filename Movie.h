#pragma once

#include <string>

using namespace std;

class Movie {
private:
    string title;           // Название фильма
    int duration;           // Длительность в минутах
    string genre;           // Жанр (комедия, драма, боевик и т.д.)
    int ageRestriction;     // Возрастное ограничение (0, 6, 12, 16, 18)
    string mediaFile;       // Путь к файлу с фильмом для проектора

public:
    // Конструктор: создает фильм со всеми характеристиками
    Movie(const string& t, int dur, const string& gen, int age, const string& media);

    string getTitle() const;           // Получить название фильма
    int getDuration() const;            // Получить длительность
    string getGenre() const;            // Получить жанр
    int getAgeRestriction() const;      // Получить возрастное ограничение
    string getMediaFile() const;        // Получить путь к файлу фильма
};