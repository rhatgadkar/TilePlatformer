#ifndef GLOBALS_H
#define GLOBALS_H

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT // 0, 1, 2, 3
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 24;

const int MAX_LEVEL_WIDTH = 3200;

const int MAX_NUM_COLS = MAX_LEVEL_WIDTH / TILE_WIDTH;

const int NUM_ROWS = SCREEN_HEIGHT / TILE_HEIGHT;

const float FPS = 60;

#endif // GLOBALS_H
