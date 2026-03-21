#include "ProjectorDecorator.h"

using namespace std;

ProjectorDecorator::ProjectorDecorator(Projector* projector)
    : Projector("") {  // Временный ID
    wrappedProjector = projector;
}

void ProjectorDecorator::turnOn() {
    wrappedProjector->turnOn();
}

void ProjectorDecorator::turnOff() {
    wrappedProjector->turnOff();
}

void ProjectorDecorator::play(const string& fileName) {
    wrappedProjector->play(fileName);
}

string ProjectorDecorator::getStatus() const {
    return wrappedProjector->getStatus();
}

string ProjectorDecorator::getType() const {
    return wrappedProjector->getType();
}

void ProjectorDecorator::stop() {
    wrappedProjector->stop();
}
