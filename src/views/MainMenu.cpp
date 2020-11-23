#include "MainMenu.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

using namespace shahu;

bool MainMenu::load()
{
    tlo = al_load_bitmap("./assets/menu/tlo.png");
    title = al_load_bitmap("./assets/menu/Title.png");
    button1 = al_load_bitmap("./assets/menu/Przycisk_graj.png");
    button2 = al_load_bitmap("./assets/menu/ustawienia.png");
    button3 = al_load_bitmap("./assets/menu/wyjdz.png");
    //Tak jak Pan Jezus powiedzial

    return true;
}

