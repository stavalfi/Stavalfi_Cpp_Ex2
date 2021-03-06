#ifndef STAVALFI_CPP_EX2_MOVING_SOLDIER_STRATEGY_H
#define STAVALFI_CPP_EX2_MOVING_SOLDIER_STRATEGY_H

#include "MapReader.h"
#include "Healer.h"
#include "Warrior.h"
#include "SoldierStrategy.h"

class MovingSoldierStrategy : public SoldierStrategy {
public:
    explicit MovingSoldierStrategy(const std::shared_ptr<const MapReader> &mapReader);

    virtual Point2d chooseNewLocation(const Warrior &warrior) const = 0;

    virtual Point2d chooseNewLocation(const Healer &healer) const = 0;

    virtual ~MovingSoldierStrategy() = 0;

protected:
    virtual Point2d chooseNewLocationByDirection(const Warrior &warrior, const Point2d &direction) const = 0;

    virtual Point2d chooseNewLocationByDirection(const Healer &healer, const Point2d &direction) const = 0;
};


#endif //STAVALFI_CPP_EX2_MOVING_SOLDIER_STRATEGY_H
