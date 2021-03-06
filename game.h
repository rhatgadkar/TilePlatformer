#ifndef GAME_H
#define GAME_H

#include <list>
#include <allegro5/allegro_font.h>
#include "gameobject.h"
#include "movingobject.h"
#include "stationaryobject.h"
#include "globals.h"

class Player;

class Game
{
public:
    Game(char* mapFile);
    ~Game();
    void addNewStationaryObject(StationaryObject* stationaryobject)
      { m_stationaryobjects.push_back(stationaryobject); stationaryobject->setPos(m_stationaryCount); m_stationaryCount++; }
    void addNewMovingObject(MovingObject* movingobject)
      { m_movingobjects.push_back(movingobject); movingobject->setPos(m_movingCount); m_movingCount++; }
    bool removeStationaryObject(StationaryObject* toDelete);
    bool removeMovingObject(MovingObject* toDelete);
    void draw();
    void getInput();
    GameObject* getMap(int r, int c) const { return m_map[r][c]; }
    void setMap(int r, int c, GameObject* go) { m_map[r][c] = go; }
    Player* getPlayer() const { return m_player; }
    void drawGameOverScreen() const;
    void drawStartScreen() const;
    bool isReset() const { return m_reset; }
    void setReset(bool val) { m_reset = val; }
    void reset();
    bool getKey(int pos) const { return m_key[pos]; }
    void setKey(int pos, bool val) { m_key[pos] = val; }
    int parseMapFile();
    int getStationaryCount() const { return m_stationaryCount; }
    int getMovingCount() const { return m_movingCount; }
    int getLevelWidth() const { return m_levelWidth; }
    int getNumCols() const { return m_numCols; }
    void boundingBox(int pX, int pY, bool& insideLeft, bool& insideRight, bool& insideTop, bool& insideDown, bool& insideEnemy);
    bool inTileCol(int x) { return (x % TILE_WIDTH == 0); }
    bool inTileRow(int y) { return (y % TILE_HEIGHT == 0); }
    bool validRow(int r) { return (r >= 0 && r < NUM_ROWS); }
    bool validCol(int c) { return (c >= 0 && c < m_numCols); }
    bool validX(int x) { return (x >= 0 && x < m_levelWidth); }
    bool validY(int y) { return (y >= 0 && y < SCREEN_HEIGHT); }
private:
    char* m_mapFile;
    GameObject*** m_map;
    std::list<StationaryObject*> m_stationaryobjects;
    int m_stationaryCount;
    std::list<MovingObject*> m_movingobjects;
    int m_movingCount;
    Player* m_player;
    bool* m_key;
    bool m_reset;
    ALLEGRO_FONT* m_font;
    int m_levelWidth;
    int m_numCols;
    int m_camX;
};

#endif // GAME_H
