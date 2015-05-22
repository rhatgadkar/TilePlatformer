#include "stationaryobject.h"
#include "game.h"
#include <stdio.h>

StationaryObject::StationaryObject(int r, int c, char tile, int w, int h, Game* g) : GameObject(r, c, tile, w, h, g)
{
    if (tile != 'w' && tile != 's')
    {
        fprintf(stderr, "%s %c %s \n", "Invalid StationaryObject tile char: '", tile, "'");
        exit(0);
    }

    m_row = r;
    m_col = c;

    for (int rCount = r; rCount < r + h; rCount++)
    {
        for (int cCount = c; cCount < c + w; cCount++)
            g->setMap(rCount, cCount, tile);
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
            game->setMap(rCount, cCount, '0');
    }
}
