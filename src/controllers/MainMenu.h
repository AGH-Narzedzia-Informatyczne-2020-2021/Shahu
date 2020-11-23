#ifndef SHAHU_MAINMENU_H
#define SHAHU_MAINMENU_H

#include "Controller.h"

namespace shahu {
    class MainMenu : shahu::Controller {
    public:
        virtual double update();
        virtual double render();
    };
}


#endif //SHAHU_MAINMENU_H
