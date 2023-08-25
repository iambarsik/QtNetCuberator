#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include <QObject>

#include <QSet>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>

#include "game_network_def.h"

class game_server : public QObject
{
    Q_OBJECT
public:
    game_server(game_network_node NodeInfo);
    ~game_server();

    void addCommand(game_network_command command);
    void addCommandToClient(QTcpSocket* sock, game_network_command command);
    void addPackage(QByteArray package);

    qint32 getClientCount() { return connection_set.size(); }

signals:
    void signalNewClientConnected(QTcpSocket *sock);
    void signalClientHasDisconnected(QTcpSocket *sock);
    void signalClientsAreConnected(int);
    void signalCommandFromClient(game_network_command);

private slots:

    void OnTimer();

    void newConnection();
    void appendToSocketList(QTcpSocket* socket);
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void sendMessage(QTcpSocket* socket, game_network_command_type type);

private:
    QTcpServer* m_server;
    QSet<QTcpSocket*> connection_set;
    QByteArray bufferRead, bufferSend, dataSend;

    QTimer *m_timer;

    game_network_node Node;
};

#endif // GAME_SERVER_H
