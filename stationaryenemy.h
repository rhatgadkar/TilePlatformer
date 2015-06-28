#ifndef STATIONARYENEMY_H
#define STATIONARYENEMY_H

#include "stationaryobject.h"

class Game;

class StationaryEnemy : public StationaryObject
{
public:
    StationaryEnemy(int r, int c, int w, int h, Game* g, int line) : StationaryObject(r, c, 's', w, h, g, line) {}
    virtual ~StationaryEnemy() {}
};

#endif // STATIONARYENEMY_H
