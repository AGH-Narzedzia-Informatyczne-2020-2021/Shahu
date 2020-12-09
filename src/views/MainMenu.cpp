#include "MainMenu.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

using namespace shahu;

bool MainMenu::load()
{
    tlo = al_load_bitmap("../assets/menu/Tlo.png");
    title = al_load_bitmap("../assets/menu/Title.png");
    button1 = al_load_bitmap("../assets/menu/Przycisk_graj.png");
    button2 = al_load_bitmap("../assets/menu/ustawienia.png");
    button3 = al_load_bitmap("../assets/menu/wyjdz.png");
    //Tak jak Pan Jezus powiedzial

    return true;
}

bool MainMenu::render() {
    al_draw_bitmap(tlo,0,0,0);
    al_draw_bitmap(title,140,0,0);
    al_draw_bitmap(button1,x[0],y[0],0);
    al_draw_bitmap(button2,x[1],y[1],0);
    al_draw_bitmap(button3,x[2],y[2],0);

    return true;
}

