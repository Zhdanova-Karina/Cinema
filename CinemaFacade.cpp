#include "CinemaFacade.h"
#include "Hall.h"
#include "HallGroup.h"
#include "HallLeaf.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>

using namespace std;

// Конструктор
CinemaFacade::CinemaFacade(const string& name, const string& address, const string& hours) {
    cinema = make_unique<Cinema>(name, address, hours);
    movieFactory = make_unique<MovieMetadataFactory>();

    // Инициализация экспертов
    ticketExpert = make_unique<TicketExpert>(300);

    logLevel = "info";
    detailedLogging = true;

    // Инициализация сценариев по умолчанию
    initializeDefaultScenarios();

    log("Фасад инициализирован: " + name);
}

// Логирование
void CinemaFacade::log(const string& message, const string& level) {
    if (level == "info" && logLevel == "error") return;

    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    char buffer[80];
    ctime_s(buffer, sizeof(buffer), &time);
    string timeStr(buffer);
    timeStr = timeStr.substr(0, timeStr.length() - 1);

    cout << "[" << timeStr << "] [" << level << "] " << message << endl;
}

// Инициализация сценариев по умолчанию
void CinemaFacade::initializeDefaultScenarios() {
    ScenarioConfig evening;
    evening.name = "evening";
    evening.startTime = "19:00";
    evening.actions = { "dim_lights 15", "increase_volume 80", "start_laser_show" };
    evening.halls = { 1, 2, 3 };
    evening.priceMultiplier = 1.2;
    scenarios["evening"] = evening;

    ScenarioConfig morning;
    morning.name = "kids_morning";
    morning.startTime = "10:00";
    morning.actions = { "bright_lights 50", "reduce_volume 60", "disable_3d", "activate_kids_mode" };
    morning.halls = { 1, 3 };
    morning.priceMultiplier = 0.7;
    scenarios["kids_morning"] = morning;

    ScenarioConfig weekend;
    weekend.name = "weekend_marathon";
    weekend.startTime = "12:00";
    weekend.actions = { "special_offer", "extra_clean", "staff_ready" };
    weekend.halls = { 1, 2, 3 };
    weekend.priceMultiplier = 1.1;
    scenarios["weekend_marathon"] = weekend;
}

bool CinemaFacade::validateSessionInput(const string& movieTitle, const string& time, int hallNumber) {
    Hall* hall = cinema->getHall(hallNumber);
    if (!hall) {
        log("Ошибка: зал " + to_string(hallNumber) + " не найден", "error");
        return false;
    }

    return true;
}

string CinemaFacade::getCurrentTime() {
    auto now = chrono::system_clock::now();
    auto time = chrono::system_clock::to_time_t(now);
    char buffer[80];
    ctime_s(buffer, sizeof(buffer), &time);
    return string(buffer);
}

void CinemaFacade::notifyStaff(const string& message) {
    cout << "[УВЕДОМЛЕНИЕ ПЕРСОНАЛА] " << message << endl;
}

void CinemaFacade::autoSaveState() {
    log("Автосохранение состояния системы");
    // Здесь можно добавить сохранение в файл
}

// ========== УПРАВЛЕНИЕ ФИЛЬМАМИ ==========

bool CinemaFacade::addMovie(const string& title, int duration, const string& genre,
    int ageRestriction, const string& poster, const string& description) {
    log("Добавление фильма: " + title);

    auto metadata = movieFactory->getMetadata(title, duration, genre, ageRestriction,
        poster, description);

    if (metadata) {
        log("Фильм добавлен: " + title + " (" + to_string(duration) + " мин., " + genre + ", " + to_string(ageRestriction) + "+)");
        return true;
    }

    log("Ошибка при добавлении фильма", "error");
    return false;
}

void CinemaFacade::listNowShowing() const {
    cout << "\n=== СЕЙЧАС В ПРОКАТЕ ===" << endl;

    auto iterator = cinema->getAllSessionsIterator();
    map<string, int> movieCount;

    while (iterator.hasNext()) {
        auto session = iterator.next();
        string title = session->getMovieTitle();
        movieCount[title]++;
    }

    for (auto it = movieCount.begin(); it != movieCount.end(); ++it) {
        cout << "  • " << it->first << " - " << it->second << " сеансов" << endl;
    }
}

vector<string> CinemaFacade::searchMovies(const string& query) const {
    vector<string> results;

    auto iterator = cinema->getAllSessionsIterator();
    map<string, bool> uniqueMovies;

    while (iterator.hasNext()) {
        auto session = iterator.next();
        string title = session->getMovieTitle();

        if (title.find(query) != string::npos && !uniqueMovies[title]) {
            results.push_back(title);
            uniqueMovies[title] = true;
        }
    }

    return results;
}

vector<string> CinemaFacade::getMoviesByGenre(const string& genre) const {
    vector<string> results;

    auto iterator = cinema->getAllSessionsIterator();
    map<string, bool> uniqueMovies;

    while (iterator.hasNext()) {
        auto session = iterator.next();
        // В реальном коде нужно получать жанр из фильма
        // Здесь заглушка
        if (!uniqueMovies[session->getMovieTitle()]) {
            results.push_back(session->getMovieTitle());
            uniqueMovies[session->getMovieTitle()] = true;
        }
    }

    return results;
}

string CinemaFacade::getMovieDetails(const string& title) const {
    stringstream ss;
    ss << "\n=== ДЕТАЛИ ФИЛЬМА: " << title << " ===" << endl;

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setMovieFilter(title);

    if (iterator.hasNext()) {
        auto session = iterator.next();
        ss << "Название: " << session->getMovieTitle() << endl;
        ss << "Длительность: " << session->getDuration() << " мин." << endl;
        ss << "Возрастное ограничение: " << session->getAgeRestriction() << "+" << endl;
        ss << "\nРасписание сеансов:" << endl;

        iterator.reset();
        while (iterator.hasNext()) {
            auto s = iterator.next();
            char buffer[80];
            time_t time = s->getStartTime();
            struct tm timeinfo;
            localtime_s(&timeinfo, &time);
            strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
            ss << "  • Зал " << s->getHallNumber() << " в " << buffer
                << " - " << (s->getAvailability() ? "Свободно" : "Занято") << endl;
        }
    }
    else {
        ss << "Фильм не найден в расписании" << endl;
    }

    return ss.str();
}

// ========== УПРАВЛЕНИЕ ЗАЛАМИ ==========

bool CinemaFacade::createStandardHall(int number, int capacity) {
    log("Создание стандартного зала №" + to_string(number) + " (вместимость: " + to_string(capacity) + ")");

    auto hall = make_unique<StandardHall>(number, capacity);
    cinema->addHall(move(hall));

    return true;
}

bool CinemaFacade::createVipHall(int number, int capacity) {
    log("Создание VIP зала №" + to_string(number) + " (вместимость: " + to_string(capacity) + ")");

    auto hall = make_unique<VipHall>(number, capacity);
    cinema->addHall(move(hall));

    return true;
}

bool CinemaFacade::createImaxHall(int number, int capacity) {
    log("Создание IMAX зала №" + to_string(number) + " (вместимость: " + to_string(capacity) + ")");

    auto hall = make_unique<ImaxHall>(number, capacity);
    cinema->addHall(move(hall));

    return true;
}

bool CinemaFacade::createHallGroup(const string& groupName, const vector<int>& hallNumbers) {
    log("Создание группы залов: " + groupName);

    auto group = make_shared<HallGroup>(groupName, "Группа залов");

    for (int num : hallNumbers) {
        Hall* hall = cinema->getHall(num);
        if (hall) {
            // В реальном коде нужно создать HallLeaf из существующего зала
            // Здесь упрощенно
            cout << "  Добавлен зал " << num << " в группу " << groupName << endl;
        }
    }

    cinema->addComponent(group);
    return true;
}

string CinemaFacade::getHallStatus(int hallNumber) const {
    Hall* hall = cinema->getHall(hallNumber);
    if (!hall) {
        return "Зал не найден";
    }

    stringstream ss;
    ss << "\n=== СТАТУС ЗАЛА №" << hallNumber << " ===" << endl;
    ss << "Тип: ";

    if (dynamic_cast<ImaxHall*>(hall)) ss << "IMAX";
    else if (dynamic_cast<VipHall*>(hall)) ss << "VIP";
    else ss << "Стандартный";

    ss << "\nВместимость: " << hall->getCapacity() << " мест" << endl;
    ss << "Статус: " << hall->getStatus() << endl;

    auto sessions = hall->getSchedule();
    ss << "Сеансов сегодня: " << sessions.size() << endl;

    return ss.str();
}

vector<int> CinemaFacade::getAvailableHalls(const string& time) const {
    vector<int> available;

    auto halls = cinema->getAllHalls();
    for (auto* hall : halls) {
        if (hall->isAvailable(time)) {
            available.push_back(hall->getNumber());
        }
    }

    return available;
}

int CinemaFacade::getTotalCapacity() const {
    return cinema->getTotalCapacity();
}

// ========== УПРАВЛЕНИЕ СЕАНСАМИ ==========

bool CinemaFacade::createSession(const string& movieTitle, const string& time,
    int hallNumber, const string& sessionType,
    const string& format) {
    if (!validateSessionInput(movieTitle, time, hallNumber)) {
        return false;
    }

    log("Создание сеанса: " + movieTitle + " в " + time + ", зал №" + to_string(hallNumber) +
        ", тип: " + sessionType + ", формат: " + format);

    // Создание сеанса
    Session session(movieTitle, time, 120, "Неизвестно", 0);
    Hall* hall = cinema->getHall(hallNumber);
    hall->addSession(session);

    log("Сеанс создан");
    return true;
}

bool CinemaFacade::cancelSession(int sessionId) {
    log("Отмена сеанса ID: " + to_string(sessionId));
    // В реальном коде нужно найти и удалить сеанс по ID
    return true;
}

vector<string> CinemaFacade::getSchedule(const string& date) const {
    vector<string> result;
    result.push_back("=== РАСПИСАНИЕ НА " + date + " ===");

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setSortType(SortType::BY_TIME);

    int count = 1;
    while (iterator.hasNext()) {
        auto session = iterator.next();
        stringstream ss;

        time_t time = session->getStartTime();
        struct tm timeinfo;
        localtime_s(&timeinfo, &time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);

        ss << count++ << ". " << buffer << " | Зал " << session->getHallNumber()
            << " | " << session->getMovieTitle()
            << " (" << session->getDuration() << " мин.)"
            << " | " << (session->getAvailability() ? "Свободно" : "Занято");

        result.push_back(ss.str());
    }

    return result;
}

vector<string> CinemaFacade::findSessions(const string& movieTitle) const {
    vector<string> result;

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setMovieFilter(movieTitle);
    iterator.setSortType(SortType::BY_TIME);

    while (iterator.hasNext()) {
        auto session = iterator.next();
        stringstream ss;

        time_t time = session->getStartTime();
        struct tm timeinfo;
        localtime_s(&timeinfo, &time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);

        ss << "  Зал " << session->getHallNumber() << " в " << buffer
            << " - " << (session->getAvailability() ? "Свободно" : "Занято");

        result.push_back(ss.str());
    }

    return result;
}

vector<string> CinemaFacade::findAvailableSessions(const string& time) const {
    vector<string> result;

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setAvailableOnly(true);
    iterator.setSortType(SortType::BY_TIME);

    while (iterator.hasNext()) {
        auto session = iterator.next();
        stringstream ss;

        time_t sessionTime = session->getStartTime();
        struct tm timeinfo;
        localtime_s(&timeinfo, &sessionTime);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);

        ss << "  " << session->getMovieTitle() << " в " << buffer
            << ", зал " << session->getHallNumber();

        result.push_back(ss.str());
    }

    return result;
}

int CinemaFacade::getTodaySessionsCount() const {
    auto iterator = cinema->getAllSessionsIterator();
    int count = 0;
    while (iterator.hasNext()) {
        iterator.next();
        count++;
    }
    return count;
}

// ========== УПРАВЛЕНИЕ БИЛЕТАМИ ==========

OperationResult CinemaFacade::bookTicket(const string& movieTitle, const string& time,
    int hallNumber, int age, bool isStudent,
    bool isVip, const string& promoCode) {
    OperationResult result;

    log("Бронирование билета: " + movieTitle + " в " + time + ", зал №" + to_string(hallNumber));

    // Поиск сеанса
    Hall* hall = cinema->getHall(hallNumber);
    if (!hall) {
        result.success = false;
        result.message = "Зал не найден";
        return result;
    }

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setMovieFilter(movieTitle);
    iterator.setHallFilter(hallNumber);

    if (!iterator.hasNext()) {
        result.success = false;
        result.message = "Сеанс не найден";
        return result;
    }

    auto session = iterator.next();

    // Проверка возрастных ограничений через эксперта
    if (!ticketExpert->canWatch(*session, age)) {
        result.success = false;
        result.message = "Возрастное ограничение " + to_string(session->getAgeRestriction()) +
            "+ не позволяет просмотр в возрасте " + to_string(age) + " лет";
        return result;
    }

    // Проверка доступности
    if (!session->getAvailability()) {
        result.success = false;
        result.message = "Билеты на этот сеанс уже проданы";
        return result;
    }

    // Расчет цены
    ViewerInfo viewer;
    viewer.age = age;
    viewer.isStudent = isStudent;
    viewer.isPensioner = (age > 65);
    viewer.isVip = isVip;
    viewer.promoCode = promoCode;

    int price = ticketExpert->calculatePrice(*session, viewer, 1);

    // Бронирование
    session->book();

    result.success = true;
    result.message = "Билет успешно забронирован";
    result.data["price"] = to_string(price);
    result.data["booking_id"] = to_string(rand() % 10000);
    result.data["movie"] = movieTitle;
    result.data["hall"] = to_string(hallNumber);
    result.data["time"] = time;

    log("Билет забронирован. Цена: " + to_string(price) + " руб.");

    return result;
}

int CinemaFacade::calculateTicketPrice(const string& movieTitle, const string& time,
    int hallNumber, int age, bool isStudent, bool isVip) {
    auto iterator = cinema->getAllSessionsIterator();
    iterator.setMovieFilter(movieTitle);
    iterator.setHallFilter(hallNumber);

    if (!iterator.hasNext()) {
        return 0;
    }

    auto session = iterator.next();

    ViewerInfo viewer;
    viewer.age = age;
    viewer.isStudent = isStudent;
    viewer.isPensioner = (age > 65);
    viewer.isVip = isVip;
    viewer.promoCode = "";

    return ticketExpert->calculatePrice(*session, viewer, 1);
}

bool CinemaFacade::cancelBooking(int bookingId) {
    log("Отмена бронирования ID: " + to_string(bookingId));
    // В реальном коде нужно найти бронь по ID и отменить
    return true;
}

// ========== СЦЕНАРИИ РАБОТЫ ==========

bool CinemaFacade::runScenario(const string& scenarioName) {
    auto it = scenarios.find(scenarioName);
    if (it == scenarios.end()) {
        log("Сценарий не найден: " + scenarioName, "error");
        return false;
    }

    auto& scenario = it->second;
    log("Запуск сценария: " + scenario.name);

    cout << "\n=== СЦЕНАРИЙ: " << scenario.name << " ===" << endl;
    cout << "Время начала: " << scenario.startTime << endl;
    cout << "Задействованы залы: ";
    for (int h : scenario.halls) cout << h << " ";
    cout << endl;
    cout << "Выполняемые действия:" << endl;

    for (const auto& action : scenario.actions) {
        cout << "  • " << action << endl;

        // Имитация выполнения действий
        if (action.find("dim_lights") != string::npos) {
            // Управление освещением
        }
        else if (action.find("increase_volume") != string::npos) {
            // Управление звуком
        }
        else if (action.find("start_laser_show") != string::npos) {
            cout << "    Лазерное шоу запущено" << endl;
        }
        else if (action.find("activate_kids_mode") != string::npos) {
            cout << "    Детский режим активирован" << endl;
        }
    }

    log("Сценарий завершен");
    return true;
}

bool CinemaFacade::runEveningProgram() {
    return runScenario("evening");
}

bool CinemaFacade::runWeekendMarathon() {
    return runScenario("weekend_marathon");
}

void CinemaFacade::emergencyShutdown() {
    log("АВАРИЙНОЕ ЗАВЕРШЕНИЕ РАБОТЫ", "error");

    cout << "\n!!! АВАРИЙНОЕ ЗАВЕРШЕНИЕ РАБОТЫ КИНОТЕАТРА !!!" << endl;

    // Остановка всех залов
    auto halls = cinema->getAllHalls();
    for (auto* hall : halls) {
        cout << "  • Аварийная остановка зала " << hall->getNumber() << endl;
        hall->getController()->emergencyStop();
    }

    cout << "  • Открытие аварийных выходов" << endl;
    cout << "  • Активация сигнализации" << endl;
    cout << "  • Уведомление служб" << endl;

    notifyStaff("Аварийное завершение! Эвакуация!");
    autoSaveState();

    log("Аварийное завершение выполнено");
}

void CinemaFacade::addScenario(const ScenarioConfig& scenario) {
    scenarios[scenario.name] = scenario;
    log("Добавлен сценарий: " + scenario.name);
}

// ========== СТАТИСТИКА И ОТЧЕТЫ ==========

void CinemaFacade::printStatistics() const {
    cout << "\n=== СТАТИСТИКА КИНОТЕАТРА ===" << endl;

    auto halls = cinema->getAllHalls();
    int totalSessions = getTodaySessionsCount();
    int bookedSessions = 0;

    auto iterator = cinema->getAllSessionsIterator();
    while (iterator.hasNext()) {
        if (!iterator.next()->getAvailability()) {
            bookedSessions++;
        }
    }

    cout << "Название: " << cinema->getName() << endl;
    cout << "Всего залов: " << halls.size() << endl;
    cout << "Общая вместимость: " << cinema->getTotalCapacity() << " мест" << endl;
    cout << "Сеансов сегодня: " << totalSessions << endl;
    cout << "Продано билетов: " << bookedSessions << endl;
    cout << "Заполняемость: " << (totalSessions > 0 ? (bookedSessions * 100 / totalSessions) : 0) << "%" << endl;

    // Информация по залам
    cout << "\nСтатистика по залам:" << endl;
    for (auto* hall : halls) {
        int hallSessions = hall->getSchedule().size();
        cout << "  Зал " << hall->getNumber() << ": " << hallSessions << " сеансов, "
            << "вместимость " << hall->getCapacity() << " мест" << endl;
    }
}

string CinemaFacade::generateDailyReport(const string& date) const {
    stringstream ss;

    ss << "\n=== ОТЧЕТ ЗА " << date << " ===" << endl;
    ss << "Кинотеатр: " << cinema->getName() << endl;
    ss << "Адрес: " << cinema->getAddress() << endl;
    ss << endl;

    auto iterator = cinema->getAllSessionsIterator();
    iterator.setSortType(SortType::BY_TIME);

    int totalTickets = 0;
    double totalRevenue = 0;
    map<string, int> movieStats;

    ss << "Проведенные сеансы:" << endl;

    while (iterator.hasNext()) {
        auto session = iterator.next();
        string title = session->getMovieTitle();
        movieStats[title]++;

        if (!session->getAvailability()) {
            totalTickets++;
            totalRevenue += 300; // Заглушка
        }

        char buffer[80];
        time_t time = session->getStartTime();
        struct tm timeinfo;
        localtime_s(&timeinfo, &time);
        strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);

        ss << "  " << buffer << " | Зал " << session->getHallNumber()
            << " | " << title
            << " | " << (session->getAvailability() ? "Свободно" : "Продано") << endl;
    }

    ss << "\nИтоги дня:" << endl;
    ss << "  Всего сеансов: " << getTodaySessionsCount() << endl;
    ss << "  Продано билетов: " << totalTickets << endl;
    ss << "  Выручка: " << totalRevenue << " руб." << endl;

    ss << "\nПопулярные фильмы:" << endl;
    for (auto it = movieStats.begin(); it != movieStats.end(); ++it) {
        ss << "  • " << it->first << ": " << it->second << " сеансов" << endl;
    }

    return ss.str();
}

double CinemaFacade::getTotalRevenue() const {
    double revenue = 0;
    auto iterator = cinema->getAllSessionsIterator();

    while (iterator.hasNext()) {
        auto session = iterator.next();
        if (!session->getAvailability()) {
            revenue += 300; // Заглушка
        }
    }

    return revenue;
}

string CinemaFacade::getCinemaInfo() const {
    stringstream ss;

    ss << "\n=== " << cinema->getName() << " ===" << endl;
    ss << "Адрес: " << cinema->getAddress() << endl;
    ss << "Часы работы: " << cinema->getWorkingHours() << endl;

    auto halls = cinema->getAllHalls();
    ss << "Количество залов: " << halls.size() << endl;
    ss << "Общая вместимость: " << cinema->getTotalCapacity() << " мест" << endl;

    int imaxCount = 0, vipCount = 0, standardCount = 0;
    for (auto* hall : halls) {
        if (dynamic_cast<ImaxHall*>(hall)) imaxCount++;
        else if (dynamic_cast<VipHall*>(hall)) vipCount++;
        else standardCount++;
    }

    if (imaxCount > 0) ss << "IMAX залов: " << imaxCount << endl;
    if (vipCount > 0) ss << "VIP залов: " << vipCount << endl;
    if (standardCount > 0) ss << "Стандартных залов: " << standardCount << endl;

    return ss.str();
}