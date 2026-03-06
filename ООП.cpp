// ООП.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <memory>
#include <thread>
#include "Cinema.h"
#include "Projector.h"
#include "SoundSystem.h"
#include "LightSystem.h"
#include "AirConditioner.h"
#include "DoorLock.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    cout << "=== Лабораторная работа 4: Паттерны Делегирование и Proxy ===" << endl;
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

    controller2->registerEquipment(make_shared<Projector>("proj_201"));
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

    cout << "\nЗавершение сеанса в зале 1:" << endl;
    hall1->cleanupAfterSession();

    // Демонстрация для VIP зала
    cout << "\n--- Работа с VIP залом (Зал #2) ---" << endl;
    Hall* hall2 = cinema.getHall(2);

    Session session2("Дюна 2", "20:00", 155);
    hall2->addSession(session2);
    hall2->prepareForSession(session2);

    // Демонстрация аварийной остановки
    cout << "\n--- Тест аварийной остановки ---" << endl;
    hall2->getController()->emergencyStop();

    cout << "\n=== Работа программы завершена ===" << endl;

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
