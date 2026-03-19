#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

// Внутреннее (разделяемое) состояние фильма - неизменяемый объект
class MovieMetadata {
private:
    string title;               // Название фильма
    int duration;               // Длительность в минутах
    string genre;               // Жанр
    int ageRestriction;         // Возрастное ограничение
    string posterPath;          // Путь к постеру
    string description;         // Описание фильма
    string director;            // Режиссер
    vector<string> actors;      // Список актеров
    string country;             // Страна производства
    int year;                   // Год выпуска
    string studio;              // Студия-производитель

public:
    // Конструктор
    MovieMetadata(const string& title, int duration, const string& genre,
        int ageRestriction, const string& poster, const string& description,
        const string& director, const vector<string>& actors,
        const string& country, int year, const string& studio);

    // Геттеры (все методы константные, так как объект неизменяемый)
    string getTitle() const { return title; }
    int getDuration() const { return duration; }
    string getGenre() const { return genre; }
    int getAgeRestriction() const { return ageRestriction; }
    string getPosterPath() const { return posterPath; }
    string getDescription() const { return description; }
    string getDirector() const { return director; }
    vector<string> getActors() const { return actors; }
    string getCountry() const { return country; }
    int getYear() const { return year; }
    string getStudio() const { return studio; }

    // Получить ключ для кэширования (уникальный идентификатор фильма)
    string getCacheKey() const;

    // Вывод информации о фильме
    void displayInfo() const;
    void displayShortInfo() const;
};