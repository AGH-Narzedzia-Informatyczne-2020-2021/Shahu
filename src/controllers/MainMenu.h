#ifndef SHAHU_MAINMENU_H
#define SHAHU_MAINMENU_H

#include "Controller.h"

namespace shahu {
    class MainMenu : shahu::Controller {
    public:
        bool update();
        bool render();
        static void wczytaj();
    };
}


#endif //SHAHU_MAINMENU_H
