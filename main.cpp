#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <iostream>

#include "shahu.h"

int main()
{
    shahu::HelloWorld::printHello();
//mmddmdm
    std::cout << std::endl << "Hey! Nice to see you in Shahu Game!" << std::endl << std::endl;
    std::cout << "  Makers:" << std::endl << std::endl;
    std::cout << "      Dawid Slomnicki," << std::endl;
    std::cout << "      Jakub Hulek," << std::endl;
    std::cout << "      Marcin Kosiba," << std::endl;
    std::cout << "      Michal Palucki," << std::endl;
    std::cout << "      Patryk Gegotek," << std::endl;
    std::cout << "      Piotr Pyrczak." << std::endl;
    std::cout << "                                               All rights reserved." << std::endl;

    //---------------INITIALIZATION------------
    if(!al_init())
    {
        std::cout << "couldn't initialize allegro" << std::endl;
        return 1;
    }

    if(!al_install_keyboard())
    {
        std::cout << "couldn't initialize keyboard\n" << std::endl;
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    if(!timer)
    {
        std::cout << "couldn't initialize timer\n" << std::endl;
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if(!queue)
    {
        std::cout << "couldn't initialize queue\n" << std::endl;
        return 1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(1280, 720);
    if(!disp)
    {
        std::cout << "couldn't initialize display\n" << std::endl;
        return 1;
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if(!font)
    {
        std::cout << "couldn't initialize font\n" << std::endl;
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    //-------------------------




    bool exit = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    //-------------MAIN LOOP--------------
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                exit = true;
                break;
        }

        if(exit)
            break;

        //----------Here we go---------------
        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redraw = false;
        }
    }

    //----------EXIT----------------
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
