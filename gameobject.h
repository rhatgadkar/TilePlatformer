#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "globals.h"

class Game;

class GameObject
{
public:
    GameObject(int r, int c, char tile, int w, int h, Game* g, int line);
    virtual ~GameObject() {}
    virtual void doSomething() = 0;
    Game* getGame() const { return m_game; }
    char getTile() const { return m_tile; }
    int getPos() const { return m_pos; }
    void setPos(int pos) { m_pos = pos; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getR() const { return m_row; }
    int getC() const { return m_col; }
private:
    Game* m_game;
    char m_tile;
    int m_pos; // position in m_gameobjects list - either m_stationaryobject or m_movingobject
    int m_width; // num tile width
    int m_height; // num tile height
protected:
    int m_x;
    int m_y;
    int m_row;
    int m_col;
};

#endif // GAMEOBJECT_H
