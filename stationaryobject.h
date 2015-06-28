#ifndef STATIONARYOBJECT_H
#define STATIONARYOBJECT_H

#include "gameobject.h"

class Game;

class StationaryObject : public GameObject
{
public:
    StationaryObject(int r, int c, char tile, int w, int h, Game* g, int line);
    virtual ~StationaryObject();
    virtual void doSomething() {}
};

#endif // STATIONARYOBJECT_H
