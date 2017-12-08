#ifndef STAVALFI_CPP_EX2_MAP_OBJECT_H
#define STAVALFI_CPP_EX2_MAP_OBJECT_H


#include "Point2d.h"
#include <string>

class MapObject {
    const std::string &id;
public:
    const std::string &getId() const;

private:
    Point2d &location;

public:
    explicit MapObject(const std::string &id, const Point2d &location);

    const Point2d &getLocation() const;

    void setLocation(int x, int y);

    virtual ~MapObject() = 0;
};


#endif //STAVALFI_CPP_EX2_MAP_OBJECT_H
