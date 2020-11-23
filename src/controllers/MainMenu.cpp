#include "MainMenu.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

using namespace shahu;

void MainMenu::menu()
{
    ALLEGRO_BITMAP *tlo = al_load_bitmap("./assets/menu/tlo.png");
    ALLEGRO_BITMAP *title = al_load_bitmap("./assets/menu/Title.png");
    ALLEGRO_BITMAP *button1 = al_load_bitmap("./assets/menu/Przycisk_graj.png");
    ALLEGRO_BITMAP *button2 = al_load_bitmap("./assets/menu/ustawienia.png");
    ALLEGRO_BITMAP *button3 = al_load_bitmap("./assets/menu/wyjdz.png");

    al_draw_bitmap (tlo,0,0,0);
    al_draw_bitmap (title,0,0,0);
    al_draw_bitmap (button1,0,0,0);
    al_draw_bitmap (button2,0,0,0);
    al_draw_bitmap (button3,0,0,0);


}


