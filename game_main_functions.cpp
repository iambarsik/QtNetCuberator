#include "game_main_class.h"

void game_main_class::setup()   {

    level.setWorld((int *)dev_world,20,40);
    level.setMainImageFromMap();

    //addPlayer(game_player_type::dev, QPoint(tile_size*15,tile_size*2), false);
    //addPlayer(game_player_type::cube1, QPoint(tile_size*19,tile_size*2), true);
}

void game_main_class::update()  {

    for(game_player * player: players)   {

        /*
        if(player->id() == buttons_id)   {
            for(int i = 0; i < BUTTON_COUNT; i++)   {
                player->setButton(i,buttons[i]);
            }
        }
        */

        if(bServerNode) {

            player->update_y();
            if(level.getYCollide(player->body())) {
                player->collideY();
            }
            player->update_x();
            if(level.getXCollide(player->body())) {
                player->collideX();
            }
            player->live();

            if(server->getClientCount() > 0)    {
                server->addCommand({game_command_list::set_player_x,game_network_param(player->id()),game_network_param(player->x()),timeNow()});
                server->addCommand({game_command_list::set_player_y,game_network_param(player->id()),game_network_param(player->y()),timeNow()});
            }

        } else {

            player->update_y();
            if(level.getYCollide(player->body())) {
                player->collideY();
            }
            player->update_x();
            if(level.getXCollide(player->body())) {
                player->collideX();
            }
            player->live();




            if(client->isConnected())   {
                if(buttons_id != -1)    {
                    game_network_command c1;
                    c1.code = game_command_list::set_buttons_1;
                    c1.par1 = game_network_param(buttons_id);
                    c1.par2.data.append((char)buttons[BUTTON_UP]);
                    c1.par2.data.append((char)buttons[BUTTON_RIGHT]);
                    c1.par2.data.append((char)buttons[BUTTON_DOWN]);
                    c1.par2.data.append((char)buttons[BUTTON_LEFT]);
                    c1.par2.data.append((char)buttons[BUTTON_ACT1]);
                    c1.par2.data.append((char)buttons[BUTTON_ACT2]);
                    c1.par2.data.append((char)buttons[BUTTON_ACT3]);
                    c1.par2.data.append((char)buttons[BUTTON_ACT4]);
                    c1.time = timeNow();

                    game_network_command c2;
                    c2.code = game_command_list::set_buttons_2;
                    c2.par1 = game_network_param(buttons_id);
                    c2.par2.data.append((char)buttons[BUTTON_ACT5]);
                    c2.par2.data.append((char)buttons[BUTTON_ACT6]);
                    c2.par2.data.append((char)buttons[BUTTON_ACT7]);
                    c2.par2.data.append((char)buttons[BUTTON_ACT8]);
                    c2.par2.data.append((char)buttons[BUTTON_MENU]);
                    c2.par2.data.append((char)0x00);
                    c2.par2.data.append((char)0x00);
                    c2.par2.data.append((char)0x00);
                    c2.time = timeNow();

                    client->addCommand(c1);
                    client->addCommand(c2);
                }
            }
        }
    }
}

void game_main_class::animate() {
    /*
    if(bServerNode) {
        for(game_player * player : players) {
            if(server->getClientCount() > 0)    {
                server->addCommand({game_command_list::set_player_x,game_network_param(player->id()),game_network_param(player->x()),timeNow()});
                server->addCommand({game_command_list::set_player_y,game_network_param(player->id()),game_network_param(player->y()),timeNow()});
            }
        }
    }
    */
}
