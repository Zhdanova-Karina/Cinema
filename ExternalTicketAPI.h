#pragma once

#include <string>
#include <vector>
#include <iostream>

using namespace std;

// Адаптируемый класс - внешний API билетного оператора
class ExternalTicketAPI {
public:
    string createReservation(const string& eventCode, int seat);
    bool cancelReservation(const string& reservationId);
    vector<int> getFreeSeats(const string& eventCode);
};