#pragma once

#include <string>
#include <vector>

using namespace std;

// Целевой интерфейс для системы бронирования
class IBookingSystem {
public:
    virtual ~IBookingSystem() = default;
    virtual bool bookTicket(const string& sessionId, int seatNumber) = 0;
    virtual bool cancelBooking(const string& bookingId) = 0;
    virtual vector<int> getAvailableSeats(const string& sessionId) = 0;
};