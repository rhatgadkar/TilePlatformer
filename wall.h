#ifndef WALL_H
#define WALL_H

#include "stationaryobject.h"

class Game;

class Wall : public StationaryObject
{
public:
    Wall(int r, int c, int w, int h, Game* g, int line) : StationaryObject(r, c, 'w', w, h, g, line) {}
    virtual ~Wall() {}
};

#endif // WALL_H

