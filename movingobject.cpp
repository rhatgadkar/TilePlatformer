#include "movingobject.h"
#include "gameobject.h"
#include "game.h"
#include <stdio.h>

MovingObject::MovingObject(int r, int c, char tile, int w, int h, Game* g) : GameObject(r, c, tile, w, h, g)
{
    if (tile != 'p' && tile != 'm')
    {
        fprintf(stderr, "%s %c %s \n", "Invalid MovingObject tile char: '", tile, "'");
        exit(0);
    }
}
