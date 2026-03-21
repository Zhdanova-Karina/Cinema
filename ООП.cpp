#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <ctime>
#include "Cinema.h"
#include "Projector.h"
#include "SoundSystem.h"
#include "ScheduledProjector.h"
#include "LightSystem.h"
#include "AirConditioner.h"
#include "DoorLock.h"
#include "HallGroupFactory.h"
#include "ScheduleIterator.h"
#include "RegularSession.h"
#include "PremiereSession.h"
#include "LateNightSession.h"
#include "TwoDScreening.h"
#include "ThreeDScreening.h"
#include "ImaxScreening.h"
#include "MovieMetadata.h"
#include "MovieMetadataFactory.h"
#include "LightweightMovie.h"
#include "TicketExpert.h"
#include "CinemaFacade.h"
#include "CoolingProjector.h"
#include "ExternalTicketAPI.h"
#include "IBookingSystem.h"
#include "BookingAdapter.h"


using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "=== Система управления кинотеатром ===\n" << endl;

    // Создаем кинотеатр
    Cinema cinema("Космос", "ул. Ленина, 1", "10:00-23:00");
    cout << "Создан кинотеатр: " << cinema.getName() << endl;
    cout << "Адрес: " << cinema.getAddress() << endl;
    cout << "Часы работы: " << cinema.getWorkingHours() << "\n" << endl;

    // Создаем IMAX зал
    auto imaxHall = make_unique<ImaxHall>(1, 250);
    auto* controller1 = imaxHall->getController();

    // Добавляем оборудование в IMAX зал
    controller1->registerEquipment(make_shared<ImaxProjector>("proj_imax_101"));
    controller1->registerEquipment(make_shared<SurroundSound>("sound_101"));
    controller1->registerEquipment(make_shared<LightSystem>("light_101"));
    controller1->registerEquipment(make_shared<AirConditioner>("ac_101"));
    controller1->registerEquipment(make_shared<DoorLock>("door_101"));

    cinema.addHall(move(imaxHall));

    // Создаем VIP зал
    auto vipHall = make_unique<VipHall>(2, 50);
    auto* controller2 = vipHall->getController();

    controller2->registerEquipment(make_shared<StandardProjector>("proj_201"));
    controller2->registerEquipment(make_shared<SoundSystem>("sound_201"));
    controller2->registerEquipment(make_shared<LightSystem>("light_201"));
    controller2->registerEquipment(make_shared<AirConditioner>("ac_201"));
    controller2->registerEquipment(make_shared<DoorLock>("door_201"));

    cinema.addHall(move(vipHall));

    cout << "\n=== Демонстрация работы ===" << endl;

    // Получаем залы
    auto halls = cinema.getAllHalls();
    Hall* hall1 = cinema.getHall(1);
    Hall* hall2 = cinema.getHall(2);

    // Демонстрация для IMAX зала
    cout << "\n--- Работа с IMAX залом (Зал #1) ---" << endl;

    Session session1("Аватар 2", "18:30", 180);
    hall1->addSession(session1);
    hall1->prepareForSession(session1);

    cout << "\nСтатус оборудования в зале 1:" << endl;
    auto statuses1 = hall1->getController()->getStatuses();
    for (const auto& status : statuses1) {
        cout << "  " << status << endl;
    }
    // ========== ТЕСТИРОВАНИЕ АДАПТЕРА СИСТЕМЫ БРОНИРОВАНИЯ ==========
    cout << "\n=== ТЕСТИРОВАНИЕ АДАПТЕРА СИСТЕМЫ БРОНИРОВАНИЯ ===\n" << endl;

    // Создаем внешний API (несовместимый интерфейс)
    ExternalTicketAPI* externalAPI = new ExternalTicketAPI();

    // Адаптируем его к нашему интерфейсу
    IBookingSystem* bookingSystem = new BookingAdapter(externalAPI);

    // Получаем доступные места для сеанса
    string sessionId = "SESSION_001";
    vector<int> availableSeats = bookingSystem->getAvailableSeats(sessionId);

    cout << "Доступные места: ";
    for (int seat : availableSeats) {
        cout << seat << " ";
    }
    cout << endl;

    // Бронируем билет
    if (bookingSystem->bookTicket(sessionId, 5)) {
        cout << "Билет успешно забронирован!" << endl;
    }

    // Отменяем бронирование
    if (bookingSystem->cancelBooking("RES_1234")) {
        cout << "Бронирование отменено!" << endl;
    }

    // Очистка памяти
    delete bookingSystem;
    delete externalAPI;

    // ========== ТЕСТИРОВАНИЕ ДЕКОРАТОРА-ПЛАНИРОВЩИКА ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ДЕКОРАТОРА-ПЛАНИРОВЩИКА ===\n" << endl;

    // Создаем обычный проектор (обычный указатель)
    Projector* basicProjector1 = new StandardProjector("STD-001");

    // Оборачиваем его в декоратор с планировщиком (объявляем как Projector*)
    Projector* scheduledProjector = new ScheduledProjector(basicProjector1);

    // Запускаем планировщик (с приведением типа)
    static_cast<ScheduledProjector*>(scheduledProjector)->startScheduler();

    // Получаем текущее время
    time_t now = time(nullptr);

    // Планируем задачи с интервалами
    cout << "\n--- Планирование задач ---" << endl;
    static_cast<ScheduledProjector*>(scheduledProjector)->scheduleTurnOn(now + 2);
    static_cast<ScheduledProjector*>(scheduledProjector)->schedulePlay(now + 4, "Дюна.mp4");
    static_cast<ScheduledProjector*>(scheduledProjector)->scheduleTurnOff(now + 6);

    // Показываем расписание
    cout << "\n--- Текущее расписание ---" << endl;
    static_cast<ScheduledProjector*>(scheduledProjector)->showSchedule();

    // Имитация работы в реальном времени
    cout << "\n--- Ожидание выполнения задач (10 секунд) ---" << endl;
    for (int i = 1; i <= 10; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "\nСекунда " << i << ":" << endl;

        // Показываем статус на определенных секундах
        if (i == 3 || i == 5 || i == 7) {
            cout << static_cast<ScheduledProjector*>(scheduledProjector)->getStatus() << endl;
        }
    }

    // Останавливаем планировщик
    static_cast<ScheduledProjector*>(scheduledProjector)->stopScheduler();

    // Демонстрация ручного управления
    cout << "\n--- Ручное управление ---" << endl;
    scheduledProjector->turnOn();      // Эти методы доступны через Projector*
    scheduledProjector->play("Аватар 2.mp4");
    scheduledProjector->turnOff();

    // Удаляем объекты
    delete scheduledProjector;
    delete basicProjector1;

    // ========== ТЕСТИРОВАНИЕ ДЕКОРАТОРА ОСТЫВАНИЯ ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ДЕКОРАТОРА ОСТЫВАНИЯ ===\n" << endl;

    // Создаем обычный проектор
    Projector* basicProjector2 = new StandardProjector("STD-002");

    // Оборачиваем его в декоратор остывания
    Projector* coolingProjector = new CoolingProjector(basicProjector2, 10); // 10 секунд остывания

    // Тестируем работу
    cout << "\n--- Включение и воспроизведение ---" << endl;
    coolingProjector->turnOn();
    coolingProjector->play("Фильм.mp4");

    cout << "\n--- Выключение (запуск остывания) ---" << endl;
    coolingProjector->turnOff();

    cout << "\n--- Попытка включить во время остывания ---" << endl;
    this_thread::sleep_for(chrono::seconds(3));
    coolingProjector->turnOn();  // Должен подождать остывания

    cout << "\n--- Статус проектора ---" << endl;
    cout << coolingProjector->getStatus() << endl;

    // Удаляем объекты
    delete coolingProjector;
    delete basicProjector2;

    // ========== ТЕСТИРОВАНИЕ ВЛОЖЕННЫХ ДЕКОРАТОРОВ ==========
// Планировщик + Остывание
    cout << "\n=== ТЕСТИРОВАНИЕ ВЛОЖЕННЫХ ДЕКОРАТОРОВ ===\n";
    cout << "=== Планировщик + Остывание ===\n" << endl;

    // Создаем базовый проектор
    Projector* basicProjector = new StandardProjector("STD-003");

    // Сначала оборачиваем в декоратор остывания
    Projector* coolingProjector1 = new CoolingProjector(basicProjector, 15); // 15 секунд остывания

    // Затем оборачиваем в декоратор планировщика
    Projector* scheduledProjector1 = new ScheduledProjector(coolingProjector1);

    // Приводим к типу ScheduledProjector* для доступа к методам планировщика
    ScheduledProjector* combo = static_cast<ScheduledProjector*>(scheduledProjector1);

    // Запускаем планировщик
    combo->startScheduler();

    // Получаем текущее время
    time_t nowTime = time(nullptr);

    // Планируем задачи с интервалами
    cout << "\n--- Планирование задач ---" << endl;
    combo->scheduleTurnOn(now + 2);      // Через 2 секунды включить
    combo->schedulePlay(now + 5, "Интерстеллар.mp4");   // Через 5 секунд воспроизвести
    combo->scheduleTurnOff(now + 10);     // Через 10 секунд выключить (запустит остывание)

    // Показываем расписание
    cout << "\n--- Текущее расписание ---" << endl;
    combo->showSchedule();

    // Имитация работы в реальном времени
    cout << "\n--- Ожидание выполнения задач (20 секунд) ---" << endl;
    for (int i = 1; i <= 20; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "\nСекунда " << i << ":" << endl;

        // Показываем статус на определенных секундах
        if (i == 3 || i == 6 || i == 8 || i == 12 || i == 15 || i == 18) {
            cout << combo->getStatus() << endl;
        }

        // Показываем состояние остывания каждые 5 секунд
        if (i % 5 == 0) {
            // Получаем доступ к декоратору остывания через приведение типов
            // Так как scheduledProjector оборачивает coolingProjector
            CoolingProjector* cooler = static_cast<CoolingProjector*>(
                static_cast<ScheduledProjector*>(scheduledProjector1)->getWrappedProjector()
                );
            if (cooler) {
                cout << "  [Статус остывания] "
                    << (cooler->isCoolingDown() ? "Проектор остывает" : "Проектор остыл") << endl;
            }
        }
    }

    // Останавливаем планировщик
    combo->stopScheduler();

    // Удаляем объекты в правильном порядке (обратном созданию)
    delete scheduledProjector1;  // Удаляем декоратор планировщика
    delete coolingProjector1;     // Удаляем декоратор остывания
    delete basicProjector;       // Удаляем базовый проектор

    // ========== ТЕСТИРОВАНИЕ КОМПОНОВЩИКА ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ПАТТЕРНА КОМПОНОВЩИК ===\n" << endl;

    // 1. Создаем структуру залов с помощью компоновщика
    cout << "1. Создание иерархической структуры залов:" << endl;

    // Создаем восточное крыло (группа залов)
    auto eastWing = make_shared<HallGroup>("Восточное крыло", "Залы 10-13");

    // Добавляем залы в восточное крыло
    eastWing->add(make_shared<HallLeaf>(
        make_unique<StandardHall>(10, 120), "Стандартный зал 10"));
    eastWing->add(make_shared<HallLeaf>(
        make_unique<StandardHall>(11, 150), "Стандартный зал 11"));
    eastWing->add(make_shared<HallLeaf>(
        make_unique<VipHall>(12, 40), "VIP зал 12"));

    cout << "\n   Восточное крыло создано" << endl;

    // Создаем западное крыло с вложенной группой
    auto westWing = make_shared<HallGroup>("Западное крыло", "Залы 14-17");

    // Добавляем залы
    westWing->add(make_shared<HallLeaf>(
        make_unique<StandardHall>(14, 200), "Стандартный зал 14"));
    westWing->add(make_shared<HallLeaf>(
        make_unique<StandardHall>(15, 180), "Стандартный зал 15"));

    // Создаем мини-группу внутри западного крыла
    auto imaxCluster = make_shared<HallGroup>("IMAX кластер", "Залы с IMAX технологией");
    imaxCluster->add(make_shared<HallLeaf>(
        make_unique<ImaxHall>(16, 300), "IMAX зал 16"));
    imaxCluster->add(make_shared<HallLeaf>(
        make_unique<ImaxHall>(17, 280), "IMAX зал 17"));

    // Добавляем вложенную группу в западное крыло
    westWing->add(imaxCluster);

    cout << "   Западное крыло создано (с вложенной группой IMAX)" << endl;

    // 2. Добавляем созданные группы в кинотеатр
    cout << "\n2. Добавление групп в кинотеатр:" << endl;
    cinema.addComponent(eastWing);
    cinema.addComponent(westWing);

    // 3. Демонстрация информации о структуре
    cout << "\n3. Информация о структуре кинотеатра:" << endl;

    cout << "\n   Восточное крыло:" << endl;
    cout << eastWing->getInfo() << endl;

    cout << "\n   Западное крыло:" << endl;
    cout << westWing->getInfo() << endl;

    // 4. Поиск зала по номеру через компонент
    cout << "\n4. Поиск зала №16:" << endl;
    Hall* foundHall = cinema.getHall(16);
    if (foundHall) {
        cout << "   Найден: зал " << foundHall->getNumber()
            << " (вместимость: " << foundHall->getCapacity() << ")" << endl;
    }

    // 5. Демонстрация групповых операций
    cout << "\n5. Групповые операции с залами:" << endl;

    Session session3("Оппенгеймер", "21:00", 180);

    // Подготавливаем все залы в восточном крыле
    cout << "\n   Подготовка всех залов восточного крыла:" << endl;
    eastWing->prepareForSession(session3);

    // Уборка во всех залах западного крыла
    cout << "\n   Уборка всех залов западного крыла:" << endl;
    westWing->cleanupAfterSession();

    // 6. Поиск компонента по имени
    cout << "\n6. Поиск компонента 'IMAX кластер':" << endl;
    auto foundComponent = cinema.findComponent("IMAX кластер");
    if (foundComponent) {
        cout << "   Найден компонент:" << endl;
        cout << foundComponent->getInfo() << endl;

        // Работа с найденным компонентом
        cout << "   Подготовка всех залов в IMAX кластере:" << endl;
        foundComponent->prepareForSession(session3);
    }

    // 7. Демонстрация общей вместимости
    cout << "\n7. Статистика кинотеатра:" << endl;
    cout << "   Всего залов (старых и новых): " << cinema.getAllHalls().size() << endl;
    cout << "   Общая вместимость: " << cinema.getTotalCapacity() << " мест" << endl;

    // 8. Демонстрация единообразной работы с группами
    cout << "\n8. Единообразная работа с разными уровнями:" << endl;

    // Работа с отдельным залом
    cout << "\n   Работа с отдельным залом (Зал 1):" << endl;
    hall1->prepareForSession(session1);

    // Работа с группой залов
    cout << "\n   Работа с группой (Восточное крыло):" << endl;
    eastWing->prepareForSession(session1);

    // Работа со всем кинотеатром
    cout << "\n   Работа со всем кинотеатром:" << endl;
    cinema.prepareAllHalls(session1);

    // ========== ТЕСТИРОВАНИЕ ИТЕРАТОРА ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ПАТТЕРНА ИТЕРАТОР ===\n" << endl;

    // Добавляем тестовые сеансы для демонстрации
    cout << "1. Добавление тестовых сеансов:" << endl;

    Hall* hall16 = cinema.getHall(16);

    // Сеансы для зала 1
    Session session101("Аватар 2", "10:00", 180, "Фантастика", 12);
    Session session102("Дюна 2", "13:30", 155, "Фантастика", 12);
    Session session103("Оппенгеймер", "17:00", 180, "Драма", 18);
    Session session104("Барби", "20:30", 114, "Комедия", 6);

    if (hall1) {
        hall1->addSession(session101);
        hall1->addSession(session102);
        hall1->addSession(session103);
        hall1->addSession(session104);
        cout << "  Добавлены сеансы в зал 1" << endl;
    }

    // Сеансы для зала 2
    Session session201("Дюна 2", "11:00", 155, "Фантастика", 12);
    Session session202("Оппенгеймер", "14:30", 180, "Драма", 18);
    Session session203("Аватар 2", "18:00", 180, "Фантастика", 12);
    Session session204("Матрица", "21:30", 150, "Фантастика", 16);

    if (hall2) {
        hall2->addSession(session201);
        hall2->addSession(session202);
        hall2->addSession(session203);
        hall2->addSession(session204);
        cout << "  Добавлены сеансы в зал 2" << endl;
    }

    // Сеансы для зала 16
    if (hall16) {
        Session session1601("Интерстеллар", "12:00", 169, "Фантастика", 12);
        Session session1602("Начало", "15:30", 148, "Триллер", 12);
        Session session1603("Тёмный рыцарь", "19:00", 152, "Боевик", 16);

        hall16->addSession(session1601);
        hall16->addSession(session1602);
        hall16->addSession(session1603);
        cout << "  Добавлены сеансы в зал 16 (IMAX)" << endl;
    }

    // ПОЛУЧАЕМ ИТЕРАТОР ПО ВСЕМ СЕАНСАМ
    cout << "\n2. Получение итератора по всем сеансам:" << endl;
    auto allSessions = cinema.getAllSessionsIterator();
    cout << "   Всего сеансов: " << allSessions.size() << endl;

    // 1. Обход всех сеансов в прямом порядке
    cout << "\n3. Обход всех сеансов (по времени):" << endl;
    allSessions.reset();
    int count = 1;
    while (allSessions.hasNext()) {
        auto session = allSessions.next();
        cout << "   " << count++ << ". ";
        session->print();
    }

    // 2. Сортировка по залам
    cout << "\n4. Сортировка по номерам залов:" << endl;
    allSessions.setSortType(SortType::BY_HALL);
    allSessions.reset();
    count = 1;
    while (allSessions.hasNext()) {
        auto session = allSessions.next();
        char buffer[80];
        time_t time = session->getStartTime();
        struct tm timeinfo;
        localtime_s(&timeinfo, &time);
        strftime(buffer, sizeof(buffer), "%H:%M", &timeinfo);
        cout << "   " << count++ << ". Зал " << session->getHallNumber()
            << ": " << session->getMovieTitle()
            << " в " << buffer << endl;
    }

    // 3. Сортировка по названиям фильмов
    cout << "\n5. Сортировка по названиям фильмов:" << endl;
    allSessions.setSortType(SortType::BY_MOVIE);
    allSessions.reset();
    count = 1;
    while (allSessions.hasNext()) {
        auto session = allSessions.next();
        cout << "   " << count++ << ". " << session->getMovieTitle()
            << " (зал " << session->getHallNumber() << ")" << endl;
    }

    // 4. Обратный порядок
    cout << "\n6. Обратный порядок (по времени):" << endl;
    allSessions.setSortType(SortType::BY_TIME);
    allSessions.setDirection(Direction::REVERSE);
    allSessions.reset();
    count = 1;
    while (allSessions.hasNext()) {
        auto session = allSessions.next();
        cout << "   " << count++ << ". ";
        session->print();
    }

    // 5. Фильтр по фильму
    cout << "\n7. Фильтр: только сеансы фильма 'Аватар 2':" << endl;
    auto avatarSessions = cinema.getAllSessionsIterator();
    avatarSessions.setMovieFilter("Аватар 2");
    if (avatarSessions.isEmpty()) {
        cout << "   Сеансы не найдены" << endl;
    }
    else {
        avatarSessions.setSortType(SortType::BY_TIME);
        avatarSessions.reset();
        count = 1;
        while (avatarSessions.hasNext()) {
            auto session = avatarSessions.next();
            cout << "   " << count++ << ". Зал " << session->getHallNumber()
                << " - " << session->getMovieTitle() << endl;
        }
    }

    // 6. Фильтр по залу
    cout << "\n8. Фильтр: только сеансы в зале 2:" << endl;
    auto hall2Sessions = cinema.getAllSessionsIterator();
    hall2Sessions.setHallFilter(2);
    hall2Sessions.setSortType(SortType::BY_TIME);
    hall2Sessions.reset();
    count = 1;
    while (hall2Sessions.hasNext()) {
        auto session = hall2Sessions.next();
        cout << "   " << count++ << ". " << session->getMovieTitle() << endl;
    }

    // 7. Помечаем один сеанс как занятый
    cout << "\n9. Бронирование одного сеанса:" << endl;
    auto sessions = cinema.getAllSessionsIterator();
    if (sessions.hasNext()) {
        auto firstSession = sessions.next();
        firstSession->book();
        cout << "   Забронирован: " << firstSession->getMovieTitle()
            << " в зале " << firstSession->getHallNumber() << endl;
    }

    // 8. Фильтр только доступные
    cout << "\n10. Фильтр: только доступные сеансы:" << endl;
    auto availableSessions = cinema.getAllSessionsIterator();
    availableSessions.setAvailableOnly(true);
    availableSessions.setSortType(SortType::BY_TIME);
    availableSessions.reset();
    count = 1;
    while (availableSessions.hasNext()) {
        auto session = availableSessions.next();
        cout << "   " << count++ << ". " << session->getMovieTitle()
            << " (зал " << session->getHallNumber() << ")" << endl;
    }

    // 9. Сортировка по доступности
    cout << "\n11. Сортировка по доступности (сначала свободные):" << endl;
    auto byAvailability = cinema.getAllSessionsIterator();
    byAvailability.setSortType(SortType::BY_AVAILABILITY);
    byAvailability.reset();
    count = 1;
    while (byAvailability.hasNext()) {
        auto session = byAvailability.next();
        cout << "   " << count++ << ". " << session->getMovieTitle()
            << " - " << (session->getAvailability() ? "СВОБОДНО" : "ЗАНЯТО") << endl;
    }

    // 10. Комбинированный фильтр
    cout << "\n12. Комбинированный фильтр: зал 1, доступные сеансы:" << endl;
    auto combinedFilter = cinema.getAllSessionsIterator();
    combinedFilter.setHallFilter(1);
    combinedFilter.setAvailableOnly(true);
    combinedFilter.setSortType(SortType::BY_TIME);
    combinedFilter.reset();
    count = 1;
    while (combinedFilter.hasNext()) {
        auto session = combinedFilter.next();
        cout << "   " << count++ << ". " << session->getMovieTitle() << endl;
    }

    // 11. Поиск первого сеанса по условию
    cout << "\n13. Поиск первого сеанса в зале 16:" << endl;
    auto searchIterator = cinema.getAllSessionsIterator();
    bool found = searchIterator.seekToFirst([](auto session) {
        return session->getHallNumber() == 16;
        });
    if (found) {
        auto session = searchIterator.next();
        cout << "   Найден: " << session->getMovieTitle()
            << " в зале " << session->getHallNumber() << endl;
    }
    else {
        cout << "   Сеанс не найден" << endl;
    }

    // 12. Итератор для конкретного зала
    cout << "\n14. Итератор только для зала 16:" << endl;
    auto hall16Iterator = cinema.getHallSessionsIterator(16);
    hall16Iterator.setSortType(SortType::BY_TIME);
    hall16Iterator.reset();
    count = 1;
    while (hall16Iterator.hasNext()) {
        auto session = hall16Iterator.next();
        cout << "   " << count++ << ". " << session->getMovieTitle() << endl;
    }

    // 13. Статистика
    cout << "\n15. Статистика по сеансам:" << endl;
    auto stats = cinema.getAllSessionsIterator();
    cout << "   Всего сеансов в системе: " << stats.size() << endl;
    stats.printSummary();

    // ========== ТЕСТИРОВАНИЕ ПАТТЕРНА BRIDGE ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ПАТТЕРНА BRIDGE ===\n" << endl;

    // Создаем тестовый фильм
    Movie testMovie("Дюна 2", 155, "Фантастика", 12, "dune2.mp4");
    int basePrice = 300;

    cout << "Демонстрация различных комбинаций типов сеансов и форматов показа:\n" << endl;

    // Создаем различные комбинации
    vector<unique_ptr<SessionType>> bridgeSessions;

    // Обычные сеансы в разных форматах
    bridgeSessions.push_back(make_unique<RegularSession>(make_unique<TwoDScreening>()));
    bridgeSessions.push_back(make_unique<RegularSession>(make_unique<ThreeDScreening>()));
    bridgeSessions.push_back(make_unique<RegularSession>(make_unique<ImaxScreening>()));

    // Премьеры в разных форматах
    bridgeSessions.push_back(make_unique<PremiereSession>(make_unique<ThreeDScreening>()));
    bridgeSessions.push_back(make_unique<PremiereSession>(make_unique<ImaxScreening>()));

    // Ночные сеансы
    bridgeSessions.push_back(make_unique<LateNightSession>(make_unique<TwoDScreening>()));
    bridgeSessions.push_back(make_unique<LateNightSession>(make_unique<ThreeDScreening>()));

    // Демонстрация всех комбинаций
    for (size_t i = 0; i < bridgeSessions.size(); i++) {
        cout << "----------------------------------------" << endl;
        cout << "Сеанс " << i + 1 << ": " << bridgeSessions[i]->getDescription() << endl;

        // Проведение сеанса
        bridgeSessions[i]->startScreening(testMovie);

        // Расчет цены
        int price = bridgeSessions[i]->calculatePrice(basePrice);
        cout << "Цена билета: " << price << " руб." << endl;
    }

    // ========== ТЕСТИРОВАНИЕ ПАТТЕРНА FLYWEIGHT ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ПАТТЕРНА FLYWEIGHT ===\n" << endl;

    // Настройка кэша
    CacheSettings cacheSettings;
    cacheSettings.maxSize = 5;
    cacheSettings.evictionPolicy = "LRU";

    // Создание фабрики
    MovieMetadataFactory factory(cacheSettings);

    // Создание метаданных для фильмов
    cout << "1. Создание метаданных фильмов:" << endl;

    vector<string> actors1 = { "Сэм Уортингтон", "Зои Салдана", "Сигурни Уивер" };
    auto meta1 = factory.getMetadata(
        "Аватар 2", 192, "Фантастика", 12, "avatar2.jpg",
        "Возвращение на Пандору", "Джеймс Кэмерон", actors1,
        "США", 2022, "20th Century Studios"
    );

    vector<string> actors2 = { "Тимоти Шаламе", "Ребекка Фергюсон", "Оскар Айзек" };
    auto meta2 = factory.getMetadata(
        "Дюна 2", 155, "Фантастика", 12, "dune2.jpg",
        "Продолжение эпической саги", "Дени Вильнёв", actors2,
        "США", 2024, "Warner Bros"
    );

    vector<string> actors3 = { "Киллиан Мёрфи", "Эмили Блант", "Мэтт Деймон" };
    auto meta3 = factory.getMetadata(
        "Оппенгеймер", 180, "Драма", 18, "oppenheimer.jpg",
        "История создателя атомной бомбы", "Кристофер Нолан", actors3,
        "США", 2023, "Universal Pictures"
    );

    // Попытка создать дубликат (должен вернуть существующий объект)
    cout << "\n2. Попытка создания дубликата фильма:" << endl;
    auto meta1_duplicate = factory.getMetadata(
        "Аватар 2", 192, "Фантастика", 12, "avatar2.jpg",
        "Возвращение на Пандору", "Джеймс Кэмерон", actors1,
        "США", 2022, "20th Century Studios"
    );

    cout << "   meta1 адрес: " << meta1.get() << endl;
    cout << "   meta1_duplicate адрес: " << meta1_duplicate.get() << endl;
    cout << "   Это " << (meta1 == meta1_duplicate ? "один" : "разные") << " объект" << endl;

    // Создание легковесных фильмов (сеансов)
    cout << "\n3. Создание легковесных фильмов (сеансов):" << endl;

    time_t currentTime = time(nullptr);
    vector<LightweightMovie> flyweightSessions;

    // Создаем несколько сеансов для каждого фильма
    for (int i = 0; i < 5; i++) {
        LightweightMovie movie1(meta1, currentTime + i * 86400, true, 1);
        movie1.setSessionId(100 + i);
        movie1.sellTicket(50 + i * 10);
        flyweightSessions.push_back(movie1);
    }

    for (int i = 0; i < 3; i++) {
        LightweightMovie movie2(meta2, currentTime + i * 86400, true, 2);
        movie2.setSessionId(200 + i);
        movie2.sellTicket(30 + i * 5);
        flyweightSessions.push_back(movie2);
    }

    for (int i = 0; i < 2; i++) {
        LightweightMovie movie3(meta3, currentTime + i * 86400, true, 3);
        movie3.setSessionId(300 + i);
        movie3.sellTicket(20 + i * 8);
        flyweightSessions.push_back(movie3);
    }

    cout << "   Создано " << flyweightSessions.size() << " легковесных сеансов" << endl;
    cout << "   При этом метаданных создано: " << factory.getCacheSize() << endl;

    // Демонстрация экономии памяти
    cout << "\n4. Демонстрация экономии памяти:" << endl;
    cout << "   Размер метаданных (1 объект): ~" << sizeof(MovieMetadata) << " байт" << endl;
    cout << "   Размер легковесного фильма: ~" << sizeof(LightweightMovie) << " байт" << endl;
    cout << "   Всего сеансов: " << flyweightSessions.size() << endl;

    double withoutFlyweight = (flyweightSessions.size() * sizeof(MovieMetadata)) / 1024.0;
    double withFlyweight = (factory.getCacheSize() * sizeof(MovieMetadata) +
        flyweightSessions.size() * sizeof(LightweightMovie)) / 1024.0;
    double savings = (1 - withFlyweight / withoutFlyweight) * 100;

    cout << "   Без Flyweight: " << withoutFlyweight << " КБ" << endl;
    cout << "   С Flyweight: " << withFlyweight << " КБ" << endl;
    cout << "   Экономия: ~" << savings << "%" << endl;

    // Вывод информации о сеансах
    cout << "\n5. Информация о сеансах:" << endl;
    for (const auto& session : flyweightSessions) {
        session.displaySessionInfo();
        cout << endl;
    }

    // Статистика кэша
    factory.printCacheStats();

    // Тест LRU (добавление новых фильмов для вытеснения)
    cout << "\n6. Тест LRU (добавление новых фильмов для вытеснения):" << endl;

    vector<string> actors4 = { "Леонардо ДиКаприо", "Джозеф Гордон-Левитт" };
    factory.getMetadata("Начало", 148, "Триллер", 16, "inception.jpg",
        "Путешествие по снам", "Кристофер Нолан", actors4,
        "США", 2010, "Warner Bros");

    vector<string> actors5 = { "Мэттью МакКонахи", "Энн Хэтэуэй" };
    factory.getMetadata("Интерстеллар", 169, "Фантастика", 12, "interstellar.jpg",
        "Путешествие через червоточину", "Кристофер Нолан", actors5,
        "США", 2014, "Warner Bros");

    vector<string> actors6 = { "Хоакин Феникс", "Леди Гага" };
    factory.getMetadata("Джокер 2", 138, "Драма", 18, "joker2.jpg",
        "Безумие продолжается", "Тодд Филлипс", actors6,
        "США", 2024, "Warner Bros");

    factory.printCacheStats();

    // ========== ТЕСТИРОВАНИЕ ПАТТЕРНА INFORMATION EXPERT ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ПАТТЕРНА INFORMATION EXPERT ===\n" << endl;

    // Создаем эксперта по билетам
    TicketExpert ticketExpert(300);

    // Настраиваем дополнительные скидки
    ticketExpert.setCategoryDiscount("student", 0.15);
    ticketExpert.setCategoryDiscount("child", 0.30);
    ticketExpert.setCategoryDiscount("pensioner", 0.20);

    // Добавляем промокод
    PromoCode weekendPromo;
    weekendPromo.code = "WEEKEND20";
    weekendPromo.discount = 0.20;
    weekendPromo.validDays = { "saturday", "sunday" };
    weekendPromo.minTickets = 2;
    weekendPromo.maxUses = 100;
    weekendPromo.currentUses = 0;
    weekendPromo.validUntil = time(nullptr) + 30 * 24 * 3600; // 30 дней

    ticketExpert.addPromoCode(weekendPromo);

    // Создаем тестовые сеансы для эксперта
    time_t expertNow = time(nullptr);

    // Сеанс 1: вечерний, 3D
    Session expertSession1("Аватар 2", "19:30", 180, "Фантастика", 12);

    // Сеанс 2: утренний, 2D
    Session expertSession2("Дюна 2", "10:00", 155, "Фантастика", 12);

    // Тест 1: Расчет цены для разных категорий зрителей
    cout << "\n1. Расчет цены для разных категорий зрителей (сеанс 19:30, 3D):" << endl;

    ViewerInfo viewerAdult = { 30, false, false, false, "" };
    ViewerInfo viewerStudent = { 20, true, false, false, "" };
    ViewerInfo viewerChild = { 10, false, false, false, "" };
    ViewerInfo viewerPensioner = { 70, false, true, false, "" };
    ViewerInfo viewerVip = { 35, false, false, true, "" };

    cout << "   Взрослый: " << ticketExpert.calculatePrice(expertSession1, viewerAdult) << " руб." << endl;
    cout << "   Студент: " << ticketExpert.calculatePrice(expertSession1, viewerStudent) << " руб." << endl;
    cout << "   Ребенок (10 лет): " << ticketExpert.calculatePrice(expertSession1, viewerChild) << " руб." << endl;
    cout << "   Пенсионер: " << ticketExpert.calculatePrice(expertSession1, viewerPensioner) << " руб." << endl;
    cout << "   VIP: " << ticketExpert.calculatePrice(expertSession1, viewerVip) << " руб." << endl;

    // Тест 2: Расчет для утреннего сеанса (со скидкой)
    cout << "\n2. Расчет цены для утреннего сеанса (10:00, 2D):" << endl;
    cout << "   Взрослый утром: " << ticketExpert.calculatePrice(expertSession2, viewerAdult) << " руб." << endl;

    // Тест 3: Проверка возрастных ограничений
    cout << "\n3. Проверка возрастных ограничений:" << endl;
    cout << "   Фильм 'Аватар 2' (12+):" << endl;
    cout << "     10 лет: " << (ticketExpert.canWatch(expertSession1, 10) ? "Можно" : "Нельзя") << endl;
    cout << "     14 лет: " << (ticketExpert.canWatch(expertSession1, 14) ? "Можно" : "Нельзя") << endl;

    // Тест 4: Применение промокода
    cout << "\n4. Применение промокода WEEKEND20:" << endl;
    ViewerInfo viewerWithPromo = { 30, false, false, false, "WEEKEND20" };

    int priceWithoutPromo = ticketExpert.calculatePrice(expertSession1, viewerAdult, 2);
    int priceWithPromo = ticketExpert.calculatePrice(expertSession1, viewerWithPromo, 2);

    cout << "   2 билета без промокода: " << priceWithoutPromo << " руб." << endl;
    cout << "   2 билета с промокодом: " << priceWithPromo << " руб." << endl;
    cout << "   Экономия: " << priceWithoutPromo - priceWithPromo << " руб." << endl;

    // Тест 5: Детальный расчет цены
    cout << "\n5. Детальный расчет цены:" << endl;
    cout << ticketExpert.getPriceBreakdown(expertSession1, viewerStudent);

    // Тест 6: Рекомендации фильмов
    cout << "\n6. Рекомендации фильмов:" << endl;
    vector<Movie> movies = {
        Movie("Аватар 2", 192, "Фантастика", 12, ""),
        Movie("Дюна 2", 155, "Фантастика", 12, ""),
        Movie("Оппенгеймер", 180, "Драма", 18, ""),
        Movie("Барби", 114, "Комедия", 6, "")
    };

    auto recommendations = ticketExpert.getRecommendedMovies(movies, 14, "Фантастика");
    cout << "   Для 14-летнего любителя фантастики:" << endl;
    for (const auto& title : recommendations) {
        cout << "     • " << title << endl;
    }

    // ========== ТЕСТИРОВАНИЕ ПАТТЕРНА FACADE ==========
    cout << "\n" << endl;
    cout << "======================================================================" << endl;
    cout << "=== ТЕСТИРОВАНИЕ ПАТТЕРНА FACADE ===" << endl;
    cout << "======================================================================" << endl;

    // Создание фасада
    CinemaFacade facade("Космос", "ул. Ленина, 1", "10:00-23:00");

    cout << "\n1. ИНФОРМАЦИЯ О КИНОТЕАТРЕ:" << endl;
    cout << "----------------------------------------" << endl;
    cout << facade.getCinemaInfo() << endl;

    // 1. Добавление фильмов через фасад
    cout << "\n2. ДОБАВЛЕНИЕ ФИЛЬМОВ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.addMovie("Аватар 2", 192, "Фантастика", 12, "avatar.jpg", "Продолжение истории о Пандоре");
    facade.addMovie("Дюна 2", 155, "Фантастика", 12, "dune.jpg", "Эпическая сага о песчаных червях");
    facade.addMovie("Оппенгеймер", 180, "Драма", 18, "oppenheimer.jpg", "История создателя атомной бомбы");
    facade.addMovie("Барби", 114, "Комедия", 6, "barbie.jpg", "Розовая комедия");

    // 2. Создание залов через фасад
    cout << "\n3. СОЗДАНИЕ ЗАЛОВ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.createImaxHall(1, 250);
    facade.createVipHall(2, 50);
    facade.createStandardHall(3, 120);
    facade.createStandardHall(4, 80);

    // 3. Создание сеансов через фасад
    cout << "\n4. СОЗДАНИЕ СЕАНСОВ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.createSession("Аватар 2", "10:00", 1, "Regular", "IMAX");
    facade.createSession("Аватар 2", "13:30", 1, "Regular", "IMAX");
    facade.createSession("Аватар 2", "17:00", 1, "Regular", "IMAX");
    facade.createSession("Аватар 2", "20:30", 1, "Premiere", "IMAX");

    facade.createSession("Дюна 2", "11:30", 3, "Regular", "2D");
    facade.createSession("Дюна 2", "15:00", 3, "Regular", "2D");
    facade.createSession("Дюна 2", "19:30", 3, "Premiere", "2D");

    facade.createSession("Оппенгеймер", "12:00", 2, "Regular", "2D");
    facade.createSession("Оппенгеймер", "16:30", 2, "Regular", "2D");
    facade.createSession("Оппенгеймер", "21:00", 2, "LateNight", "2D");

    facade.createSession("Барби", "14:00", 4, "KidsMorning", "2D");
    facade.createSession("Барби", "18:00", 4, "Regular", "2D");

    // 4. Получение расписания
    cout << "\n5. РАСПИСАНИЕ СЕАНСОВ:" << endl;
    cout << "----------------------------------------" << endl;
    auto schedule = facade.getSchedule("2026-03-20");
    for (size_t i = 0; i < schedule.size(); i++) {
        cout << schedule[i] << endl;
    }

    // 5. Информация о фильме
    cout << "\n6. ДЕТАЛИ ФИЛЬМА 'Аватар 2':" << endl;
    cout << "----------------------------------------" << endl;
    cout << facade.getMovieDetails("Аватар 2") << endl;

    // 6. Поиск сеансов
    cout << "\n7. ПОИСК СЕАНСОВ ФИЛЬМА 'Дюна 2':" << endl;
    cout << "----------------------------------------" << endl;
    auto foundSessions = facade.findSessions("Дюна 2");
    for (size_t i = 0; i < foundSessions.size(); i++) {
        cout << foundSessions[i] << endl;
    }
    // 8. Информация о зале
    cout << "\n8. ИНФОРМАЦИЯ О ЗАЛАХ:" << endl;
    cout << "----------------------------------------" << endl;
    cout << facade.getHallStatus(1);
    cout << facade.getHallStatus(2);
    cout << facade.getHallStatus(3);

    // 9. Доступные залы
    cout << "\n9. ДОСТУПНЫЕ ЗАЛЫ НА 15:00:" << endl;
    cout << "----------------------------------------" << endl;
    auto availableHalls = facade.getAvailableHalls("15:00");
    cout << "  Залы: ";
    for (size_t i = 0; i < availableHalls.size(); i++) {
        cout << availableHalls[i] << " ";
    }
    cout << endl;

    // 10. Бронирование билетов
    cout << "\n10. БРОНИРОВАНИЕ БИЛЕТОВ:" << endl;
    cout << "----------------------------------------" << endl;

    // Взрослый
    auto book1 = facade.bookTicket("Аватар 2", "20:30", 1, 30, false, false, "");
    if (book1.success) {
        cout << "  ✓ Взрослый: " << book1.message << endl;
        cout << "    Номер брони: " << book1.data["booking_id"] << ", Цена: " << book1.data["price"] << " руб." << endl;
    }

    // Студент
    auto book2 = facade.bookTicket("Дюна 2", "19:30", 3, 20, true, false, "");
    if (book2.success) {
        cout << "  ✓ Студент: " << book2.message << endl;
        cout << "    Номер брони: " << book2.data["booking_id"] << ", Цена: " << book2.data["price"] << " руб." << endl;
    }

    // Ребенок
    auto book3 = facade.bookTicket("Барби", "14:00", 4, 10, false, false, "");
    if (book3.success) {
        cout << "  ✓ Ребенок: " << book3.message << endl;
        cout << "    Номер брони: " << book3.data["booking_id"] << ", Цена: " << book3.data["price"] << " руб." << endl;
    }

    // VIP
    auto book4 = facade.bookTicket("Оппенгеймер", "21:00", 2, 35, false, true, "");
    if (book4.success) {
        cout << "  ✓ VIP: " << book4.message << endl;
        cout << "    Номер брони: " << book4.data["booking_id"] << ", Цена: " << book4.data["price"] << " руб." << endl;
    }

    // 11. Расчет цены без бронирования
    cout << "\n11. РАСЧЕТ ЦЕНЫ БИЛЕТОВ:" << endl;
    cout << "----------------------------------------" << endl;
    int price1 = facade.calculateTicketPrice("Аватар 2", "20:30", 1, 30, false, false);
    int price2 = facade.calculateTicketPrice("Аватар 2", "20:30", 1, 20, true, false);
    int price3 = facade.calculateTicketPrice("Аватар 2", "20:30", 1, 10, false, false);
    int price4 = facade.calculateTicketPrice("Аватар 2", "20:30", 1, 65, false, false);

    cout << "  Взрослый: " << price1 << " руб." << endl;
    cout << "  Студент: " << price2 << " руб." << endl;
    cout << "  Ребенок: " << price3 << " руб." << endl;
    cout << "  Пенсионер: " << price4 << " руб." << endl;

    // 12. Запуск сценариев
    cout << "\n12. ЗАПУСК СЦЕНАРИЕВ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.runEveningProgram();
    facade.runWeekendMarathon();

    // 13. Статистика
    cout << "\n13. СТАТИСТИКА КИНОТЕАТРА:" << endl;
    cout << "----------------------------------------" << endl;
    facade.printStatistics();

    // 14. Отчет за день
    cout << "\n14. ОТЧЕТ ЗА ДЕНЬ:" << endl;
    cout << "----------------------------------------" << endl;
    cout << facade.generateDailyReport("2026-03-20") << endl;

    // 16. Список фильмов в прокате
    cout << "\n15. ФИЛЬМЫ В ПРОКАТЕ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.listNowShowing();

    // 17. Создание группы залов
    cout << "\n16. СОЗДАНИЕ ГРУППЫ ЗАЛОВ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.createHallGroup("IMAX комплекс", { 1, 3 });

    // 18. Тест аварийного завершения
    cout << "\n17. ТЕСТ АВАРИЙНОГО ЗАВЕРШЕНИЯ:" << endl;
    cout << "----------------------------------------" << endl;
    facade.emergencyShutdown();

    // 19. Общая информация после всех операций
    cout << "\n18. ИТОГОВАЯ ИНФОРМАЦИЯ:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Всего сеансов сегодня: " << facade.getTodaySessionsCount() << endl;
    cout << "Общая вместимость: " << facade.getTotalCapacity() << " мест" << endl;
    cout << "Общая выручка: " << facade.getTotalRevenue() << " руб." << endl;

    cout << "\n=== Работа программы завершена ===" << endl;

    return 0;
}
