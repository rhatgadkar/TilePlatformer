#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H

#include "gameobject.h"

class Game;

class MovingObject : public GameObject
{
public:
    MovingObject(int r, int c, char tile, int w, int h, Game* g, int line);
    virtual ~MovingObject() {}
    virtual void doSomething() {}
    void moveLeft() { m_x--; }
    void moveRight() { m_x++; }
    void moveUp() { m_y--; }
    void moveDown() { m_y++; }
};

#endif // MOVINGOBJECT_H

