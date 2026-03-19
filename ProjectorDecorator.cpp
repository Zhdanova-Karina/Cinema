#include "ProjectorDecorator.h"

using namespace std;

ProjectorDecorator::ProjectorDecorator(unique_ptr<IProjector> projector)
    : wrappedProjector(move(projector)) {}

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

int ProjectorDecorator::getLampHours() const {
    return wrappedProjector->getLampHours();
}

void ProjectorDecorator::setLampHours(int hours) {
    wrappedProjector->setLampHours(hours);
}