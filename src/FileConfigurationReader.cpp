#include "FileConfigurationReader.h"
#include "SoldierStrategyFactory.h"
#include <cassert>
#include <sstream>


FileConfigurationReader::FileConfigurationReader(const std::string &fileGameConfigurationLocation,
                                                 const std::string &fileSoldierDirectionsLocation)
        : ConfigurationReader(fileGameConfigurationLocation, fileSoldierDirectionsLocation) {
    this->playerTypes["human"] = PlayerType::HUMAN;
    this->playerTypes["computer"] = PlayerType::COMPUTER;
    this->soldierTypes["normal"] = SoldierType::REGULAR_WARRIOR;
    this->soldierTypes["paramedic"] = SoldierType::REGULAR_HEALER;
    this->soldierTypes["sniper"] = SoldierType::SNIPER_WARRIOR;
    this->weaponTypes[""] = WeaponType::HEALER_WEAPON;
    this->weaponTypes["UZI"] = WeaponType::UZI_RIFLE;
    this->weaponTypes["M16"] = WeaponType::M16_RIFLE;
    this->weaponTypes["Missile"] = WeaponType::MISSILE;
    this->armorTypes["ShieldArmor"] = ArmorType::SHIELD_ARMOR;
    this->armorTypes["BodyArmor"] = ArmorType::BODY_ARMOR;
    this->solidItemTypes["Tree"] = SolidItemType::REGULAR_SOLID_ITEM;
    this->itemType["Armor"] = ItemType::ARMOR;
    this->itemType["weapon"] = ItemType::WEAPON;
    this->itemType["solid"] = ItemType::SOLID_ITEM;
}

ConfigurationReader::MapSize FileConfigurationReader::getMapSize() const {
    std::ifstream reader;
    reader.open(this->getFileGameConfigurationLocation(), std::ios::in);
    std::string s1 = "";
    while (s1 != "battlefieldSize") {
        assert(reader.good());
        reader >> s1;
    }

    signed int mapWidth, mapHigh;
    reader >> mapWidth >> mapHigh;
    assert(reader.good());

    reader.close();
    return MapSize(mapWidth, mapHigh);
}

std::list<std::shared_ptr<Player>>
FileConfigurationReader::getPlayers(const std::shared_ptr<const MapReader> &map) const {
    std::ifstream reader;
    reader.open(this->getFileGameConfigurationLocation(), std::ios::in);
    std::list<std::shared_ptr<Player>> players;

    std::string s1 = "";
    while (s1 != "players") {
        assert(reader.good());
        reader >> s1;
    }

    std::size_t playersAmount, SoldiersAmountForPlayer;
    // s1 here is equal too "soldier".
    // file structure:
    // players	2
    // soldiers	3
    reader >> playersAmount >> s1 >> SoldiersAmountForPlayer;
    assert(s1 == "soldiers");
    assert(reader.good());

    std::string temp;//read until end of this line
    std::getline(reader, temp); // temp contains only spaces

    for (size_t i = 0; i < playersAmount; i++) {
        players.push_back(getPlayer(map, reader, SoldiersAmountForPlayer));
        assert(reader.good());
    }

    return players;
}

std::shared_ptr<Player>
FileConfigurationReader::getPlayer(const std::shared_ptr<const MapReader> &map, std::ifstream &reader,
                                   std::size_t SoldiersAmountForPlayer) const {
    assert(reader.good());

    std::shared_ptr<Player> player;

    std::string temp, playerType;
    // file structure:
    // p1	human
    reader >> temp >> playerType;
    std::getline(reader, temp); // temp = only spaces

    bool soldierDirectionsExist;
    switch (this->playerTypes.at(playerType)) {
        case PlayerType::HUMAN:
            player = PlayerFactory::create(this->playerTypes.at(playerType),
                                           SoldierStrategyFactory::get(
                                                   AttackingStrategyType::DEFAULT_ATTACKING_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   ChoosingWeaponStrategyType::DEFAULT_CHOOSING_WEAPON_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   ChoosingArmorStrategyType::DEFAULT_CHOOSING_ARMOR_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   HealingStrategyType::DEFAULT_HEALING_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   MovingSoldierStrategyType::HUMAN_MOVING_SOLDIER_STRATEGY, map));
            soldierDirectionsExist = true;
            break;
        case PlayerType::COMPUTER:
            player = PlayerFactory::create(this->playerTypes.at(playerType),
                                           SoldierStrategyFactory::get(
                                                   AttackingStrategyType::DEFAULT_ATTACKING_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   ChoosingWeaponStrategyType::DEFAULT_CHOOSING_WEAPON_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   ChoosingArmorStrategyType::DEFAULT_CHOOSING_ARMOR_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   HealingStrategyType::DEFAULT_HEALING_STRATEGY, map),
                                           SoldierStrategyFactory::get(
                                                   MovingSoldierStrategyType::COMPUTER_MOVING_SOLDIER_STRATEGY, map));
            soldierDirectionsExist = false;
            break;
    }

    // get all the soldiers of this player from
    // this file by this reader object
    std::list<std::shared_ptr<Soldier>> playerSoldiers = getSoldiersOfPlayer(reader, player->getPlayerId(),
                                                                             SoldiersAmountForPlayer,
                                                                             soldierDirectionsExist);

    assert(playerSoldiers.size() == SoldiersAmountForPlayer);

    for (auto &soldier:playerSoldiers)
        player->addSoldier(soldier);

    return player;
}

const std::list<std::shared_ptr<Soldier>>
FileConfigurationReader::getSoldiersOfPlayer(std::ifstream &reader,
                                             const std::string &playerId,
                                             std::size_t SoldiersAmountForPlayer,
                                             bool soldierDirectionsExist) const {
    assert(reader.good());
    std::list<std::shared_ptr<Soldier>> soldiers;

    for (size_t i = 0; i < SoldiersAmountForPlayer; i++)
        soldiers.push_back(getSoldier(reader, playerId, soldiers.size(), soldierDirectionsExist));

    return soldiers;
}

std::shared_ptr<Soldier>
FileConfigurationReader::getSoldier(std::ifstream &reader,
                                    const std::string &playerId,
                                    size_t soldierIndex,
                                    bool soldierDirectionsExist) const {
    std::string line;
    std::getline(reader, line);
    assert(reader.good());

    std::stringstream stringstream(line);

    std::string soldierType, weaponType;
    char temp;
    double y, x;
    stringstream >> soldierType >> temp >> x >> y >> temp;

    std::vector<Point2d> soldierDirections;

    if (soldierDirectionsExist)
        soldierDirections = getSoldiersDirections(soldierIndex);

    std::shared_ptr<Soldier> soldier = SoldierFactory::create(this->soldierTypes.at(soldierType), Point2d(y, x),
                                                              playerId, soldierDirections);
    Soldier &soldier1 = *soldier;
    Warrior *warrior = nullptr;
    switch (this->soldierTypes.at(soldierType)) {
        case SoldierType::REGULAR_WARRIOR: {
            stringstream >> weaponType;
            warrior = dynamic_cast<Warrior *>(&soldier1);
            warrior->setWeapon(ItemFactory::create(this->weaponTypes.at(weaponType),
                                                   soldier->getLocation(), true));
            break;
        }
        case SoldierType::SNIPER_WARRIOR: {
            stringstream >> weaponType;
            warrior = dynamic_cast<Warrior *>(&soldier1);
            warrior->setWeapon(ItemFactory::create(this->weaponTypes.at(weaponType),
                                                   soldier->getLocation(), true));
            break;
        }

    }

    return soldier;
}

std::vector<Point2d> FileConfigurationReader::getSoldiersDirections(size_t soldierIndex) const {
    std::vector<Point2d> directions;

    std::ifstream reader;
    reader.open(getFileSoldierDirectionsLocation(), std::ios::in);
    std::string line;
    for (int i = 0; i <= soldierIndex + 1; i++)
        std::getline(reader, line);

    std::stringstream stringstream(line);
    assert(stringstream.good());
    while (true) {
        char temp;
        double x, y;
        stringstream >> temp;
        if (stringstream.eof())
            break;
        stringstream >> x >> y >> temp;
        assert(stringstream.good());
        directions.emplace_back(y, x);
    }
    reader.close();
    return directions;
}

std::list<std::shared_ptr<Armor>> FileConfigurationReader::getArmorsInMap() const {
    std::list<std::shared_ptr<Armor>> armors;

    std::ifstream reader;
    reader.open(this->getFileGameConfigurationLocation(), std::ios::in);
    std::string line;
    std::getline(reader, line);
    while (!reader.eof()) {
        assert(reader.good());
        if (line.find("Armor") != std::string::npos)
            armors.push_back(getArmorInMap(std::stringstream(line)));
        std::getline(reader, line);
    }
    reader.close();
    return armors;
}

std::list<std::shared_ptr<Weapon>> FileConfigurationReader::getWeaponsInMap() const {
    std::list<std::shared_ptr<Weapon>> weapons;

    std::ifstream reader;
    reader.open(this->getFileGameConfigurationLocation(), std::ios::in);
    std::string line;
    std::getline(reader, line);
    while (!reader.eof()) {
        assert(reader.good());
        if (line.find("weapon") != std::string::npos)
            weapons.push_back(getWeaponInMap(std::stringstream(line)));
        std::getline(reader, line);
    }
    reader.close();
    return weapons;
}

std::list<std::shared_ptr<const SolidItem>> FileConfigurationReader::getSolidItemsInMap() const {
    std::list<std::shared_ptr<const SolidItem>> solidItems;

    std::ifstream reader;
    reader.open(this->getFileGameConfigurationLocation(), std::ios::in);
    std::string line;
    std::getline(reader, line);
    while (!reader.eof()) {
        assert(reader.good());
        if (line.find("solid") != std::string::npos)
            solidItems.push_back(getSolidItemInMap(std::stringstream(line)));
        std::getline(reader, line);
    }
    reader.close();
    return solidItems;
}

std::shared_ptr<const SolidItem> FileConfigurationReader::getSolidItemInMap(std::stringstream solidItemLine) const {
    std::string solidItem, solidItemType;
    char temp;
    double x, y;
    solidItemLine >> solidItem >> solidItemType >> temp >> x >> y >> temp;
    return ItemFactory::create(this->solidItemTypes.at(solidItemType), Point2d(y, x));
}

std::shared_ptr<Weapon> FileConfigurationReader::getWeaponInMap(std::stringstream weaponLine) const {
    std::string weapon, weaponType;
    char temp;
    double x, y;
    weaponLine >> weapon >> weaponType >> temp >> x >> y >> temp;
    return ItemFactory::create(this->weaponTypes.at(weaponType), Point2d(y, x), false);
}

std::shared_ptr<Armor> FileConfigurationReader::getArmorInMap(std::stringstream armorLine) const {
    std::string armor, armorType;
    char temp;
    double armorStrength, x, y;
    armorLine >> armor >> armorType >> armorStrength >> temp >> x >> y >> temp;
    return ItemFactory::create(this->armorTypes.at(armorType), Point2d(y, x), false);
}

