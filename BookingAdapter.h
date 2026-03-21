#pragma once

#include "IBookingSystem.h"
#include "ExternalTicketAPI.h"
#include <map>
#include <string>

using namespace std;

// Адаптер, преобразующий интерфейс ExternalTicketAPI к IBookingSystem
class BookingAdapter : public IBookingSystem {
private:
    ExternalTicketAPI* api;
    map<string, string> sessionToEventCode;

    string getEventCode(const string& sessionId);

public:
    BookingAdapter(ExternalTicketAPI* externalApi);
    ~BookingAdapter() = default;

    bool bookTicket(const string& sessionId, int seatNumber) override;
    bool cancelBooking(const string& bookingId) override;
    vector<int> getAvailableSeats(const string& sessionId) override;
};