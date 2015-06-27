#include "gameobject.h"
#include "game.h"
#include "globals.h"
#include <stdio.h>

GameObject::GameObject(int r, int c, char tile, int w, int h, Game* g)
{
    m_tile = tile;
    m_width = w;
    m_height = h;
    m_game = g;
    m_x =  c * TILE_WIDTH;
    m_y = r * TILE_HEIGHT;

    if (tile != 'w' && tile != 'p' && tile != 's' && tile != 'm')
    {
        fprintf(stderr, "%s %c %s \n", "Invalid tile char: '", tile, "'");
        exit(0);
    }
    if (!validRow(r))
    {
        fprintf(stderr, "%s %d %s \n", "Invalid row: '", r, "'");
        exit(0);
    }
    if (c < 0 || c >= g->getNumCols())
    {
        fprintf(stderr, "%s %d %s \n", "Invalid col: '", c, "'");
        exit(0);
    }
    if (w < 1 || w > g->getNumCols())
    {
        fprintf(stderr, "%s %d %s \n", "Invalid width: '", w, "'");
        exit(0);
    }
    if (h < 1 || h > NUM_ROWS)
    {
        fprintf(stderr, "%s %d %s \n", "Invalid height: '", h, "'");
        exit(0);
    }

    for (int i = c; i < c+w; i++)
    {
        char curr = g->getMap(r, i);
        if (curr != '0')
        {
            fprintf(stderr, "%s %c %s %c %s \n", "Cannot add GameObject: '", tile, "', This tile exists here: '", curr, "'");
            exit(0);
        }
    }

    for (int i = r; i < r+h; i++)
    {
        char curr = g->getMap(i, c);
        if (curr != '0')
        {
            fprintf(stderr, "%s %c %s %c %s \n", "Cannot add GameObject: '", tile, "', This tile exists here: '", curr, "'");
            exit(0);
        }
    }
}

bool GameObject::validCol(int c)
{
    return (c >= 0 && c < m_game->getNumCols());
}

bool GameObject::validX(int x)
{
    return (x >= 0 && x < m_game->getLevelWidth());
}
