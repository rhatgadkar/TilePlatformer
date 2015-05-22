#ifndef STATIONARYENEMY_H
#define STATIONARYENEMY_H

#include "stationaryobject.h"

class Game;

class StationaryEnemy : public StationaryObject
{
public:
    StationaryEnemy(int r, int c, int w, int h, Game* g) : StationaryObject(r, c, 's', w, h, g) {}
    virtual ~StationaryEnemy() {}
};

#endif // STATIONARYENEMY_H
