#include "TicketExpert.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

TicketExpert::TicketExpert(int basePrice)
    : basePrice(basePrice), defaultAgeRestriction(0),
    dynamicPricingEnabled(false), dynamicPricingFactor(0.1),
    popularityThreshold(50) {

    // Инициализация скидок по умолчанию
    categoryDiscounts["student"] = 0.15;    // 15% студентам
    categoryDiscounts["child"] = 0.30;       // 30% детям
    categoryDiscounts["pensioner"] = 0.20;   // 20% пенсионерам
    categoryDiscounts["group"] = 0.10;       // 10% группам (от 5 человек)

    // Инициализация наценок за типы сеансов
    sessionTypeMarkup["Regular"] = 1.0;
    sessionTypeMarkup["Premiere"] = 1.3;
    sessionTypeMarkup["LateNight"] = 0.8;
    sessionTypeMarkup["KidsMorning"] = 0.7;

    // Инициализация наценок за форматы
    formatMarkup["2D"] = 1.0;
    formatMarkup["3D"] = 1.2;
    formatMarkup["IMAX"] = 1.5;

    // Инициализация множителей по времени суток
    timeMultipliers["morning"] = 0.7;    // 6:00-12:00
    timeMultipliers["afternoon"] = 0.9;  // 12:00-18:00
    timeMultipliers["evening"] = 1.2;    // 18:00-00:00
    timeMultipliers["night"] = 0.8;      // 00:00-06:00

    // Инициализация множителей по дням недели
    dayMultipliers["monday"] = 0.9;
    dayMultipliers["tuesday"] = 0.9;
    dayMultipliers["wednesday"] = 0.9;
    dayMultipliers["thursday"] = 0.9;
    dayMultipliers["friday"] = 1.1;
    dayMultipliers["saturday"] = 1.2;
    dayMultipliers["sunday"] = 1.1;
}

void TicketExpert::setCategoryDiscount(const string& category, double discount) {
    if (discount >= 0.0 && discount <= 0.5) {
        categoryDiscounts[category] = discount;
    }
}

void TicketExpert::setSessionTypeMarkup(const string& type, double markup) {
    if (markup >= 0.5 && markup <= 2.0) {
        sessionTypeMarkup[type] = markup;
    }
}

void TicketExpert::setFormatMarkup(const string& format, double markup) {
    if (markup >= 0.5 && markup <= 2.0) {
        formatMarkup[format] = markup;
    }
}

void TicketExpert::setTimeMultiplier(const string& timeSlot, double multiplier) {
    if (multiplier >= 0.5 && multiplier <= 1.5) {
        timeMultipliers[timeSlot] = multiplier;
    }
}

void TicketExpert::setDayMultiplier(const string& day, double multiplier) {
    if (multiplier >= 0.5 && multiplier <= 1.5) {
        dayMultipliers[day] = multiplier;
    }
}

void TicketExpert::addPromoCode(const PromoCode& promo) {
    promoCodes[promo.code] = promo;
}

bool TicketExpert::removePromoCode(const string& code) {
    return promoCodes.erase(code) > 0;
}

bool TicketExpert::validatePromoCode(const string& code) const {
    auto it = promoCodes.find(code);
    if (it == promoCodes.end()) return false;

    const auto& promo = it->second;

    // Проверка срока действия
    if (promo.validUntil < time(nullptr)) return false;

    // Проверка количества использований
    if (promo.maxUses > 0 && promo.currentUses >= promo.maxUses) return false;

    return true;
}

void TicketExpert::setDynamicPricingParams(double factor, int threshold) {
    dynamicPricingFactor = factor;
    popularityThreshold = threshold;
}

string TicketExpert::getTimeOfDay(time_t time) const {
    struct tm timeinfo;
    localtime_s(&timeinfo, &time);
    int hour = timeinfo.tm_hour;

    if (hour >= 6 && hour < 12) return "morning";
    if (hour >= 12 && hour < 18) return "afternoon";
    if (hour >= 18 && hour < 24) return "evening";
    return "night";
}

string TicketExpert::getDayOfWeek(time_t time) const {
    struct tm timeinfo;
    localtime_s(&timeinfo, &time);

    const char* days[] = { "sunday", "monday", "tuesday", "wednesday",
                          "thursday", "friday", "saturday" };
    return days[timeinfo.tm_wday];
}

double TicketExpert::calculateTimeMultiplier(time_t time) const {
    string timeOfDay = getTimeOfDay(time);
    auto it = timeMultipliers.find(timeOfDay);
    return (it != timeMultipliers.end()) ? it->second : 1.0;
}

double TicketExpert::calculateDayMultiplier(time_t time) const {
    string dayOfWeek = getDayOfWeek(time);
    auto it = dayMultipliers.find(dayOfWeek);
    return (it != dayMultipliers.end()) ? it->second : 1.0;
}

double TicketExpert::applyPromoCode(const string& code, double currentPrice, int ticketCount) const {
    if (!validatePromoCode(code)) return currentPrice;

    auto it = promoCodes.find(code);
    if (it == promoCodes.end()) return currentPrice;

    const PromoCode& promo = it->second;

    // Проверка минимального количества билетов
    if (ticketCount < promo.minTickets) return currentPrice;

    // Применяем скидку (только расчет, без изменения состояния)
    return currentPrice * (1.0 - promo.discount);
}

void TicketExpert::usePromoCode(const string& code) {
    auto it = promoCodes.find(code);
    if (it != promoCodes.end()) {
        it->second.currentUses++;
    }
}

bool TicketExpert::checkAgeRestriction(const Session& session, int age) const {
    return age >= session.getAgeRestriction();
}

PriceMultipliers TicketExpert::getMultipliers(const Session& session) const {
    PriceMultipliers multipliers;

    // Получаем тип сеанса (в реальном коде нужно получать из session)
    string sessionType = "Regular"; // Заглушка

    // Получаем формат (в реальном коде нужно получать из session)
    string format = "2D"; // Заглушка

    auto typeIt = sessionTypeMarkup.find(sessionType);
    multipliers.sessionTypeMultiplier = (typeIt != sessionTypeMarkup.end()) ? typeIt->second : 1.0;

    auto formatIt = formatMarkup.find(format);
    multipliers.formatMultiplier = (formatIt != formatMarkup.end()) ? formatIt->second : 1.0;

    multipliers.timeMultiplier = calculateTimeMultiplier(session.getStartTime());
    multipliers.dayMultiplier = calculateDayMultiplier(session.getStartTime());
    multipliers.vipMultiplier = 1.0; // По умолчанию

    return multipliers;
}

int TicketExpert::calculateBasePrice(const Session& session) const {
    PriceMultipliers multipliers = getMultipliers(session);

    double price = basePrice;
    price *= multipliers.sessionTypeMultiplier;
    price *= multipliers.formatMultiplier;
    price *= multipliers.timeMultiplier;
    price *= multipliers.dayMultiplier;

    return static_cast<int>(price);
}

double TicketExpert::calculateDiscount(const ViewerInfo& viewer) const {
    double totalDiscount = 0.0;

    if (viewer.isStudent) {
        auto it = categoryDiscounts.find("student");
        if (it != categoryDiscounts.end()) totalDiscount += it->second;
    }

    if (viewer.isPensioner) {
        auto it = categoryDiscounts.find("pensioner");
        if (it != categoryDiscounts.end()) totalDiscount += it->second;
    }

    if (viewer.age < 14) {
        auto it = categoryDiscounts.find("child");
        if (it != categoryDiscounts.end()) totalDiscount += it->second;
    }

    // Не даем скидке превысить 50%
    return min(totalDiscount, 0.5);
}

int TicketExpert::calculatePrice(const Session& session, const ViewerInfo& viewer, int ticketCount) const {
    // Проверка возрастных ограничений
    if (!checkAgeRestriction(session, viewer.age)) {
        return -1; // Нельзя смотреть
    }

    // Базовая цена
    double price = calculateBasePrice(session);

    // Применяем скидку за VIP (если есть)
    if (viewer.isVip) {
        price *= 1.2; // 20% наценка за VIP-обслуживание
    }

    // Применяем скидку за категорию зрителя
    double discount = calculateDiscount(viewer);
    price *= (1.0 - discount);

    // Применяем промокод (только расчет цены, без изменения состояния)
    if (!viewer.promoCode.empty()) {
        price = applyPromoCode(viewer.promoCode, price, ticketCount);
        // ВНИМАНИЕ: счетчик использований НЕ увеличивается здесь
        // Это нужно делать отдельно через usePromoCode
    }

    // Динамическое ценообразование (упрощенно)
    if (dynamicPricingEnabled) {
        // Здесь можно добавить логику динамического ценообразования
    }

    // Умножаем на количество билетов
    return static_cast<int>(price * ticketCount);
}

bool TicketExpert::canWatch(const Session& session, int age) const {
    return age >= session.getAgeRestriction();
}

vector<string> TicketExpert::getRecommendedMovies(const vector<Movie>& movies, int age, const string& preferredGenre) {
    vector<string> recommendations;

    for (const auto& movie : movies) {
        // Проверяем возрастное ограничение
        if (age < movie.getAgeRestriction()) continue;

        // Если указан предпочитаемый жанр, фильтруем по нему
        if (!preferredGenre.empty() && movie.getGenre() != preferredGenre) continue;

        recommendations.push_back(movie.getTitle());
    }

    return recommendations;
}

double TicketExpert::getAveragePrice() const {
    // В реальном коде здесь должен быть расчет на основе статистики
    return basePrice * 1.1; // Заглушка
}

int TicketExpert::getRevenueForSession(const Session& session, int ticketsSold) const {
    ViewerInfo averageViewer;
    averageViewer.age = 30;
    averageViewer.isStudent = false;
    averageViewer.isPensioner = false;
    averageViewer.isVip = false;

    int pricePerTicket = calculatePrice(session, averageViewer, 1);
    return pricePerTicket * ticketsSold;
}

void TicketExpert::printPriceInfo() const {
    cout << "\n=== ИНФОРМАЦИЯ О ЦЕНАХ ===" << endl;
    cout << "Базовая цена: " << basePrice << " руб." << endl;

    cout << "\nСкидки:" << endl;
    for (auto it = categoryDiscounts.begin(); it != categoryDiscounts.end(); ++it) {
        cout << "  " << it->first << ": " << it->second * 100 << "%" << endl;
    }

    cout << "\nНаценки за типы сеансов:" << endl;
    for (auto it = sessionTypeMarkup.begin(); it != sessionTypeMarkup.end(); ++it) {
        cout << "  " << it->first << ": x" << it->second << endl;
    }

    cout << "\nНаценки за форматы:" << endl;
    for (auto it = formatMarkup.begin(); it != formatMarkup.end(); ++it) {
        cout << "  " << it->first << ": x" << it->second << endl;
    }
}

string TicketExpert::getPriceBreakdown(const Session& session, const ViewerInfo& viewer) const {
    stringstream ss;

    PriceMultipliers multipliers = getMultipliers(session);
    double discount = calculateDiscount(viewer);

    ss << "\n=== РАСЧЕТ ЦЕНЫ БИЛЕТА ===" << endl;
    ss << "Базовая цена: " << basePrice << " руб." << endl;
    ss << "Множитель типа сеанса: x" << multipliers.sessionTypeMultiplier << endl;
    ss << "Множитель формата: x" << multipliers.formatMultiplier << endl;
    ss << "Множитель времени: x" << multipliers.timeMultiplier << endl;
    ss << "Множитель дня: x" << multipliers.dayMultiplier << endl;
    ss << "Промежуточная цена: " << calculateBasePrice(session) << " руб." << endl;
    ss << "Скидка: " << discount * 100 << "%" << endl;

    if (viewer.isVip) {
        ss << "VIP-наценка: +20%" << endl;
    }

    int finalPrice = calculatePrice(session, viewer, 1);
    ss << "ИТОГОВАЯ ЦЕНА: " << finalPrice << " руб." << endl;

    return ss.str();
}