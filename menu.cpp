void menu(int wys_okna, int szer_okna, int wys_fig, int szer_fig){
    ALLEGRO_DISPLAY *okno = NULL;
    ALLEGRO_BITMAP *tlo = NULL;
    ALLEGRO_BITMAP *kafelek1 = NULL;
    ALLEGRO_BITMAP *kafelek2 = NULL;
    ALLEGRO_BITMAP *kafelek3 = NULL;
    ALLEGRO_BITMAP *kafelek4 = NULL;

    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int x3 = 0;
    int y3 = 0;
    int x4 = 0;
    int y4 = 0;
    int x,y;

    okno = al_create_display(szer_okna, wys_okna);
    queue = al_create_event_queue();
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(okno));
    al_register_event_source(queue, al_get_mouse_event_source());

    al_init_image_addon();
    tlo = al_load_bitmap();
    kafelek1 = al_load_bitmap();
    kafelek2 = al_load_bitmap();
    kafelek3 = al_load_bitmap();
    kafelek4 = al_load_bitmap();

    bool running = true;
    int n;
    while (running) {
        al_clear_to_color(al_map_rgb(240, 255, 240));
        al_draw_bitmap(tlo, x0, y0, 0);
        al_draw_bitmap(kafelek1, x1, y1, 0);
        al_draw_bitmap(kafelek2, x2, y2, 0);
        al_draw_bitmap(kafelek1, x3, y3, 0);
        al_draw_bitmap(kafelek1, x3, y3, 0);
        al_flip_display();


        ALLEGRO_EVENT event;
        if (!al_is_event_queue_empty(queue))
        {
            al_wait_for_event(queue, &event);
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)  running=false;
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            {
                x = event.mouse.x;
                y = event.mouse.y;
                al_set_mouse_xy(okno, x, y);
                if(x1<=x&&x<=x1+szer_fig&&y1<=y&&y<=y1+wys_fig){
                    n = 1;
                    break;
                }
                else if(x2<=x&&x<=x2+szer_fig&&y2<=y&&y<=y2+wys_fig){
                    n = 2;
                    break;
                }
                else if(x3<=x&&x<=x3+szer_fig&&y3<=y&&y<=y3+wys_fig){
                    n = 3;
                    break;
                }
                else if(x4<=x&&x<=x4+szer_fig&&y4<=y&&y<=y4+wys_fig){
                    n = 4;
                    break;
                }
            }
        }
    }

    if(n==1)  ;//wywolujemy jakas funkcje
    else if(n==2)  ;//wywolujemy jakas funkcje
    else if(n==3)  ;//wywolujemy jakas funkcje
    else if(n==4)  ;//wywolujemy jakas funkcje
}