#include "stationaryobject.h"
#include "game.h"
#include <stdio.h>

StationaryObject::StationaryObject(int r, int c, char tile, int w, int h, Game* g, int line)
  : GameObject(r, c, tile, w, h, g, line)
{
    if (tile != 'w' && tile != 's')
    {
        fprintf(stderr, "%s %c %s %d \n", "Invalid StationaryObject tile char: '", tile, "' at line: ", line);
        exit(0);
    }
}

StationaryObject::~StationaryObject()
{
    Game* game = getGame();
    int h = getHeight();
    int w = getWidth();

    for (int rCount = m_row; rCount < m_row + h; rCount++)
    {
        for (int cCount = m_col; cCount < m_col + w; cCount++)
            game->setMap(rCount, cCount, NULL);
    }
}
