#include "movingobject.h"
#include "gameobject.h"
#include "game.h"
#include <stdio.h>

MovingObject::MovingObject(int r, int c, char tile, int w, int h, Game* g, int line) : GameObject(r, c, tile, w, h, g, line)
{
    if (tile != 'p' && tile != 'm')
    {
        fprintf(stderr, "%s %c %s %d \n", "Invalid MovingObject tile char: '", tile, "' at line: ", line);
        exit(0);
    }
}
