#ifndef SHAHU_MAINMENU_H
#define SHAHU_MAINMENU_H

#include "View.h"
#include <allegro5/bitmap.h>

namespace shahu{
    class MainMenu : shahu::View
    {
    public:
        bool render();
        bool load();
    private:
        ALLEGRO_BITMAP *tlo;
        ALLEGRO_BITMAP *title;
        ALLEGRO_BITMAP *button1;
        ALLEGRO_BITMAP *button2;
        ALLEGRO_BITMAP *button3;
    };
}
//komentarz
//komentarz

#endif //SHAHU_MAINMENU_H
