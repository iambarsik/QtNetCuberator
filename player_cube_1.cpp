#include "player_cube_1.h"
#include "game_enums.h"

player_cube_1::player_cube_1(game_player_type type, double g, qint64 id)
    : game_player(type,g,id)
{

}

void player_cube_1::actionLeft()    {
    if(vx() > -par_max_speed)
        set_vx(vx() - g()*2);
}

void player_cube_1::actionRight()   {
    if(vx() < par_max_speed)
        set_vx(vx() + g()*2);
}

void player_cube_1::action1()
{

}

void player_cube_1::action2()   {
    if(isFly() == false)    {
        set_vy(par_jump_force);
    }
}

void player_cube_1::update()
{


}
