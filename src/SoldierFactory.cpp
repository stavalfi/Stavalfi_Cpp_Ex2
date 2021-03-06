#include "SoldierFactory.h"
#include "RegularHealer.h"
#include "RegularWarrior.h"
#include "SniperWarrior.h"
#include "ItemFactory.h"

std::shared_ptr<Soldier> SoldierFactory::create(SoldierType soldierType,
                                                const Point2d &location,
                                                const std::string &playerId,
                                                std::vector<Point2d> &soldierDirections) {
    static signed int id = 0;

    switch (soldierType) {
        case SoldierType::REGULAR_HEALER: {
            std::shared_ptr<Weapon> weapon(ItemFactory::create(WeaponType::HEALER_WEAPON, location, true));
            return std::make_shared<RegularHealer>(std::to_string(++id), playerId, location, soldierDirections, weapon);
        }
        case SoldierType::REGULAR_WARRIOR:
            return std::make_shared<RegularWarrior>(std::to_string(++id), playerId, location, soldierDirections);
        case SoldierType::SNIPER_WARRIOR:
            return std::make_shared<SniperWarrior>(std::to_string(++id), playerId, location, soldierDirections);
    }
}
