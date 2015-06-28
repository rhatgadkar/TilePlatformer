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
    bool inTileCol(int x) { return (x % TILE_WIDTH == 0); }
    bool inTileRow(int y) { return (y % TILE_HEIGHT == 0); }
    bool validRow(int r) { return (r >= 0 && r < NUM_ROWS); }
    bool validCol(int c);
    bool validX(int x);
    bool validY(int y) { return (y >= 0 && y < SCREEN_HEIGHT); }
private:
    Game* m_game;
    char m_tile;
    int m_pos; // position in m_gameobjects list - either m_stationaryobject or m_movingobject
    int m_width; // num tile width
    int m_height; // num tile height
protected:
    int m_x;
    int m_y;
};

#endif // GAMEOBJECT_H
