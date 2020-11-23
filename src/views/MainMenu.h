#ifndef SHAHU_MAINMENU_H
#define SHAHU_MAINMENU_H

#include "View.h"

namespace shahu{
    class MainMenu : shahu::View
    {
    public:
        bool render();
        bool load();
    };
}

#endif //SHAHU_MAINMENU_H
