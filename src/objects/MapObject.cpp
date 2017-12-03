#include "MapObject.h"
#include <cassert>

MapObject::MapObject(const std::string& id, const Point2d &location)
        : id(id), location(*(new Point2d(location))) {}

const Point2d &MapObject::getLocation() const {
    return this->location;
}

void MapObject::setLocation(int x,int y) {
    assert(x>=0 && y>=0);
    this->location.setX(x);
    this->location.setY(y);
}

MapObject::~MapObject() {
    delete (&this->location);
}

const std::string &MapObject::getId() const {
    return id;
}
