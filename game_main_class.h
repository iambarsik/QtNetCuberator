#ifndef GAME_MAIN_CLASS_H
#define GAME_MAIN_CLASS_H

#include <QObject>
#include <QList>

#include <QPainter>
#include <QSettings>
#include <QFile>
#include <QVariant>

#include "qgametext.h"

#include "game_additional_classes.h"
#include "game_level.h"
#include "game_enums.h"

#include "game_network_def.h"
#include "game_server.h"
#include "game_client.h"

#include "player_cube_1.h"


class game_main_class : public QObject
{
    Q_OBJECT
public:
    game_main_class(int tile_size, double g = 0);

    char getVersion() { return m_game_version; }
    bool isServer() { return m_server; }

    qint32 timeNow();

    void update();
    void animate();
    void render(QPainter *p);
    void clear();
    void setup();

    qint64 addPlayer(game_player_type type, QPoint pos, bool control, qint64 id = -1);
    qint64 genID() { m_ids++; return m_ids; }

    void setButtonsID(int id)    { buttons_id = id; }
    int getButtonsID()  { return  buttons_id; }

    void setButton(int num, bool value) {
        if(num >= BUTTON_COUNT)
            return;
        buttons[num] = value;
    }

    int randomBetween(int low, int high) { return (qrand() % ((high + 1) - low) + low); };

public slots:
    void setClientInformation(int count);
    void slotReadCommand(game_network_command command);
    void slotNewClientConnected(QTcpSocket *sock);


private:
    char m_game_version;
    bool m_server;

    game_camera camera;
    int tile_size;
    double g_gravity;

    game_level level;
    QList<game_player *> players;

    gametextQ gt;

    qint64 m_ids = 100;

    bool buttons[BUTTON_COUNT];
    int buttons_id = -1;

    game_server *server;
    game_client *client;

    game_network_node CurrentNode;
    QList<game_network_node> NA_Nodes;

    bool bServerNode;
    int game_network_player_count;

};

#endif // GAME_MAIN_CLASS_H
