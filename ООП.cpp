#include <iostream>
#include <memory>
#include <thread>
#include "Cinema.h"
#include "Projector.h"
#include "SoundSystem.h"
#include "ScheduledProjector.h"
#include "LightSystem.h"
#include "AirConditioner.h"
#include "DoorLock.h"
#include "IMediaPlayer.h"
#include "ImaxAdapter.h"
#include "HallGroupFactory.h"
#include "ScheduleIterator.h"

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

    // Демонстрация для IMAX зала
    cout << "\n--- Работа с IMAX залом (Зал #1) ---" << endl;
    Hall* hall1 = cinema.getHall(1);

    Session session1("Аватар 2", "18:30", 180);
    hall1->addSession(session1);
    hall1->prepareForSession(session1);

    cout << "\nСтатус оборудования в зале 1:" << endl;
    auto statuses1 = hall1->getController()->getStatuses();
    for (const auto& status : statuses1) {
        cout << "  " << status << endl;
    }

    // ========== ТЕСТИРОВАНИЕ ДЕКОРАТОРА-ПЛАНИРОВЩИКА ==========
    cout << "\n=== ТЕСТИРОВАНИЕ ДЕКОРАТОРА-ПЛАНИРОВЩИКА ===\n" << endl;

    // Создаем обычный проектор
    auto basicProjector = make_unique<StandardProjector>("STD-001");

    // Оборачиваем его в декоратор с планировщиком
    auto scheduledProjector = make_unique<ScheduledProjector>(move(basicProjector));

    // Запускаем планировщик
    scheduledProjector->startScheduler();

    // Получаем текущее время
    time_t now = time(nullptr);

    // Планируем задачи с интервалами
    cout << "\n--- Планирование задач ---" << endl;
    scheduledProjector->scheduleTurnOn(now + 2);      // Через 2 секунды
    scheduledProjector->schedulePlay(now + 4, "Дюна.mp4");   // Через 4 секунды
    scheduledProjector->scheduleTurnOff(now + 6);     // Через 6 секунд

    // Показываем расписание
    cout << "\n--- Текущее расписание ---" << endl;
    scheduledProjector->showSchedule();

    // Имитация работы в реальном времени
    cout << "\n--- Ожидание выполнения задач (10 секунд) ---" << endl;
    for (int i = 1; i <= 10; i++) {
        this_thread::sleep_for(chrono::seconds(1));
        cout << "\nСекунда " << i << ":" << endl;

        // Показываем статус на определенных секундах
        if (i == 3 || i == 5 || i == 7) {
            cout << scheduledProjector->getStatus() << endl;
        }
    }

    // Останавливаем планировщик
    scheduledProjector->stopScheduler();

    // Демонстрация ручного управления
    cout << "\n--- Ручное управление ---" << endl;
    scheduledProjector->turnOn();
    scheduledProjector->play("Аватар 2.mp4");
    scheduledProjector->turnOff();

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

    // Получаем залы для добавления сеансов
    Hall* hall2 = cinema.getHall(2);
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

    cout << "=== ТЕСТИРОВАНИЕ ПРОЕКТОРОВ ===\n" << endl;

    // 1. Обычное использование (без адаптера)
    cout << "1. Прямое использование проекторов:" << endl;

    auto imax = make_unique<ImaxProjector>("IMAX-001");
    auto threeD = make_unique<ThreeDProjector>("3D-001");

    imax->turnOn();
    imax->play("Dune.mp4");
    imax->calibrate();  // Специфический метод IMAX
    imax->turnOff();

    cout << endl;
    threeD->turnOn();
    threeD->enable3DMode();
    threeD->play("Avatar_2.mp4");
    threeD->getStatus();

    cout << "\n2. Использование через IMediaPlayer:" << endl;

    vector<unique_ptr<IMediaPlayer>> players;
    auto imax2 = make_unique<ImaxProjector>("IMAX-002");
    auto threeD2 = make_unique<ThreeDProjector>("3D-002");

    // Включаем проекторы до добавления в вектор
    imax2->turnOn();
    threeD2->turnOn();

    players.push_back(move(imax2));
    players.push_back(move(threeD2));

    for (auto& player : players) {
        player->play("Oppenheimer.mp4");
    }

    // 3. Использование адаптера
    cout << "\n3. Использование адаптера:" << endl;

    auto adapter = make_unique<ImaxAdapter>(make_unique<ImaxProjector>("IMAX-003"));
    adapter->play("Interstellar.mp4");


    cout << "\nЗавершение сеанса в зале 1:" << endl;
    hall1->cleanupAfterSession();

    // Демонстрация для VIP зала
    cout << "\n--- Работа с VIP залом (Зал #2) ---" << endl;

    Session session2("Дюна 2", "20:00", 155);
    hall2->addSession(session2);
    hall2->prepareForSession(session2);

    // Демонстрация аварийной остановки
    cout << "\n--- Тест аварийной остановки ---" << endl;
    hall2->getController()->emergencyStop();

    cout << "\n=== Работа программы завершена ===" << endl;

    return 0;
}
