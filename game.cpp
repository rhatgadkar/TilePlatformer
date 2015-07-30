#include "game.h"
#include "allegro5/allegro_primitives.h"
#include "wall.h"
#include "stationaryenemy.h"
#include <list>
#include "gameobject.h"
#include "movingobject.h"
#include "stationaryobject.h"
#include "globals.h"
#include "player.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cctype>
#include <iterator>
#include <iostream>
using namespace std;

Game::Game(char* mapFile)
{
    if (mapFile == NULL)
    {
        fprintf(stderr, "Map file cannot be NULL!\n");
        exit(0);
    }

    m_mapFile = mapFile;
    m_reset = false;
    m_stationaryCount = 0;
    m_movingCount = 0;
    m_levelWidth = MAX_LEVEL_WIDTH;
    m_numCols = MAX_NUM_COLS;
    m_camX = 0;
    m_player = NULL;

    m_key = new bool[4];
    m_key[0] = false; m_key[1] = false; m_key[2] = false; m_key[3] = false;

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "pirulen.ttf");
    m_font = al_load_ttf_font(al_path_cstr(path, '/'), 28, 0);
    if (!m_font)
    {
        fprintf(stderr, "Could not load 'pirulen.ttf'.\n");
        exit(0);
    }
    al_destroy_path(path);

    // initialize map
    m_map = new GameObject**[NUM_ROWS];
    for (int r = 0; r < NUM_ROWS; r++)
    {
        m_map[r] = new GameObject*[m_numCols];
        for (int c = 0; c < m_numCols; c++)
            m_map[r][c] = NULL;
    }

    m_levelWidth = parseMapFile() + TILE_WIDTH; // the last wall or stenemy determines level width
    m_numCols = m_levelWidth / TILE_WIDTH;

    for (int r = 0; r < NUM_ROWS; r++)
    {
        for (int c = 0; c < m_numCols; c++)
        if (m_map[r][c] != NULL)
            cout << m_map[r][c]->getTile();
        else
            cout << 0;
        cout << endl;
    }
}

Game::~Game()
{
    al_destroy_font(m_font);

    delete m_key;

    delete m_player;
    m_player = NULL;

    list<StationaryObject*>::iterator it = m_stationaryobjects.begin();
    while (it != m_stationaryobjects.end())
    {
        StationaryObject* toDelete = *it;
        it = m_stationaryobjects.erase(it);
        delete toDelete;
    }
    m_stationaryobjects.clear();

    list<MovingObject*>::iterator it2 = m_movingobjects.begin();
    while (it2 != m_movingobjects.end())
    {
        MovingObject* toDelete = *it2;
        it2 = m_movingobjects.erase(it2);
        delete toDelete;
    }
    m_movingobjects.clear();

    for (int r = 0; r < NUM_ROWS; r++)
        delete [] m_map[r];
}

bool Game::removeStationaryObject(StationaryObject* toDelete)
{
    int pos = toDelete->getPos();

    if (m_stationaryCount - pos < pos)
    {
        list<StationaryObject*>::reverse_iterator it = m_stationaryobjects.rbegin();
        StationaryObject* temp = *it;
        for (; it != m_stationaryobjects.rend(); it++)
        {
            temp = *it;
            if (pos == temp->getPos())
                break;
        }

        if (pos != temp->getPos()) // object to be deleted not found
            return false;

        for (list<StationaryObject*>::reverse_iterator it2 = m_stationaryobjects.rbegin(); it2 != it; it2++)
        {
            StationaryObject* changePos = *it2;
            changePos->setPos(changePos->getPos() - 1);
        }

        m_stationaryobjects.erase(--it.base());
        delete toDelete;
    }
    else
    {
        list<StationaryObject*>::iterator it = m_stationaryobjects.begin();
        StationaryObject* temp = *it;
        for (; it != m_stationaryobjects.end(); it++)
        {
            temp = *it;
            if (pos == temp->getPos())
                break;
        }

        if (pos != temp->getPos()) // object to be deleted not found
            return false;

        for (list<StationaryObject*>::iterator it2 = m_stationaryobjects.begin(); it2 != it; it2++)
        {
            StationaryObject* changePos = *it2;
            changePos->setPos(changePos->getPos() - 1);
        }

        m_stationaryobjects.erase(it);
        delete toDelete;
    }

    m_stationaryCount--;
    return true;
}

bool Game::removeMovingObject(MovingObject* toDelete)
{
    int pos = toDelete->getPos();

    if (m_movingCount - pos < pos)
    {
        list<MovingObject*>::reverse_iterator it = m_movingobjects.rbegin();
        MovingObject* temp = *it;
        for (; it != m_movingobjects.rend(); it++)
        {
            temp = *it;
            if (pos == temp->getPos())
                break;
        }

        if (pos != temp->getPos()) // object to be deleted not found
            return false;

        for (list<MovingObject*>::reverse_iterator it2 = m_movingobjects.rbegin(); it2 != it; it2++)
        {
            MovingObject* changePos = *it2;
            changePos->setPos(changePos->getPos() - 1);
        }

        m_movingobjects.erase(--it.base());
        delete toDelete;
    }
    else
    {
        list<MovingObject*>::iterator it = m_movingobjects.begin();
        MovingObject* temp = *it;
        for (; it != m_movingobjects.end(); it++)
        {
            temp = *it;
            if (pos == temp->getPos())
                break;
        }

        if (pos != temp->getPos()) // object to be deleted not found
            return false;

        for (list<MovingObject*>::iterator it2 = m_movingobjects.begin(); it2 != it; it2++)
        {
            MovingObject* changePos = *it2;
            changePos->setPos(changePos->getPos() - 1);
        }

        m_movingobjects.erase(it);
        delete toDelete;
    }

    m_movingCount--;
    return true;
}

void Game::reset()
{
    m_reset = true;

    delete m_player;
    m_player = NULL;

    m_camX = 0;

    list<StationaryObject*>::iterator it = m_stationaryobjects.begin();
    while (it != m_stationaryobjects.end())
    {
        StationaryObject* toDelete = *it;
        it = m_stationaryobjects.erase(it);
        delete toDelete;
    }
    m_stationaryobjects.clear();

    list<MovingObject*>::iterator it2 = m_movingobjects.begin();
    while (it2 != m_movingobjects.end())
    {
        MovingObject* toDelete = *it2;
        it2 = m_movingobjects.erase(it2);
        delete toDelete;
    }
    m_movingobjects.clear();

    m_stationaryCount = 0;
    m_movingCount = 0;

    for (int r = 0; r < NUM_ROWS; r++)
    {
        m_map[r] = new GameObject*[m_numCols];
        for (int c = 0; c < m_numCols; c++)
            m_map[r][c] = NULL;
    }

    parseMapFile();
}

void Game::getInput()
{
    m_player->doSomething();

    for (list<MovingObject*>::iterator it = m_movingobjects.begin(); it != m_movingobjects.end(); it++)
    {
        MovingObject* current = *it;
        current->doSomething();
        if (m_reset)
            return;
    }
}

void Game::draw()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // draw player
    float x1 = m_player->getX();
    float x2 = x1 + (m_player->getWidth() * PLAYER_WIDTH);
    float y1 = m_player->getY();
    float y2 = y1 + (m_player->getHeight() * PLAYER_HEIGHT);
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 255, 0));

    float playerX = x1;

    // move camera
    if (playerX <= SCREEN_WIDTH / 2)
        //camX = 0;
        ;
    else if (playerX >= m_levelWidth - SCREEN_WIDTH / 2)
        m_camX = m_levelWidth - SCREEN_WIDTH;
        //;
    else
    {
        m_camX = playerX - SCREEN_WIDTH / 2;
    }
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, -m_camX, 0);
    al_use_transform(&trans);

    for (list<StationaryObject*>::iterator it = m_stationaryobjects.begin(); it != m_stationaryobjects.end(); it++)
    {
        StationaryObject* current = *it;
        int width = current->getWidth() * TILE_WIDTH;
        int height = current->getHeight() * TILE_HEIGHT;
        float x1 = current->getX();
        float x2 = x1 + width;
        float y1 = current->getY();
        float y2 = y1 + height;

        if (x2 >= m_camX || x1 >= m_camX)
        {
            char tile = current->getTile();
            switch (tile)
            {
            case 'w':
                al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 0, 0));
                break;
            case 's':
                al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 255, 0));
                break;
            }
        }
    }

    al_flip_display();
}

void Game::drawGameOverScreen() const
{
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_translate_transform(&trans, 0, 0);
    al_use_transform(&trans);

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(m_font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 3.5, SCREEN_HEIGHT / 10, ALLEGRO_ALIGN_LEFT, "Game Over");
    al_draw_text(m_font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press Enter to retry");

    al_flip_display();
}

void Game::drawStartScreen() const
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(m_font, al_map_rgb(255, 255, 255), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press Enter to start");

    al_flip_display();
}

void Game::boundingBox(int pX, int pY, bool& insideLeft, bool& insideRight, bool& insideTop, bool& insideDown, bool& insideEnemy)
{
    int px_start = pX;
    int px_end = pX + PLAYER_WIDTH;
    int py_start = pY;
    int py_end = pY + PLAYER_HEIGHT;

    // collision for stationary objects. more efficient than using loops. There is a wall jumping issue with this.
    int pRow = py_start / TILE_HEIGHT;
    int pCol = px_start / TILE_WIDTH;

    int tr_row = pRow - 1, tr_col = pCol + 1;
    int t_row = pRow - 1, t_col = pCol;
    int tl_row = pRow - 1, tl_col = pCol - 1;
    int l_row = pRow, l_col = pCol - 1;
    int bl_row = pRow + 1, bl_col = pCol - 1;
    int b_row = pRow + 1, b_col = pCol;
    int br_row = pRow + 1, br_col = pCol + 1;
    int r_row = pRow, r_col = pCol + 1;

    int tileRows[] = { tr_row, t_row, tl_row, l_row, bl_row, b_row, br_row, r_row, pRow };
    int tileCols[] = { tr_col, t_col, tl_col, l_col, bl_col, b_col, br_col, r_col, pCol };

    int lastSelectedX = -1;
    int lastSelectedY = -1;

    for (int k = 0; k < 9; k++)
    {
        if (validRow(tileRows[k]) && validCol(tileCols[k]))
        {
            GameObject* go = m_map[ tileRows[k] ][ tileCols[k] ];
            if (go == NULL)
                continue;

            char id = go->getTile();
            if (id != 's' && id != 'w')
                continue;

            int gx_start = go->getX();
            int g_width = go->getWidth();
            int g_height = go->getHeight();
            int gx_end = gx_start + TILE_WIDTH * g_width;
            int gy_start = go->getY();
            int gy_end = gy_start + TILE_HEIGHT * g_height;

            if (gx_start == lastSelectedX && gy_start == lastSelectedY)
                continue;
            lastSelectedX = gx_start;
            lastSelectedY = gy_start;

            if ( (px_end >= gx_start) && (px_start <= gx_end ) && (py_end >= gy_start) && (py_start <= gy_end) ) // collision occured
            {
                if (id == 's')
                {
                    insideEnemy = true;
                    return;
                }

                if (py_start == gy_end/* && px_start != gx_end && px_end != gx_start*/)
                {
                    int topRow = tileRows[k] - 1;
                    int botRow = tileRows[k] + 1;

                    if (validRow(topRow) && validRow(botRow))
                    {
                        GameObject* go_topRow = m_map[topRow][tileCols[k]];
                        GameObject* go_botRow = m_map[botRow][tileCols[k]];

                        if (go_topRow != NULL && go_botRow != NULL && go_topRow->getTile() == 'w' && go_botRow->getTile() == 'w')
                            ;
                        else
                            insideTop = true;
                    }
                    else
                        insideTop = true;
                }
                else if (py_end == gy_start/* && px_start != gx_end && px_end != gx_start*/)
                {
                    int topRow = tileRows[k] - 1;
                    int botRow = tileRows[k] + 1;

                    if (validRow(topRow) && validRow(botRow))
                    {
                        GameObject* go_topRow = m_map[topRow][tileCols[k]];
                        GameObject* go_botRow = m_map[botRow][tileCols[k]];

                        if (go_topRow != NULL && go_botRow != NULL && go_topRow->getTile() == 'w' && go_botRow->getTile() == 'w')
                            ;
                        else
                            insideDown = true;
                    }
                    else
                        insideDown = true;
                }
                else if (px_end == gx_start && py_start != gy_end && py_end != gy_start)
                    insideRight = true;
                else if (px_start == gx_end && py_start != gy_end && py_end != gy_start)
                    insideLeft = true;
            }
        }
    }

//    for (list<StationaryObject*>::iterator it = m_stationaryobjects.begin(); it != m_stationaryobjects.end(); it++)
//    {
//        StationaryObject* current = *it;
//        char id = current->getTile();
//        int gx_start = current->getX();
//        int g_width = current->getWidth();
//        int g_height = current->getHeight();
//        int gx_end = gx_start + TILE_WIDTH * g_width;
//        int gy_start = current->getY();
//        int gy_end = gy_start + TILE_HEIGHT * g_height;
//
//        if (id == 'p')
//            continue;
//
//        if ( (px_end >= gx_start) && (px_start <= gx_end ) && (py_end >= gy_start) && (py_start <= gy_end) ) // collision occured
//        {
//            if (id == 's')
//            {
//                insideEnemy = true;
//                return;
//            }
//
//            if (px_end == gx_start)
//                insideRight = true;
//            else if (px_start == gx_end)
//                insideLeft = true;
//            else if (py_start == gy_end)
//                insideTop = true;
//            else if (py_end == gy_start)
//                insideDown = true;
//        }
//    }
}

int Game::parseMapFile() // returns value for m_levelWidth
{
    ifstream mapFile;
    mapFile.open(m_mapFile);
    if (!mapFile.is_open())
    {
        fprintf(stderr, "Could not open map file.\n");
        exit(0);
    }

    int highest_x = 0;

    int lineNum = 1;
    string line;
    int commaCount = 0;
    while (getline(mapFile, line))
    {
        char tile;
        int row, col, width, height;
        int num = -1;

        for (size_t k = 0; k < line.length(); k++)
        {
            if (line[k] == ',')
            {
                commaCount++;
                //num = -1;
            }
            else if (isdigit(line[k]))
            {
                if (num != -1)
                {
                    num = (num * 10) + (line[k] - '0');
                }
                else
                    num = line[k] - '0';

                continue;
            }

            if (k == 0) // tile char
            {
                tile = line[k];
                if (tile != 'w' && tile != 'p' && tile != 's' && tile != 'm')
                {
                    fprintf(stderr, "%s %c %s %d \n", "Map file error:\nInvalid tile char: '", tile, "' at line: ", lineNum);
                    exit(0);
                }
            }
            else
            {
                switch (commaCount)
                {
                case 1:
                    break;
                case 2: // row
                    row = num;
                    num = -1;
                    if (row < 0 || row >= NUM_ROWS)
                    {
                        fprintf(stderr, "%s %d %s %d \n", "Map file error:\nInvalid row: '", row, "' at line: ", lineNum);
                        exit(0);
                    }
                    break;
                case 3: // col
                    col = num;
                    num = -1;
                    if (col < 0 || col >= MAX_NUM_COLS)
                    {
                        fprintf(stderr, "%s %d %s %d \n", "Map file error:\nInvalid col: '", col, "' at line: ", lineNum);
                        exit(0);
                    }
                    break;
                case 4: // width
                    width = num;
                    num = -1;
                    if (width < 1 || width > MAX_NUM_COLS)
                    {
                        fprintf(stderr, "%s %d %s %d \n", "Map file error:\nInvalid width: '", width, "' at line: ", lineNum);
                        exit(0);
                    }
                    break;
                case 5: // height
                    height = num;
                    num = -1;
                    if (height < 1 || height > NUM_ROWS)
                    {
                        fprintf(stderr, "%s %d %s %d \n", "Map file error:\nInvalid height: '", height, "' at line: ", lineNum);
                        exit(0);
                    }
                    break;
                default:
                    fprintf(stderr, "%s %d \n", "Map file error:\nExtra parameters at line: ", lineNum);
                    exit(0);
                }
            }
        }

        // create the gameobject
        switch (tile)
        {
        case 'w':
            addNewStationaryObject(new Wall(row, col, width, height, this, lineNum));
            break;
        case 'p':
        {
            if (m_player != NULL)
            {
                fprintf(stderr, "%s%d\n", "Map file error:\nCan only have one player. Extra player at line: ", lineNum);
                exit(0);
            }

            m_player = new Player(row, col, width, height, this, lineNum);
            break;
        }
        case 's':
            addNewStationaryObject(new StationaryEnemy(row, col, width, height, this, lineNum));
            break;
        case 'm':
            break;
        }

        if (tile == 'w' || tile == 's')
        {
            StationaryObject* last = m_stationaryobjects.back();
            int last_x = last->getX();
            if (last_x > highest_x)
                highest_x = last_x;
        }

        lineNum++;
        commaCount = 0;
    }

    if (m_player == NULL)
    {
        fprintf(stderr, "%s\n", "Map file error:\nA player must be added.");
        exit(0);
    }

    // remove chars in map for player
    int player_row = m_player->getR();
    int player_col = m_player->getC();
    for (int r = player_row; r < (player_row + m_player->getHeight()); r++)
    {
        for (int c = player_col; c < (player_col + m_player->getWidth()); c++)
            setMap(r, c, NULL);
    }

    // remove chars in map for movingobjects
    // not yet implemented because there are not movingobjects yet.


    mapFile.close();

    return highest_x;
}
