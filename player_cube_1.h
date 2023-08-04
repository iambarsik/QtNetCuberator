#ifndef PLAYER_CUBE_1_H
#define PLAYER_CUBE_1_H

#include "game_additional_classes.h"

class player_cube_1 : public game_player
{
public:
    player_cube_1(game_player_type type, double g, qint64 id);

    void actionLeft() override;
    void actionRight() override;
    void action1() override;
    void action2() override;
    void update() override;



};

#endif // PLAYER_CUBE_1_H
