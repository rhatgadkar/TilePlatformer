#include "player.h"
#include "game.h"
#include "globals.h"
#include "gameobject.h"

void Player::doSomething()
{
    Game* game = getGame();
    int x = getX();
    int y = getY();

    bool insideLeft = false, insideRight = false, insideTop = false, insideDown = false, insideEnemy = false;
    checkBounds(game, x, y, insideLeft, insideRight, insideTop, insideDown, insideEnemy);

    if (insideEnemy)
    {
        game->reset();
        return;
    }

    movePlayer(game, insideLeft, insideRight, insideTop, insideDown);
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

void Player::checkBounds(Game* game, int x, int y, bool& insideLeft, bool& insideRight, bool& insideTop, bool& insideDown, bool& insideEnemy)
{
    game->boundingBox(m_x, m_y, insideLeft, insideRight, insideTop, insideDown, insideEnemy);
}
