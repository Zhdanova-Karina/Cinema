#pragma once

#include "MovieMetadata.h"
#include <memory>
#include <ctime>

using namespace std;

// Внешнее состояние фильма (уникальное для каждого сеанса)
class LightweightMovie {
private:
    shared_ptr<MovieMetadata> metadata;  // Ссылка на разделяемые метаданные
    time_t releaseDate;                   // Дата выхода в прокат
    bool isNowShowing;                     // Сейчас в прокате
    int ticketsSold;                        // Количество проданных билетов
    int hallNumber;                          // Номер зала
    float rating;                             // Рейтинг (пользовательский)
    int sessionId;                             // ID сеанса

public:
    // Конструктор
    LightweightMovie(shared_ptr<MovieMetadata> meta, time_t release,
        bool showing, int hall = 0);

    // Геттеры для внутреннего состояния (делегируются метаданным)
    string getTitle() const { return metadata->getTitle(); }
    int getDuration() const { return metadata->getDuration(); }
    string getGenre() const { return metadata->getGenre(); }
    int getAgeRestriction() const { return metadata->getAgeRestriction(); }
    string getPosterPath() const { return metadata->getPosterPath(); }
    string getDescription() const { return metadata->getDescription(); }
    string getDirector() const { return metadata->getDirector(); }
    vector<string> getActors() const { return metadata->getActors(); }

    // Геттеры для внешнего состояния
    time_t getReleaseDate() const { return releaseDate; }
    bool isShowing() const { return isNowShowing; }
    int getTicketsSold() const { return ticketsSold; }
    int getHallNumber() const { return hallNumber; }
    float getRating() const { return rating; }
    int getSessionId() const { return sessionId; }

    // Сеттеры для внешнего состояния
    void setHallNumber(int hall) { hallNumber = hall; }
    void setShowing(bool showing) { isNowShowing = showing; }
    void setRating(float newRating) { rating = newRating; }
    void setSessionId(int id) { sessionId = id; }

    // Операции с билетами
    void sellTicket(int count = 1) { ticketsSold += count; }
    void refundTicket(int count = 1) { ticketsSold = max(0, ticketsSold - count); }

    // Получить полную информацию о фильме
    void displayFullInfo() const;

    // Получить краткую информацию о сеансе
    void displaySessionInfo() const;

    // Проверка доступности
    bool isAvailable() const { return isNowShowing; }

    // Получить заполненность зала в процентах
    double getHallFillPercentage(int totalSeats) const {
        return (static_cast<double>(ticketsSold) / totalSeats) * 100;
    }
};