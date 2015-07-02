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
    int playerC = x / TILE_WIDTH;
    int playerR = y / TILE_HEIGHT;
    int x_end = x + PLAYER_WIDTH;
    int y_end = y + PLAYER_HEIGHT;

    // top row
    int topRow = playerR - 1;
    if (validRow(topRow))
    {
        int topRow_y = topRow * TILE_HEIGHT;
        int topRow_y_end = topRow_y + TILE_HEIGHT;

        if (topRow_y_end == y)
            insideTop = true;
    }

    // bot row
    int botRow = playerR + 1;
    if (validRow(botRow))
    {
        int botRow_y = botRow * TILE_HEIGHT;

        if (botRow_y == y_end)
            insideDown = true;
    }

    // left col
    int leftCol = playerC - 1;
    if (validCol(leftCol))
    {
        int leftCol_x = leftCol * TILE_WIDTH;
        int leftCol_x_end = leftCol_x + TILE_WIDTH;

        if (leftCol_x_end == x)
            insideLeft = true;
    }

    // right col
    int rightCol = playerC + 1;
    if (validCol(rightCol))
    {
        int rightCol_x = rightCol * TILE_WIDTH;

        if (rightCol_x == x_end)
            insideRight = true;
    }

    // top left
    int tlRow = playerR - 1;
    int tlCol = playerC - 1;

    if (!insideLeft && validCol(tlCol))
    {
        int tlCol_x = tlCol * TILE_WIDTH;
        int tlCol_x_end = tlCol_x + TILE_WIDTH;

        if (tlCol_x_end == x)
            insideLeft = true;
    }

    if (!insideTop && validRow(tlRow))
    {
        int tlRow_y = tlRow * TILE_HEIGHT;
        int tlRow_y_end = tlRow_y + TILE_HEIGHT;

        if (tlRow_y_end == y)
            insideTop = true;
    }

    // top right
    int trRow = playerR - 1;
    int trCol = playerC + 1;

    if (!insideRight && validCol(trCol))
    {
        int trCol_x = trCol * TILE_WIDTH;

        if (trCol_x == x_end)
            insideRight = true;
    }

    if (!insideTop && validRow(trRow))
    {
        int trRow_y = trRow * TILE_HEIGHT;
        int trRow_y_end = trRow_y + TILE_HEIGHT;

        if (trRow_y_end == y)
            insideTop = true;
    }

    // bot left
    int blRow = playerR + 1;
    int blCol = playerC - 1;

    if (!insideLeft && validCol(blCol))
    {
        int blCol_x = blCol * TILE_WIDTH;
        int blCol_x_end = blCol_x + TILE_WIDTH;

        if (blCol_x_end == x)
            insideLeft = true;
    }

    if (!insideDown && validRow(blRow))
    {
        int blRow_y = blRow * TILE_HEIGHT;

        if (blRow_y == y_end)
            insideDown = true;
    }

    // bot right
    int brRow = playerR + 1;
    int brCol = playerC + 1;

    if (!insideRight && validCol(brCol))
    {
        int brCol_x = brCol * TILE_WIDTH;

        if (brCol_x == x_end)
            insideRight = true;
    }

    if (!insideDown && validRow(brRow))
    {
        int brRow_y = brRow * TILE_HEIGHT;

        if (brRow_y == y_end)
            insideDown = true;
    }
}
