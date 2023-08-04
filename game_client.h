#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QDataStream>
#include <QTimer>
#include <QException>

#include "game_network_def.h"
#include "game_enums.h"

class game_client : public QObject
{
    Q_OBJECT
public:
    game_client(game_network_node NodeInfo, bool NA_mode = false);
    ~game_client();

    bool isConnected() { return bConnected; }

    QString getNodeName() { return Node.name; }
    qint32 getID() { return Node.ID; }

    void addCommand(game_network_command command);

signals:
    void signalPackageFromServer(QByteArray);
    void signalCommandFromServer(game_network_command);
    void clientConnected();
    void clientDisconnected();

private slots:
    void OnTimer();
    void discardSocket();
    void getErrorCode(QAbstractSocket::SocketError errorCode);

    void readyRead();

    void connectToServer();
    void slotStopReConnect();
    void slotStartReConnect();

    void sendMessage(game_network_command_type type);

private:
    QTcpSocket* socket;
    QByteArray bufferRead, bufferSend, dataSend;

    bool bConnected;
    bool bNA_MODE;
    QTimer *m_timer;
    QTimer *reconnect_timer;

    game_network_node Node;
};

#endif // GAME_CLIENT_H
