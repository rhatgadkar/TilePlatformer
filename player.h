#ifndef PLAYER_H
#define PLAYER_H

#include "movingobject.h"

class Game;

const int MAX_JUMP_COUNT = 50;

class Player : public MovingObject
{
public:
    Player(int r, int c, int w, int h, Game* g, int line) : MovingObject(r, c, 'p', w, h, g, line)
      { m_jumpCount = MAX_JUMP_COUNT; m_fallDown = false; m_canJump = false; }
    virtual ~Player() {}
    virtual void doSomething();
private:
    int m_jumpCount;
    bool m_fallDown;
    bool m_canJump;
    void movePlayer(Game* game, bool insideLeftTile, bool insideRightTile, bool insideUpTile, bool insideDownTile);
    void insideWall(Game* game, int x, int y, bool& insideLeft, bool& insideRight, bool& insideTop, bool& insideDown);
    bool insideStationaryEnemy(Game* game, int x, int y);
};

#endif // PLAYER_H
