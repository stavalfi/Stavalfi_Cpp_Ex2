#include "ComputerMovingSoldierStrategy.h"

ComputerMovingSoldierStrategy::ComputerMovingSoldierStrategy(const std::shared_ptr<const MapReader> &mapReader)
        : MovingSoldierStrategy(
        mapReader) {}

Point2d ComputerMovingSoldierStrategy::chooseNewLocation(const Warrior &warrior) const {
    if (warrior.getSoldierDirections().isEnabled() &&
            warrior.getSoldierDirections().anyLeft()) {
        Point2d direction = warrior.getSoldierDirections().getNextDirection();
        chooseNewLocationByDirection(warrior, direction);
    }
    return warrior.getLocation();
}

Point2d ComputerMovingSoldierStrategy::chooseNewLocation(const Healer &healer) const {
    if (healer.getSoldierDirections().isEnabled() &&
            healer.getSoldierDirections().anyLeft()) {
        Point2d direction = healer.getSoldierDirections().getNextDirection();
        chooseNewLocationByDirection(healer, direction);
    }
    return healer.getLocation();
}

Point2d
ComputerMovingSoldierStrategy::chooseNewLocationByDirection(const Warrior &warrior, const Point2d &direction) const {
    if (warrior.getLocation().distance(direction) <= warrior.getRunningDistance())
        return direction;
    return warrior.getLocation();
}

Point2d
ComputerMovingSoldierStrategy::chooseNewLocationByDirection(const Healer &healer, const Point2d &direction) const {
    if (healer.getLocation().distance(direction) <= healer.getRunningDistance())
        return direction;
    return healer.getLocation();
}