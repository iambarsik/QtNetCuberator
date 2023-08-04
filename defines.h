#ifndef DEFINES_H
#define DEFINES_H

#include <QtWidgets>

    // engine consts
#define TILE_SIZE           25.0
#define GLOBAL_G            0.3
#define ON_GAME_PERIOD      15
#define ON_ANIMATION_PERIOD 100

    // world size
#define SCREEN_TILE_WIDTH    40
#define SCREEN_TILE_HEIGHT   20

    // buttons for controll
#define BUTTON_COUNT    13

#define BUTTON_UP       0
#define BUTTON_RIGHT    1
#define BUTTON_DOWN     2
#define BUTTON_LEFT     3
#define BUTTON_ACT1     4
#define BUTTON_ACT2     5
#define BUTTON_ACT3     6
#define BUTTON_ACT4     7
#define BUTTON_ACT5     8
#define BUTTON_ACT6     9
#define BUTTON_ACT7     10
#define BUTTON_ACT8     11
#define BUTTON_MENU     12

    // sensor touch buttons
static const int TouchButtonGeometry[BUTTON_COUNT][5] = {

    /* Description    */ /*  X,   Y, W, H, Key   */ // geometry multyplies TILE_SIZE
    /* BUTTON_UP      */ {   4,  12, 4, 4, Qt::Key_Up},
    /* BUTTON_RIGHT   */ {   8,  14, 4, 4, Qt::Key_Right},
    /* BUTTON_DOWN    */ {   4,  16, 4, 4, Qt::Key_Down},
    /* BUTTON_LEFT    */ {   0,  14, 4, 4, Qt::Key_Left},
    /* BUTTON_ACT1    */ {  31,  16, 4, 4, Qt::Key_J},
    /* BUTTON_ACT2    */ {  35,  16, 4, 4, Qt::Key_K},
    /* BUTTON_ACT3    */ {  31,  12, 4, 4, Qt::Key_U},
    /* BUTTON_ACT4    */ {  35,  10, 4, 4, Qt::Key_I},
    /* BUTTON_ACT5    */ {   0,   0, 4, 4, Qt::Key_Q},
    /* BUTTON_ACT6    */ {   4,   0, 4, 4, Qt::Key_E},
    /* BUTTON_ACT7    */ {  27,   8, 4, 4, Qt::Key_H},
    /* BUTTON_ACT8    */ {  10,   0, 4, 4, Qt::Key_Space},
    /* BUTTON_MENU    */ {  35,   0, 4, 4, Qt::Key_Escape}
};

#endif // DEFINES_H
