#ifndef WALL_H
#define WALL_H

#include "stationaryobject.h"

class Game;

class Wall : public StationaryObject
{
public:
    Wall(int r, int c, int w, int h, Game* g) : StationaryObject(r, c, 'w', w, h, g) {}
    virtual ~Wall() {}
};

#endif // WALL_H

