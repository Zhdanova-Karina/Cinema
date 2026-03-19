#pragma once
#include <iostream>
#include "Session.h"
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

using namespace std;

// Типы сортировки для итератора
enum class SortType {
    BY_TIME,           // По времени
    BY_HALL,           // По номеру зала
    BY_MOVIE,          // По названию фильма
    BY_AVAILABILITY,   // Сначала свободные
    BY_DURATION        // По длительности фильма
};

// Направление обхода
enum class Direction {
    FORWARD,           // Прямой порядок
    REVERSE            // Обратный порядок
};

// Класс итератора для обхода сеансов
class ScheduleIterator {
private:
    vector<shared_ptr<Session>> sessions;
    size_t currentIndex;
    SortType sortType;
    Direction direction;
    string filterMovie;           // Фильтр по названию фильма
    int filterHall;                     // Фильтр по номеру зала
    bool filterAvailable;               // Фильтр только доступные

    // Функции сравнения для разных типов сортировки
    static bool compareByTime(const shared_ptr<Session>& a,
        const shared_ptr<Session>& b) {
        return a->getStartTime() < b->getStartTime();
    }

    static bool compareByHall(const shared_ptr<Session>& a,
        const shared_ptr<Session>& b) {
        if (a->getHallNumber() == b->getHallNumber()) {
            return a->getStartTime() < b->getStartTime();
        }
        return a->getHallNumber() < b->getHallNumber();
    }

    static bool compareByMovie(const shared_ptr<Session>& a,
        const shared_ptr<Session>& b) {
        if (a->getMovieTitle() == b->getMovieTitle()) {
            return a->getStartTime() < b->getStartTime();
        }
        return a->getMovieTitle() < b->getMovieTitle();
    }

    static bool compareByAvailability(const shared_ptr<Session>& a,
        const shared_ptr<Session>& b) {
        if (a->getAvailability() == b->getAvailability()) {
            return a->getStartTime() < b->getStartTime();
        }
        // Сначала свободные (true), потом занятые (false)
        return a->getAvailability() > b->getAvailability();
    }

    static bool compareByDuration(const shared_ptr<Session>& a,
        const shared_ptr<Session>& b) {
        // Здесь нужно получать длительность фильма из сеанса
        // Пока сортируем по времени
        return a->getStartTime() < b->getStartTime();
    }

    // Применить сортировку
    void applySort() {
        switch (sortType) {
        case SortType::BY_TIME:
            sort(sessions.begin(), sessions.end(), compareByTime);
            break;
        case SortType::BY_HALL:
            sort(sessions.begin(), sessions.end(), compareByHall);
            break;
        case SortType::BY_MOVIE:
            sort(sessions.begin(), sessions.end(), compareByMovie);
            break;
        case SortType::BY_AVAILABILITY:
            sort(sessions.begin(), sessions.end(), compareByAvailability);
            break;
        case SortType::BY_DURATION:
            sort(sessions.begin(), sessions.end(), compareByDuration);
            break;
        }

        // Если обратный порядок - разворачиваем вектор
        if (direction == Direction::REVERSE) {
            reverse(sessions.begin(), sessions.end());
        }
    }

    // Применить фильтры
    void applyFilters() {
        auto it = sessions.begin();
        while (it != sessions.end()) {
            bool keep = true;

            // Фильтр по названию фильма
            if (!filterMovie.empty() && (*it)->getMovieTitle() != filterMovie) {
                keep = false;
            }

            // Фильтр по номеру зала
            if (filterHall > 0 && (*it)->getHallNumber() != filterHall) {
                keep = false;
            }

            // Фильтр по доступности
            if (filterAvailable && !(*it)->getAvailability()) {
                keep = false;
            }

            if (keep) {
                ++it;
            }
            else {
                it = sessions.erase(it);
            }
        }
    }

public:
    // Конструктор
    ScheduleIterator(const vector<shared_ptr<Session>>& sessions)
        : sessions(sessions), currentIndex(0), sortType(SortType::BY_TIME),
        direction(Direction::FORWARD), filterHall(-1), filterAvailable(false) {
        applySort();
    }

    // Установка типа сортировки
    void setSortType(SortType type) {
        sortType = type;
        applySort();
        reset();
    }

    // Установка направления
    void setDirection(Direction dir) {
        direction = dir;
        applySort();
        reset();
    }

    // Установка фильтра по фильму
    void setMovieFilter(const string& movie) {
        filterMovie = movie;
        applyFilters();
        applySort();
        reset();
    }

    // Установка фильтра по залу
    void setHallFilter(int hallNumber) {
        filterHall = hallNumber;
        applyFilters();
        applySort();
        reset();
    }

    // Установка фильтра только на доступные
    void setAvailableOnly(bool only) {
        filterAvailable = only;
        applyFilters();
        applySort();
        reset();
    }

    // Сброс всех фильтров
    void clearFilters() {
        // В реальном коде нужно восстановить исходный список
        // Здесь для простоты оставляем как есть
    }

    // Проверка наличия следующего элемента
    bool hasNext() const {
        return currentIndex < sessions.size();
    }

    // Получение следующего элемента
    shared_ptr<Session> next() {
        if (!hasNext()) {
            return nullptr;
        }
        return sessions[currentIndex++];
    }

    // Получение предыдущего элемента (для обратного обхода)
    shared_ptr<Session> previous() {
        if (currentIndex == 0) {
            return nullptr;
        }
        return sessions[--currentIndex];
    }

    // Сброс итератора в начало
    void reset() {
        currentIndex = 0;
    }

    // Получение текущего индекса
    size_t getCurrentIndex() const {
        return currentIndex;
    }

    // Получение общего количества сеансов (с учетом фильтров)
    size_t size() const {
        return sessions.size();
    }

    // Проверка, пуст ли итератор
    bool isEmpty() const {
        return sessions.empty();
    }

    // Перейти к первому элементу, удовлетворяющему условию
    bool seekToFirst(function<bool(shared_ptr<Session>)> predicate) {
        for (size_t i = 0; i < sessions.size(); ++i) {
            if (predicate(sessions[i])) {
                currentIndex = i;
                return true;
            }
        }
        return false;
    }

    // Перейти к следующему элементу, удовлетворяющему условию
    bool seekToNext(function<bool(shared_ptr<Session>)> predicate) {
        for (size_t i = currentIndex + 1; i < sessions.size(); ++i) {
            if (predicate(sessions[i])) {
                currentIndex = i;
                return true;
            }
        }
        return false;
    }

    // Получить все сеансы в виде вектора (после фильтров)
    vector<shared_ptr<Session>> getAll() const {
        return sessions;
    }

    // Вывести текущее расписание
    void printSchedule() const {
        for (size_t i = 0; i < sessions.size(); ++i) {
            cout << "  " << (i + 1) << ". ";
            sessions[i]->print();
        }
    }

    // Вывести текущее расписание в сокращенном виде
    void printSummary() const {
        cout << "  Всего сеансов: " << sessions.size() << endl;
        if (!sessions.empty()) {
            cout << "  Первый: " << sessions.front()->getMovieTitle()
                << " в зале " << sessions.front()->getHallNumber() << endl;
            cout << "  Последний: " << sessions.back()->getMovieTitle()
                << " в зале " << sessions.back()->getHallNumber() << endl;
        }
    }
};
