#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <ctime>
#include "Cinema.h"
#include "Hall.h"
#include "Session.h"
#include "Movie.h"
#include "MovieMetadataFactory.h"
#include "SessionType.h"
#include "TicketExpert.h"

using namespace std;

// Структура для хранения результата операции
struct OperationResult {
    bool success;
    string message;
    map<string, string> data;
};

// Структура для конфигурации сценария
struct ScenarioConfig {
    string name;
    string startTime;
    vector<string> actions;
    vector<int> halls;
    double priceMultiplier;
};

// Главный фасад кинотеатра
class CinemaFacade {
private:
    // Основные компоненты системы
    unique_ptr<Cinema> cinema;
    unique_ptr<MovieMetadataFactory> movieFactory;
    vector<unique_ptr<SessionType>> sessionTypes;

    // Эксперты
    unique_ptr<TicketExpert> ticketExpert;

    // Конфигурация
    map<string, ScenarioConfig> scenarios;
    string logLevel;
    bool detailedLogging;

    // Вспомогательные методы
    void log(const string& message, const string& level = "info");
    void initializeDefaultScenarios();
    bool validateSessionInput(const string& movieTitle, const string& time, int hallNumber);
    string getCurrentTime();
    void notifyStaff(const string& message);
    void autoSaveState();

public:
    // Конструктор
    CinemaFacade(const string& name, const string& address, const string& hours);

    // ========== УПРАВЛЕНИЕ ФИЛЬМАМИ ==========
    bool addMovie(const string& title, int duration, const string& genre,
        int ageRestriction, const string& poster, const string& description);
    void listNowShowing() const;
    vector<string> searchMovies(const string& query) const;
    vector<string> getMoviesByGenre(const string& genre) const;
    string getMovieDetails(const string& title) const;

    // ========== УПРАВЛЕНИЕ ЗАЛАМИ ==========
    bool createStandardHall(int number, int capacity);
    bool createVipHall(int number, int capacity);
    bool createImaxHall(int number, int capacity);
    bool createHallGroup(const string& groupName, const vector<int>& hallNumbers);
    string getHallStatus(int hallNumber) const;
    vector<int> getAvailableHalls(const string& time) const;
    int getTotalCapacity() const;

    // ========== УПРАВЛЕНИЕ СЕАНСАМИ ==========
    bool createSession(const string& movieTitle, const string& time,
        int hallNumber, const string& sessionType = "Regular",
        const string& format = "2D");
    bool cancelSession(int sessionId);
    vector<string> getSchedule(const string& date) const;
    vector<string> findSessions(const string& movieTitle) const;
    vector<string> findAvailableSessions(const string& time) const;
    int getTodaySessionsCount() const;

    // ========== УПРАВЛЕНИЕ БИЛЕТАМИ ==========
    OperationResult bookTicket(const string& movieTitle, const string& time,
        int hallNumber, int age, bool isStudent = false,
        bool isVip = false, const string& promoCode = "");
    int calculateTicketPrice(const string& movieTitle, const string& time,
        int hallNumber, int age, bool isStudent, bool isVip);
    bool cancelBooking(int bookingId);

    // ========== СЦЕНАРИИ РАБОТЫ ==========
    bool runScenario(const string& scenarioName);
    bool runEveningProgram();
    bool runWeekendMarathon();
    void emergencyShutdown();
    void addScenario(const ScenarioConfig& scenario);

    // ========== СТАТИСТИКА И ОТЧЕТЫ ==========
    void printStatistics() const;
    string generateDailyReport(const string& date) const;
    double getTotalRevenue() const;

    // ========== ИНФОРМАЦИЯ О КИНОТЕАТРЕ ==========
    string getCinemaInfo() const;
    string getName() const { return cinema->getName(); }
    string getAddress() const { return cinema->getAddress(); }
    string getWorkingHours() const { return cinema->getWorkingHours(); }
};