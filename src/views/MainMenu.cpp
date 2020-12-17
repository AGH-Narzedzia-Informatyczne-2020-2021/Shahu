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

bool MainMenu::click(float a, float b){
    if((x[0]<=a&&a<=x[0]+300)&&(y[0]<=b&&b<=y[0]+150)){
        button1 = al_load_bitmap("../assets/menu/Przycisk_graj_wcisniety.png");
        return false;
    }
    else if((x[1]<=a&&a<=x[1]+130)&&(y[1]<=b&&b<=y[1]+145)){
        button2 = al_load_bitmap("../assets/menu/ustawienia_wcisniete.png");
        return false;
    }
    else if(x[2]<=a&&a<=x[2]+300&&y[2]<=b&&b<=y[2]+150){
        button3 = al_load_bitmap("../assets/menu/wyjdz_wcisniete.png");
        al_draw_bitmap(button3,x[2],y[2],0);
        al_flip_display();
        _sleep(3);
        return true;
    }
    else return false;
    

}

