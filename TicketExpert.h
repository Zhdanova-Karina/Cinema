#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <ctime>
#include "Session.h"
#include "Movie.h"

using namespace std;

// Структура для хранения информации о зрителе
struct ViewerInfo {
    int age;
    bool isStudent;
    bool isPensioner;
    bool isVip;
    string promoCode;
};

// Структура для хранения информации о промокоде
struct PromoCode {
    string code;
    double discount;           // Скидка в процентах (0.15 = 15%)
    vector<string> validDays;   // Дни недели, когда действует
    int minTickets;             // Минимальное количество билетов
    int maxUses;                // Максимальное количество использований
    int currentUses;            // Текущее количество использований
    time_t validUntil;          // Срок действия
};

// Структура для хранения ценовых коэффициентов
struct PriceMultipliers {
    double sessionTypeMultiplier;    // Множитель за тип сеанса
    double formatMultiplier;         // Множитель за формат показа
    double timeMultiplier;           // Множитель за время сеанса
    double dayMultiplier;            // Множитель за день недели
    double vipMultiplier;            // Множитель за VIP-обслуживание
};

// Класс-эксперт по билетам
class TicketExpert {
private:
    // Базовая цена
    int basePrice;

    // Скидки для категорий зрителей
    map<string, double> categoryDiscounts;  // student, child, pensioner, group

    // Наценки за типы сеансов
    map<string, double> sessionTypeMarkup;   // Regular, Premiere, LateNight, KidsMorning

    // Наценки за форматы показа
    map<string, double> formatMarkup;        // 2D, 3D, IMAX

    // Множители по времени суток
    map<string, double> timeMultipliers;     // morning, afternoon, evening, night

    // Множители по дням недели
    map<string, double> dayMultipliers;      // monday, tuesday, etc.

    // Промокоды
    map<string, PromoCode> promoCodes;

    // Возрастные ограничения по умолчанию
    int defaultAgeRestriction;

    // Динамическое ценообразование
    bool dynamicPricingEnabled;
    double dynamicPricingFactor;
    int popularityThreshold;

    // Вспомогательные методы
    string getTimeOfDay(time_t time) const;
    string getDayOfWeek(time_t time) const;
    double calculateTimeMultiplier(time_t time) const;
    double calculateDayMultiplier(time_t time) const;
    double applyPromoCode(const string& code, double currentPrice, int ticketCount) const;
    bool checkAgeRestriction(const Session& session, int age) const;

public:
    // Конструктор
    TicketExpert(int basePrice = 300);

    // Настройка цен
    void setBasePrice(int price) { basePrice = price; }
    void setCategoryDiscount(const string& category, double discount);
    void setSessionTypeMarkup(const string& type, double markup);
    void setFormatMarkup(const string& format, double markup);
    void setTimeMultiplier(const string& timeSlot, double multiplier);
    void setDayMultiplier(const string& day, double multiplier);

    // Управление промокодами
    void addPromoCode(const PromoCode& promo);
    bool removePromoCode(const string& code);
    bool validatePromoCode(const string& code) const;
    void usePromoCode(const string& code);  // НОВЫЙ МЕТОД

    // Настройка динамического ценообразования
    void enableDynamicPricing(bool enable) { dynamicPricingEnabled = enable; }
    void setDynamicPricingParams(double factor, int threshold);

    // Основные методы расчета
    int calculatePrice(const Session& session, const ViewerInfo& viewer, int ticketCount = 1) const;
    int calculateBasePrice(const Session& session) const;
    double calculateDiscount(const ViewerInfo& viewer) const;
    PriceMultipliers getMultipliers(const Session& session) const;

    // Проверка возрастных ограничений
    bool canWatch(const Session& session, int age) const;
    vector<string> getRecommendedMovies(const vector<Movie>& movies, int age, const string& preferredGenre = "");

    // Статистика
    double getAveragePrice() const;
    int getRevenueForSession(const Session& session, int ticketsSold) const;

    // Информация о ценах
    void printPriceInfo() const;
    string getPriceBreakdown(const Session& session, const ViewerInfo& viewer) const;
};