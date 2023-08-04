#include "widget.h"

void Widget::OnGame()   {

    OnTouch();

    for(int i = 0; i < BUTTON_COUNT; i++)   {
        game->setButton(i,myKey[i]);
    }
    game->update();




    repaint();
}
