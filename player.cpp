#include "player.h"
#include "game.h"
#include "globals.h"
#include "gameobject.h"
#include <iostream>
using namespace std;

void Player::doSomething()
{
    Game* game = getGame();
    int x = getX();
    int y = getY();

    if (insideStationaryEnemy(game, x, y))
    {
        game->reset();
        return;
    }

    bool insideLeft = false, insideRight = false, insideTop = false, insideDown = false;
    insideWall(game, x, y, insideLeft, insideRight, insideTop, insideDown);

    movePlayer(game, insideLeft, insideRight, insideTop, insideDown);
}

bool Player::insideStationaryEnemy(Game* game, int x, int y)
{
    // check left bound
    int leftBound_x = x + 1;
    int leftBound_tileX = leftBound_x - (TILE_WIDTH - 1);
    if (validX(leftBound_x) &&  inTileCol(leftBound_tileX))
    {
        int rBound = y / TILE_HEIGHT; // checks top left
        int cBound = leftBound_x / TILE_WIDTH;
        if (game->getMap(rBound, cBound) == 's')
            return true;
        int botYBound = y + (getHeight() * PLAYER_HEIGHT - 1) - 1; // checks bottom left
        if (validY(botYBound))
        {
            rBound =  botYBound / TILE_HEIGHT;
            if (game->getMap(rBound, cBound) == 's')
                return true;
        }
    }

    // check right bound
    int rightBound_x = x + (getWidth() * PLAYER_WIDTH - 1) - 1;
    if (validX(rightBound_x) && inTileCol(rightBound_x))
    {
        int rBound = y / TILE_HEIGHT; // checks top right
        int cBound = rightBound_x / TILE_WIDTH;
        if (game->getMap(rBound, cBound) == 's')
            return true;
        int botYBound = y + (getHeight() * PLAYER_HEIGHT - 1) - 1; // checks bottom right
        if (validY(botYBound))
        {
            rBound =  botYBound / TILE_HEIGHT;
            if (game->getMap(rBound, cBound) == 's')
                return true;
        }
    }

    // check up bound
    int upBound_y = y + 1;
    int upBound_tileY = upBound_y - (TILE_HEIGHT - 1);
    if (validY(upBound_y) && inTileRow(upBound_tileY))
    {
        int rBound = upBound_y / TILE_HEIGHT;
        int cBound = x / TILE_WIDTH; // checks top left
        if (game->getMap(rBound, cBound) == 's')
            return true;
        int rightXBound = x + (getWidth() * PLAYER_WIDTH - 1) - 1; // checks top right
        if (validX(rightXBound))
        {
            cBound =  rightXBound / TILE_WIDTH;
            if (game->getMap(rBound, cBound) == 's')
                return true;
        }
    }

    // check down bound
    int downBound_y = y + (getHeight() * PLAYER_HEIGHT);
    if (validY(downBound_y) && inTileRow(downBound_y))
    {
        int rBound = downBound_y / TILE_HEIGHT;
        int cBound = x / TILE_WIDTH; // checks bottom left
        if (game->getMap(rBound, cBound) == 's')
            return true;
        int rightXBound = x + (getWidth() * PLAYER_WIDTH - 1) - 1; // checks bottom right
        if (validX(rightXBound))
        {
            cBound =  rightXBound / TILE_WIDTH;
            if (game->getMap(rBound, cBound) == 's')
                return true;
        }
    }

    return false;
}

void Player::movePlayer(Game* game, bool insideLeftTile, bool insideRightTile, bool insideUpTile, bool insideDownTile)
{
    if (game->getKey(KEY_LEFT))
    {
        if (!insideLeftTile)
            moveLeft();
    }

    if (game->getKey(KEY_RIGHT))
    {
        if (!insideRightTile)
            moveRight();
    }

    if (!insideDownTile && m_fallDown)
    {
        moveDown();
        return;
    }

    if (insideDownTile)
    {
        m_jumpCount = MAX_JUMP_COUNT;
        m_fallDown = false;
    }

    if (game->getKey(KEY_UP))
    {
        if (insideUpTile && !insideDownTile)
        {
            moveDown();
            m_fallDown = true;
        }
        else if (!insideUpTile && !insideDownTile && m_jumpCount > 0 && m_jumpCount < MAX_JUMP_COUNT)
        {
            moveUp();
            m_jumpCount++;
        }
        else if (!insideUpTile && !insideDownTile && m_jumpCount >= MAX_JUMP_COUNT)
        {
            moveDown();
        }
        else if (!insideUpTile && insideDownTile && m_jumpCount == MAX_JUMP_COUNT && m_canJump)
        {
            m_jumpCount = 0;
            moveUp();
            m_jumpCount++;
            m_canJump = false;
        }
    }
    else
    {
        m_jumpCount = MAX_JUMP_COUNT;

        if (insideDownTile && insideUpTile)
            m_canJump = false;
        else if (insideDownTile)
        {
            m_canJump = true;
        }
        else
        {
            moveDown();
            m_canJump = false;
        }
    }
}

void Player::insideWall(Game* game, int x, int y, bool& insideLeft, bool& insideRight, bool& insideTop, bool& insideDown)
{
    int width = getWidth();
    int height = getHeight();

    // top edge detect
    int top_tile_r = (y / TILE_HEIGHT) - 1;

    for (int i = 0; i < width && !insideTop; i++)
    {
        int top_tile_c = (x + (PLAYER_WIDTH * i)) / TILE_WIDTH;

        if (validRow(top_tile_r) && validCol(top_tile_c))
        {
            if (inTileRow(y) && !inTileCol(x))
            {
                if (game->getMap(top_tile_r, top_tile_c) != 'w')
                {
                    top_tile_c++;
                    if (validCol(top_tile_c) && game->getMap(top_tile_r, top_tile_c) == 'w')
                        insideTop = true;
                }
                else
                    insideTop = true;
            }
            if (inTileCol(x) && inTileRow(y))
            {
                if (game->getMap(top_tile_r, top_tile_c) == 'w')
                    insideTop = true;
            }
        }
    }

    // bot edge detect
    int bot_tile_r = (y + (height * PLAYER_HEIGHT)) / TILE_HEIGHT;

    for (int i = 0; i < width && !insideDown; i++)
    {
        int bot_tile_c = (x + (PLAYER_WIDTH * i)) / TILE_WIDTH;

        if (validRow(bot_tile_r) && validCol(bot_tile_c))
        {
            if (inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)) && !inTileCol(x))
            {
                if (game->getMap(bot_tile_r, bot_tile_c) != 'w')
                {
                    bot_tile_c++;
                    if (validCol(bot_tile_c) && game->getMap(bot_tile_r, bot_tile_c) == 'w')
                        insideDown = true;
                }
                else
                    insideDown = true;
            }
            if (inTileCol(x) && inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)))
            {
                if (game->getMap(bot_tile_r, bot_tile_c) == 'w')
                    insideDown = true;
            }
        }

        if (insideDown)
            break;
    }

    bool checkBotLeft = false, checkBotRight = false, checkTopLeft = false, checkTopRight = false;

    // left edge detect
    int left_tile_c = (x / TILE_WIDTH) - 1;

    for (int i = 0; i < height; i++)
    {
        int left_tile_r = (y + (PLAYER_HEIGHT * i)) / TILE_HEIGHT;

        if (validRow(left_tile_r) && validCol(left_tile_c))
        {
            if (inTileCol(x) && !inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)))
            {
                if (game->getMap(left_tile_r, left_tile_c) != 'w')
                {
                    left_tile_r++;
                    if (validRow(left_tile_r) && game->getMap(left_tile_r, left_tile_c) == 'w')
                        insideLeft = true;
                }
                else
                    insideLeft = true;
            }
            else if (inTileCol(x) && inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)))
            {
                if (game->getMap(left_tile_r, left_tile_c) == 'w')
                    insideLeft = true;
                else
                {
                    int bot_left_tile_r = left_tile_r + (height * PLAYER_HEIGHT / TILE_HEIGHT);
                    int bot_left_tile_c = left_tile_c;
                    if (validRow(bot_left_tile_r) && game->getMap(bot_left_tile_r, bot_left_tile_c) == 'w')
                    {
                        checkBotLeft = true;
                    }

                    int top_left_tile_r = left_tile_r - 1;
                    int top_left_tile_c = left_tile_c;
                    if (validRow(top_left_tile_r) && game->getMap(top_left_tile_r, top_left_tile_c) == 'w')
                    {
                        checkTopLeft = true;
                    }
                }
            }
        }
    }

    // right edge detect
    int right_tile_c = (x + (width * PLAYER_WIDTH)) / TILE_WIDTH;

    for (int i = 0; i < height; i++)
    {
        int right_tile_r = (y + (PLAYER_HEIGHT * i)) / TILE_HEIGHT;

        if (validRow(right_tile_r) && validCol(right_tile_c))
        {
            if (inTileCol(x - (TILE_WIDTH - PLAYER_WIDTH)) && !inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)))
            {
                if (game->getMap(right_tile_r, right_tile_c) != 'w')
                {
                    right_tile_r++;
                    if (validRow(right_tile_r) && game->getMap(right_tile_r, right_tile_c) == 'w')
                        insideRight = true;
                }
                else
                    insideRight = true;
            }
            else if (inTileCol(x - (TILE_WIDTH - PLAYER_WIDTH)) && inTileRow(y - (TILE_HEIGHT - PLAYER_HEIGHT)))
            {
                if (game->getMap(right_tile_r, right_tile_c) == 'w')
                    insideRight = true;
                else
                {
                    int bot_right_tile_r = right_tile_r + (height * PLAYER_HEIGHT / TILE_HEIGHT);
                    int bot_right_tile_c = right_tile_c;
                    if (validRow(bot_right_tile_r) && game->getMap(bot_right_tile_r, bot_right_tile_c) == 'w')
                    {
                        checkBotRight = true;
                    }

                    int top_right_tile_r = right_tile_r - 1;
                    int top_right_tile_c = right_tile_c;
                    if (validRow(top_right_tile_r) && game->getMap(top_right_tile_r, top_right_tile_c) == 'w')
                    {
                        checkTopRight = true;
                    }
                }
            }
        }
    }

    if (checkTopLeft && checkBotLeft)
    {
        insideTop = true;
        insideDown = true;
    }
    if (checkTopRight && checkBotRight)
    {
        insideTop = true;
        insideDown = true;
    }
    if (checkBotRight && !checkTopRight && !insideLeft)
        insideDown = true;
    if (checkBotLeft && !checkTopLeft && !insideRight)
        insideDown = true;
    if (checkTopLeft && !checkBotLeft && !insideRight)
        insideTop = true;
    if (checkTopRight && !checkBotRight && !insideLeft)
        insideTop = true;
}
