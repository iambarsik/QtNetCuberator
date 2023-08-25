#include "game_main_class.h"

game_main_class::game_main_class(int tile_size, double g)
    : level(tile_size)
{
    this->tile_size = tile_size;
    g_gravity = g;
    m_game_version = version;
    for(int i = 0; i < BUTTON_COUNT; i++)   {
        buttons[i] = false;
    }

    bServerNode = false;
    CurrentNode.name = QString("NetCubenator game v. %1").arg(m_game_version);
    CurrentNode.host = "127.0.0.1";
    CurrentNode.port = 2023;
    CurrentNode.frameType = 0;
    CurrentNode.mainNode = 1;
    CurrentNode.ID = 1;
    CurrentNode.isServer = bServerNode;

    QString SettingsName = qApp->applicationDirPath() + "/game_config.ini";

    if(!QFile::exists(SettingsName))    {
        QStringList ini;
        ini.append("[MAIN_SETTINGS]");
        ini.append("server=true");
        ini.append("host=127.0.0.1");
        ini.append("port=2023");
        ini.append("name=NetCubenator server");
        ini.append("frameType=0");
        ini.append("mainNode=1");
        ini.append("nodeID=1");
        ini.append("");

        QFile file(SettingsName);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        out.setCodec(QTextCodec::codecForName("UTF-8"));
        for(int i = 0; i < ini.size(); i++)   {
            out << ini[i] << "\n";
        }
        file.close();

        qDebug() << "system :: config created";

    } else {
        QSettings settings(SettingsName, QSettings::IniFormat);
        settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

        settings.beginGroup("MAIN_SETTINGS");
        CurrentNode.isServer  = (settings.value("server", false)).toBool();
        CurrentNode.host      = (settings.value("host", "127.0.0.1")).toString();
        CurrentNode.port      = (settings.value("port", 2023)).toInt();
        CurrentNode.name      = (settings.value("name", "NetCubenator client")).toString();
        CurrentNode.frameType = (settings.value("frameType", 0)).toInt();
        CurrentNode.mainNode  = (settings.value("mainNode", 1)).toInt();
        CurrentNode.ID        = (settings.value("nodeID", 1)).toInt();
        settings.endGroup();

        qDebug() << "system :: config loaded";
    }

    bServerNode = CurrentNode.isServer;
    CurrentNode.name = QString("%1 v. %2").arg(CurrentNode.name).arg(m_game_version);


    if(bServerNode) {
        server = new game_server(CurrentNode);
        connect(server,&game_server::signalClientsAreConnected,
                this,&game_main_class::setClientInformation);
        connect(server,&game_server::signalCommandFromClient,
                this,&game_main_class::slotReadCommand);
        connect(server,&game_server::signalNewClientConnected,
                this,&game_main_class::slotNewClientConnected);
        connect(server,&game_server::signalClientHasDisconnected,
                this,&game_main_class::slotClientHasConnected);
    } else {
        client = new game_client(CurrentNode);
        //connect(STR_client, SIGNAL(signalPackageFromServer(QByteArray)),
        //        core, SLOT(setCoreFromPackage(QByteArray)));

        connect(client,&game_client::signalCommandFromServer,
                this,&game_main_class::slotReadCommand);
    }
}

qint32 game_main_class::timeNow()   {
    QTime t = QTime::currentTime();
    return t.msecsSinceStartOfDay();
}


void game_main_class::render(QPainter *p)   {
    QPen pen = p->pen();
    QBrush brush = p->brush();

    if(level.isMainImageEnabled())
        level.renderMain(camera.pos(), p);

    for(auto player: players)    {
        player->draw(camera.pos(),p);


        if(player->id() == buttons_id)  {
            gt.drawText(10,25,GameTextSize::font_size_small,QString("x=%1").arg(player->vx()),p);
            gt.drawText(10,40,GameTextSize::font_size_small,QString("y=%1").arg(player->body().y()),p);
            gt.drawText(10,65,GameTextSize::font_size_small,QString("vx=%1").arg(player->vx()),p);
            gt.drawText(10,80,GameTextSize::font_size_small,QString("id=%1").arg(player->id()),p);
        }

    }

    if(level.isFrontImageEnabled())
        level.renderFront(camera.pos(), p);


    gt.drawText(10,10,GameTextSize::font_size_small,QString("players: %1 ").arg(game_network_player_count),p);

    p->setPen(pen);
    p->setBrush(brush);
}

void game_main_class::clear()   {
    level.clear();
    players.clear();
}

qint64 game_main_class::addPlayer(game_player_type type, QPoint pos, bool control, qint64 id)  {

    if(id != -1)    {
        for(auto p : players)   {
            if(p->id() == id)
                return -1;
        }
    }
    qint64 new_id;
    QPoint xy;
    if(bServerNode) {
        xy = pos;
        new_id = genID();
    } else {
        new_id = id;
        xy.setX(50);
        xy.setY(50);
    }

    switch(type)    {
        case game_player_type::dev:
        default:    {
            game_player * p = new game_player(type, g_gravity, new_id);
            p->setGeometry(xy.x(),xy.y(),tile_size*2, tile_size*2);
            if(control)
                setButtonsID(p->id());
            players.push_back(p);
            return p->id();
        } break;
        case game_player_type::cube1: {
            player_cube_1 * p = new player_cube_1(type, g_gravity, new_id);
            p->setGeometry(xy.x(),xy.y(),tile_size*2, tile_size*2);
            if(control)
                setButtonsID(p->id());
            players.push_back(p);
            return p->id();
        } break;
    }
    return -1;
}

void game_main_class::setClientInformation(int count)   {
    game_network_player_count = count;
}

void game_main_class::slotReadCommand(game_network_command command) {
        //qDebug() << "Recieved command from format";
        //qDebug() << "Command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toInt();

        if(bServerNode) {

            switch(command.code)    {
                case game_command_list::set_buttons_1: {
                    bool button_buff[BUTTON_COUNT];
                    bool status;
                    button_buff[BUTTON_UP]    = command.par2.data.mid(0,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_RIGHT] = command.par2.data.mid(1,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_DOWN]  = command.par2.data.mid(2,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_LEFT]  = command.par2.data.mid(3,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT1]  = command.par2.data.mid(4,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT2]  = command.par2.data.mid(5,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT3]  = command.par2.data.mid(6,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT4]  = command.par2.data.mid(7,1).toHex().toInt(&status, 16);

                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            for(int i = 0; i < 8; i++)  {
                                p->setButton(i,false);
                                //qDebug() << button_buff[i];
                            }
                            p->setButton(BUTTON_UP,button_buff[BUTTON_UP]);
                            p->setButton(BUTTON_RIGHT,button_buff[BUTTON_RIGHT]);
                            p->setButton(BUTTON_DOWN,button_buff[BUTTON_DOWN]);
                            p->setButton(BUTTON_LEFT,button_buff[BUTTON_LEFT]);
                            p->setButton(BUTTON_ACT1,button_buff[BUTTON_ACT1]);
                            p->setButton(BUTTON_ACT2,button_buff[BUTTON_ACT2]);
                            p->setButton(BUTTON_ACT3,button_buff[BUTTON_ACT3]);
                            p->setButton(BUTTON_ACT4,button_buff[BUTTON_ACT4]);
                        }
                    }
                } break;
                case game_command_list::set_buttons_2:  {
                    bool button_buff[BUTTON_COUNT];
                    bool status;
                    button_buff[BUTTON_ACT5]  = command.par2.data.mid(0,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT6]  = command.par2.data.mid(1,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT7]  = command.par2.data.mid(2,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_ACT8]  = command.par2.data.mid(3,1).toHex().toInt(&status, 16);
                    button_buff[BUTTON_MENU]  = command.par2.data.mid(4,1).toHex().toInt(&status, 16);
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            for(int i = 8; i < BUTTON_COUNT; i++)  {
                                p->setButton(i,false);
                                //qDebug() << button_buff[i+8];
                            }
                            p->setButton(BUTTON_ACT5,button_buff[BUTTON_ACT5]);
                            p->setButton(BUTTON_ACT6,button_buff[BUTTON_ACT6]);
                            p->setButton(BUTTON_ACT7,button_buff[BUTTON_ACT7]);
                            p->setButton(BUTTON_ACT8,button_buff[BUTTON_ACT8]);
                            p->setButton(BUTTON_MENU,button_buff[BUTTON_MENU]);
                        }
                    }
                } break;
                default:
                    qDebug() << "server has got unknown command " << command.code;
                break;
            }

        } else {

            switch(command.code)    {
                case game_command_list::set_control_id:
                    setButtonsID(command.par1.toInt());
                break;
                case game_command_list::add_player: {
                    //qDebug() << "Input command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toInt();
                    game_player_type type = (game_player_type) command.par2.toInt();
                    bool control = buttons_id == command.par2.toInt();
                    addPlayer(type, QPoint(tile_size*randomBetween(0,35),tile_size*2), control, command.par1.toInt());
                } break;
                case game_command_list::set_player_x:
                    //qDebug() << "Input command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toDouble();
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            p->setX(command.par2.toDouble());
                        }
                    }
                break;
                case game_command_list::set_player_y:
                    //qDebug() << "Input command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toDouble();
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            p->setY(command.par2.toDouble());
                        }
                    }
                break;                    
                case game_command_list::set_player_vx:
                    //qDebug() << "Input command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toDouble();
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            p->set_vx(command.par2.toDouble());
                        }
                    }
                break;
                case game_command_list::set_player_vy:
                    //qDebug() << "Input command: " << command.code << ", pars: " << command.par1.toInt() << " " << command.par2.toDouble();
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            p->set_vy(command.par2.toDouble());
                        }
                    }
                break;
                case game_command_list::remove_player:
                    qDebug() << "Try to delete player";
                    for(game_player * p : players)  {
                        if(p->id() == command.par1.toInt()) {
                            players.removeOne(p);
                            qDebug() << "Player is deleted";
                            break;
                        }
                    }
                break;
                default:
                    qDebug() << "client has got unknown command " << command.code;
                break;
            }
        }
}

void game_main_class::slotNewClientConnected(QTcpSocket *sock)  {
    qint64 player_id = addPlayer(game_player_type::cube1, QPoint(tile_size*randomBetween(0,35),tile_size*2), false);
    if(player_id == -1) {
        qDebug() << "error to add new network player";
        return;
    }

    NodeIDs.append({sock, player_id});

    server->addCommandToClient(sock,{game_command_list::set_control_id, game_network_param(player_id), 0, timeNow()});
    for(game_player * p : players)   {
        server->addCommandToClient(sock,{game_command_list::add_player, game_network_param(p->id()), game_network_param(p->type()), timeNow()});
        qDebug() << "send player add info to client";
    }
    server->addCommand({game_command_list::add_player, (game_network_param)player_id, (game_network_param)game_player_type::cube1, timeNow()});
}

void game_main_class::slotClientHasConnected(QTcpSocket *sock)  {
    qint64 player_id = -1;
    for(game_network_id &gn : NodeIDs)  {
        if(gn.sock == sock)   {
            player_id = gn.id;
            break;
        }
    }
    if(player_id == -1)
        return;
    server->addCommand({game_command_list::remove_player, (game_network_param)player_id, (game_network_param)0, timeNow()});
    qDebug() << "cleint is deleted";
}
