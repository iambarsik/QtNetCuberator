
#ifndef GAME_NETWORK_DEF_H
#define GAME_NETWORK_DEF_H

#include <QtCore>
#include <QTcpSocket>

struct game_network_param    {
    QByteArray data;
    game_network_param(QByteArray arr)	{
        data.clear();
        data.append(arr);
    }
    game_network_param() {}
    game_network_param(uint value) {
        data.clear();
        qint64 v = (qint64) value;
        data.append(static_cast<char>((v >> 56) & 0xFF));
        data.append(static_cast<char>((v >> 48) & 0xFF));
        data.append(static_cast<char>((v >> 40) & 0xFF));
        data.append(static_cast<char>((v >> 32) & 0xFF));
        data.append(static_cast<char>((v >> 24) & 0xFF));
        data.append(static_cast<char>((v >> 16) & 0xFF));
        data.append(static_cast<char>((v >> 8) & 0xFF));
        data.append(static_cast<char>((v >> 0) & 0xFF));
    }
    game_network_param(qint32 value) {
        data.clear();
        qint64 v = (qint64) value;
        data.append(static_cast<char>((v >> 56) & 0xFF));
        data.append(static_cast<char>((v >> 48) & 0xFF));
        data.append(static_cast<char>((v >> 40) & 0xFF));
        data.append(static_cast<char>((v >> 32) & 0xFF));
        data.append(static_cast<char>((v >> 24) & 0xFF));
        data.append(static_cast<char>((v >> 16) & 0xFF));
        data.append(static_cast<char>((v >> 8) & 0xFF));
        data.append(static_cast<char>((v >> 0) & 0xFF));
    }
    game_network_param(qint64 value) {
        data.clear();
        data.append(static_cast<char>((value >> 56) & 0xFF));
        data.append(static_cast<char>((value >> 48) & 0xFF));
        data.append(static_cast<char>((value >> 40) & 0xFF));
        data.append(static_cast<char>((value >> 32) & 0xFF));
        data.append(static_cast<char>((value >> 24) & 0xFF));
        data.append(static_cast<char>((value >> 16) & 0xFF));
        data.append(static_cast<char>((value >> 8) & 0xFF));
        data.append(static_cast<char>((value >> 0) & 0xFF));
    }
    game_network_param(double value) {
        data.clear();
        QDataStream stream(&data,QIODevice::WriteOnly);
        stream << value;
    }
    game_network_param(char value)   {
        data.clear();
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(static_cast<char>(0x00));
        data.append(value);
    }
    qint64 toInt()   {
        qint64 result;
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> result;
        return result;
    }
    double toDouble()   {
        double result;
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> result;
        return result;
    }
    char toChar()   {
        char ch = *data.mid(7,1).data();
        return ch;
    }
};

struct game_network_id  {
    QTcpSocket *sock;
    qint64 id;
};

struct game_network_node  {
    QString name;
    QString host;
    bool isServer;
    qint32 port;
    qint32 ID;
    qint32 mainNode;
    qint32 frameType;
    QString modelName;
};

struct game_network_command {
    uint code;
    game_network_param par1;
    game_network_param par2;
    qint32 time;
};

enum game_network_command_type   {
    com_init,
    com_buffer,
    com_ping,
    com_package
};

enum game_command_list  {
    set_control_id = 0x0001000, /* server     id,       0,     time */

    add_player,                 /* server     id,    type,     time */
    remove_player,              /* server     id,       0,     time */
    set_player_x,               /* server     id,       x,     time */
    set_player_y,               /* server     id,       y,     time */
    set_player_vx,              /* server     id,      vx,     time */
    set_player_vy,              /* server     id,      vy,     time */

    set_buttons_1,              /* client     id, 8 bytes,     time */
                                /*    u, r, d, l, a1, a2, a3, a4    */
    set_buttons_2,              /* client     id, 8 bytes,     time */
                                /*    a5, a6, a7, a8, m, 0, 0, 0    */

    set_player_info,

    add_bullet,                 /* server     id,    type,     time */
    remove_bullet,              /* server     id,       0,     time */
    set_bullet_x,               /* server     id,       x,     time */
    set_bullet_y,               /* server     id,       y,     time */
    set_bullet_vx,              /* server     id,      vx,     time */
    set_bullet_vy,              /* server     id,      vy,     time */






};

#endif // GAME_NETWORK_DEF_H
