#include "ExternalTicketAPI.h"
#include <iostream>
#include <cstdlib>

using namespace std;

string ExternalTicketAPI::createReservation(const string& eventCode, int seat) {
    cout << "  [API] Создание резерва на событие " << eventCode << ", место " << seat << endl;
    return "RES_" + to_string(rand() % 10000);
}

bool ExternalTicketAPI::cancelReservation(const string& reservationId) {
    cout << "  [API] Отмена резерва " << reservationId << endl;
    return true;
}

vector<int> ExternalTicketAPI::getFreeSeats(const string& eventCode) {
    cout << "  [API] Получение свободных мест для " << eventCode << endl;
    return { 1, 2, 3, 4, 5, 10, 11, 12 };
}