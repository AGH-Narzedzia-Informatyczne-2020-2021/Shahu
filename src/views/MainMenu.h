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
        void click(int a, int b, ALLEGRO_BITMAP **b1, ALLEGRO_BITMAP **b2, ALLEGRO_BITMAP **b3);
    //private:
        ALLEGRO_BITMAP *tlo;
        ALLEGRO_BITMAP *title;
        ALLEGRO_BITMAP *button1;
        ALLEGRO_BITMAP *button2;
        ALLEGRO_BITMAP *button3;
        float x[3]={490,0,490};
        float y[3]={300,575,500};
    };
}


#endif //SHAHU_MAINMENU_H
