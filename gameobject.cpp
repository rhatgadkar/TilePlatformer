#include "gameobject.h"
#include "game.h"
#include "globals.h"
#include <stdio.h>

GameObject::GameObject(int r, int c, char tile, int w, int h, Game* g, int line)
{
    m_tile = tile;
    m_width = w;
    m_height = h;
    m_game = g;
    m_x =  c * TILE_WIDTH;
    m_y = r * TILE_HEIGHT;
    m_row = r;
    m_col = c;

    if (tile != 'w' && tile != 'p' && tile != 's' && tile != 'm')
    {
        fprintf(stderr, "%s %c %s %d \n", "Invalid tile char: '", tile, "' at line: ", line);
        exit(0);
    }
    if (!g->validRow(r))
    {
        fprintf(stderr, "%s %d %s %d \n", "Invalid row: '", r, "' at line: ", line);
        exit(0);
    }
    if (c < 0 || c >= g->getNumCols())
    {
        fprintf(stderr, "%s %d %s %d \n", "Invalid col: '", c, "' at line: ", line);
        exit(0);
    }
    if (w < 1 || w > g->getNumCols())
    {
        fprintf(stderr, "%s %d %s %d \n", "Invalid width: '", w, "' at line: ", line);
        exit(0);
    }
    if (h < 1 || h > NUM_ROWS)
    {
        fprintf(stderr, "%s %d %s %d \n", "Invalid height: '", h, "' at line: ", line);
        exit(0);
    }

    for (int rCount = r; rCount < r + h; rCount++)
    {
        for (int cCount = c; cCount < c + w; cCount++)
        {
            char curr = g->getMap(rCount, cCount);
            if (curr != '0')
            {
                fprintf(stderr, "%s %c %s %c %s %d \n", "Cannot add GameObject: '", tile, "', This tile exists here: '", curr, "'. Line: ", line);
                exit(0);
            }

            g->setMap(rCount, cCount, tile);
        }
    }
}
