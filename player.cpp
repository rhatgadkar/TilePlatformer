#include "player.h"
#include "game.h"
#include "globals.h"
#include "gameobject.h"

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
    // check left bound
    int leftBound_x = x + 1;
    int leftBound_tileX = leftBound_x - (TILE_WIDTH - 1);
    if (validX(leftBound_x) && inTileCol(leftBound_tileX))
    {
        int rBound = y / TILE_HEIGHT; // checks top left
        int cBound = leftBound_x / TILE_WIDTH;
        if (game->getMap(rBound, cBound) == 'w')
            insideLeft = true;
        int botYBound = y + (getHeight() * PLAYER_HEIGHT - 1) - 1; // checks bottom left
        if (validY(botYBound))
        {
            rBound =  botYBound / TILE_HEIGHT;
            if (game->getMap(rBound, cBound) == 'w')
                insideLeft = true;
        }
    }

    // check right bound
    int rightBound_x = x + (getWidth() * PLAYER_WIDTH - 1) - 1;
    if (validX(rightBound_x) && inTileCol(rightBound_x))
    {
        int rBound = y / TILE_HEIGHT; // checks top right
        int cBound = rightBound_x / TILE_WIDTH;
        if (game->getMap(rBound, cBound) == 'w')
            insideRight = true;
        int botYBound = y + (getHeight() * PLAYER_HEIGHT - 1) - 1; // checks bottom right
        if (validY(botYBound))
        {
            rBound =  botYBound / TILE_HEIGHT;
            if (game->getMap(rBound, cBound) == 'w')
                insideRight = true;
        }
    }

    // check up bound
    int upBound_y = y + 1;
    int upBound_tileY = upBound_y - (TILE_HEIGHT - 1);
    if (validY(upBound_y) && inTileRow(upBound_tileY))
    {
        int rBound = upBound_y / TILE_HEIGHT;
        int cBound = x / TILE_WIDTH; // checks top left
        if (game->getMap(rBound, cBound) == 'w')
            insideTop = true;
        int rightXBound = x + (getWidth() * PLAYER_WIDTH - 1) - 1; // checks top right
        if (validX(rightXBound))
        {
            cBound =  rightXBound / TILE_WIDTH;
            if (game->getMap(rBound, cBound) == 'w')
                insideTop = true;
        }
    }

    // check down bound
    int downBound_y = y + (getHeight() * PLAYER_HEIGHT);
    if (validY(downBound_y) && inTileRow(downBound_y))
    {
        int rBound = downBound_y / TILE_HEIGHT;
        int cBound = x / TILE_WIDTH; // checks bottom left
        if (game->getMap(rBound, cBound) == 'w')
            insideDown = true;
        int rightXBound = x + (getWidth() * PLAYER_WIDTH - 1) - 1; // checks bottom right
        if (validX(rightXBound))
        {
            cBound =  rightXBound / TILE_WIDTH;
            if (game->getMap(rBound, cBound) == 'w')
                insideDown = true;
        }
    }
}
