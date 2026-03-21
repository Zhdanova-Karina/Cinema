#include "BookingAdapter.h"
#include <iostream>

using namespace std;

BookingAdapter::BookingAdapter(ExternalTicketAPI* externalApi)
    : api(externalApi) {}

bool BookingAdapter::bookTicket(const string& sessionId, int seatNumber) {
    string eventCode = getEventCode(sessionId);
    string reservationId = api->createReservation(eventCode, seatNumber);

    if (!reservationId.empty()) {
        cout << "  [Адаптер] Билет забронирован: " << reservationId << endl;
        return true;
    }
    return false;
}

bool BookingAdapter::cancelBooking(const string& bookingId) {
    return api->cancelReservation(bookingId);
}

vector<int> BookingAdapter::getAvailableSeats(const string& sessionId) {
    string eventCode = getEventCode(sessionId);
    return api->getFreeSeats(eventCode);
}

string BookingAdapter::getEventCode(const string& sessionId) {
    if (sessionToEventCode.find(sessionId) == sessionToEventCode.end()) {
        sessionToEventCode[sessionId] = "EVENT_" + sessionId;
    }
    return sessionToEventCode[sessionId];
}