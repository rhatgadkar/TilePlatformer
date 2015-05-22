#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "game.h"
#include "globals.h"

int main()
{
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_EVENT_QUEUE* event_queue = NULL;
    ALLEGRO_TIMER* timer = NULL;
    bool doexit = false;
    bool redraw = true;
    bool gameStarted = false;
    bool gameOver = false;
    bool pause = false;

    if (!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    al_init_font_addon();
    al_init_ttf_addon();

    timer = al_create_timer(1.0 / FPS);
    if (!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display)
    {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "failed to initialize keyboard!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if (!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();

    al_start_timer(timer);

    Game game;

    while (!doexit)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if (!gameStarted || gameOver || pause)
                redraw = false;
            else
            {
                redraw = true;
                game.getInput();

                if (game.isReset())
                {
                    gameOver = true;
                    game.setReset(false);
                    redraw = false;
                }
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                game.setKey(KEY_UP, true);
                break;
            case ALLEGRO_KEY_DOWN:
                game.setKey(KEY_DOWN, true);
                break;
            case ALLEGRO_KEY_LEFT:
                game.setKey(KEY_LEFT, true);
                break;
            case ALLEGRO_KEY_RIGHT:
                game.setKey(KEY_RIGHT, true);
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch (ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                game.setKey(KEY_UP, false);
                break;
            case ALLEGRO_KEY_DOWN:
                game.setKey(KEY_DOWN, false);
                break;
            case ALLEGRO_KEY_LEFT:
                game.setKey(KEY_LEFT, false);
                break;
            case ALLEGRO_KEY_RIGHT:
                game.setKey(KEY_RIGHT, false);
                break;
            case ALLEGRO_KEY_ESCAPE:
                doexit = true;
                break;
            case ALLEGRO_KEY_P:
                if (gameStarted && !gameOver && !pause)
                    pause = true;
                else if (gameStarted && !gameOver && pause)
                    pause = false;
                break;
            case ALLEGRO_KEY_ENTER:
                if (!gameStarted && !gameOver)
                    gameStarted = true;
                else if (gameStarted && gameOver)
                    gameOver = false;
                break;
            }
        }
        if (gameStarted && !gameOver && !pause && redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            game.draw();
        }
        else if (!gameStarted)
            game.drawStartScreen();
        else if (gameOver)
            game.drawGameOverScreen();
    }

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
