#include "game_server.h"

game_server::game_server(game_network_node NodeInfo)    {
    m_server = new QTcpServer(this);

    Node = NodeInfo;

    if(m_server->listen(QHostAddress::Any, Node.port))   {
       connect(m_server, &QTcpServer::newConnection,
               this, &game_server::newConnection);

        qDebug() << "System :: server is listening...";
    } else {
        qDebug() << QString("System :: unable to start server. Code = %1").arg(m_server->errorString());
        exit(EXIT_FAILURE);
    }

    m_timer = new QTimer;
    m_timer->setInterval(500);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    m_timer->start();
}

game_server::~game_server() {
    foreach (QTcpSocket* socket, connection_set)    {
        socket->disconnectFromHost();
        socket->close();
        socket->deleteLater();
    }
    m_server->close();
    m_server->deleteLater();
    m_timer->stop();
    delete m_timer;
}

void game_server::addCommand(game_network_command command)  {
    //qDebug() << QString("Sending command to all clients: %1 %2 %3").arg(command.code).arg(command.par1.toInt()).arg(command.par2.toDouble());
    //qDebug() << QString("Sending command to all clients: %1").arg(command.code) << command.par1.data << command.par2.data;
    for(auto sock : connection_set) {
        if(sock == nullptr || sock->state() != QAbstractSocket::ConnectedState )   {
            qDebug() << "Cant send a command to client because it is unconnected";
            continue;
        }
        bufferSend.clear();
        bufferSend.append((command.code >> 24) & 0xFF);
        bufferSend.append((command.code >> 16) & 0xFF);
        bufferSend.append((command.code >> 8)  & 0xFF);
        bufferSend.append((command.code) & 0xFF);
        bufferSend.append(command.par1.data);
        bufferSend.append(command.par2.data);

        sendMessage(sock, game_network_command_type::com_buffer);
    }
}

void game_server::addCommandToClient(QTcpSocket *sock, game_network_command command)    {
    //qDebug() << QString("Sending single command: %1").arg(command.code) << command.par1.data << command.par2.data;

    //qDebug () << command.par1.toInt();
    //qDebug () << command.par2.toInt();

    //qDebug () << command.par1.data;
    //qDebug () << command.par2.data;

    bufferSend.clear();
    bufferSend.append((command.code >> 24) & 0xFF);
    bufferSend.append((command.code >> 16) & 0xFF);
    bufferSend.append((command.code >> 8)  & 0xFF);
    bufferSend.append((command.code) & 0xFF);
    bufferSend.append(command.par1.data);
    bufferSend.append(command.par2.data);

    sendMessage(sock, game_network_command_type::com_buffer);
}

void game_server::addPackage(QByteArray package)    {
    for(auto sock : connection_set) {
        bufferSend.clear();
        bufferSend.append(package);
        sendMessage(sock, game_network_command_type::com_package);
    }
}

void game_server::OnTimer() {
    for(auto sock : connection_set) {
        sendMessage(sock, game_network_command_type::com_ping);
    }
}

void game_server::newConnection()   {
    while (m_server->hasPendingConnections())   {
        appendToSocketList(m_server->nextPendingConnection());
    }
}

void game_server::appendToSocketList(QTcpSocket *socket)    {
    connection_set.insert(socket);
    connect(socket, &QTcpSocket::readyRead, this, &game_server::readyRead);
    connect(socket, &QTcpSocket::disconnected, this, &game_server::discardSocket);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));
    qDebug() << QString("Node %1 descriptor has connected").arg(socket->socketDescriptor());

    //sendMessage(socket, game_network_command_type::com_init);

    emit signalNewClientConnected(socket);
    emit signalClientsAreConnected(connection_set.size());
}

void game_server::discardSocket()   {
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    QSet<QTcpSocket*>::iterator it = connection_set.find(socket);
    if (it != connection_set.end()){

        qDebug() << QString("Node has disconnected").arg(socket->socketDescriptor());
        connection_set.remove(*it);
    }
    //socket->deleteLater();
    emit signalClientsAreConnected(connection_set.size());
}

void game_server::getErrorCode(QAbstractSocket::SocketError errorCode)  {
    qDebug() << "Socket Error = " << errorCode;
}

void game_server::readyRead()   {

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    if( socket->state() == QAbstractSocket::ConnectedState )    {

        bufferRead.clear();
        bufferRead = socket->readAll();

        //qDebug() << "Length: " << bufferRead.length();
        //qDebug() << bufferRead;


        while(bufferRead.length() > 0)  {

            bool status;
            QByteArray header;
            header = bufferRead.mid(0,8);

            qint32 iSize = header.mid( 1, 1).toHex().toInt(&status, 16);
            qint32 iMainNode = header.mid( 2, 1).toHex().toInt(&status, 16);
            qint32 iClientNode = header.mid( 3, 1).toHex().toInt(&status, 16);
            qint32 iNetworkFlag = header.mid( 4, 1).toHex().toInt(&status, 16);
            qint32 iFrameType = header.mid( 5, 1).toHex().toInt(&status, 16);

            Q_UNUSED(iSize);
            Q_UNUSED(iMainNode);
            Q_UNUSED(iClientNode);
            Q_UNUSED(iNetworkFlag);
            Q_UNUSED(iFrameType);

            //qDebug() << "Size = " << iSize << " FrameType = " << iFrameType;

                // checking for default command
            if(iSize == 32) {
                QByteArray frame;
                frame = bufferRead.mid(0,32);
                frame.remove(0,8);

                if(frame.length() >= 24)    {                   // checking if frame is correct
                    bool status;
                    uint iCommand = frame.mid( 0,4).toHex().toUInt(&status,16);

                    game_network_param p1 = game_network_param(frame.mid( 4,8));
                    game_network_param p2 = game_network_param(frame.mid( 12,8));

                    qint32 iTime    = frame.mid(20,4).toHex().toInt(&status,16);
                    //bufferRead.remove(0, 32);                   // delete frame from buffer

                    qDebug() << "Input command : " << iCommand << " " << p1.data << " " << p2.data;
                    emit signalCommandFromClient({iCommand, p1, p2, iTime});
                } else {
                    qDebug() << "wrong frame size " << frame.length();
                }
            } else {
                qDebug() << "wrong pack size " << iSize;
            }
            bufferRead.remove(0,iSize);
        }
    }
}

void game_server::sendMessage(QTcpSocket *socket, game_network_command_type type)   {
    switch(type)    {
        case com_init: {

            QByteArray byteArray;

            byteArray.append((char) 0x00);
            byteArray.append((char) 13 + Node.name.size());
            byteArray.append((char) Node.ID);
            byteArray.append((char) Node.mainNode);
            byteArray.append((char) 0x00);
            byteArray.append((char) Node.frameType);
            byteArray.append((char) 0x00);
            byteArray.append((char) 0x00);

            byteArray.append((char) 0xC0);
            byteArray.append((char) 0x00);
            byteArray.append((char) 0x00);
            byteArray.append((char) 0x03);

            byteArray.append(Node.name);
            byteArray.append((char) 0x00);

            socket->write(byteArray);
            socket->flush();
            socket->waitForBytesWritten(10000);
            qDebug() << "Init command is sent";

        } break;
        case com_ping: {
            dataSend.clear();
            // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);           // 1 байт - 0
            dataSend.append((char) 0x08);           // 2 байт - текущая длинна кадра в байтах (включая размер заголовка)
            dataSend.append((char) Node.ID);        // 3 байт - идентификатор получателя
            dataSend.append((char) Node.mainNode);  // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x00);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType); // 6 байт - тип кадра
            dataSend.append((char) 0x00);           // 7 байт - зарезервировано
            dataSend.append((char) 0x00);           // 8 байт - зарезервировано
            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
        case com_buffer: {
            dataSend.clear();
            // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);                       // 1 байт - 0
            dataSend.append((char) 8 + bufferSend.size() + 4);  // 2 байт - текущая длинна кадра в байтах (размер заголовка + size + time)
            dataSend.append((char) Node.ID);              		// 3 байт - идентификатор получателя
            dataSend.append((char) Node.mainNode);              // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x00);                       // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType);             // 6 байт - тип кадра
            dataSend.append((char) 0x00);                       // 7 байт - зарезервировано
            dataSend.append((char) 0x00);                       // 8 байт - зарезервировано

            // 4 байта - модельное время команды в миллисекундах
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            // модельное время команды в миллисекундах

            dataSend.append(bufferSend);
            bufferSend.clear(); // очищаем буфер

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
        case com_package: {
            dataSend.clear();

            qint32 iPackSize = bufferSend.size();

            // 8 байт - заголовок кадра
            dataSend.append((char) 0x01);           // 1 байт - признак пакета для синхронизации ядра
            dataSend.append((char) 0xFF);           // 2 байт - текущая длинна кадра в байтах (в пакете не используется)
            dataSend.append((char) Node.mainNode);  // 3 байт - идентификатор получателя
            dataSend.append((char) Node.ID);        // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x00);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType); // 6 байт - тип кадра
            dataSend.append((char) 0x00);           // 7 байт - зарезервировано
            dataSend.append((char) 0x00);           // 8 байт - зарезервировано

            // 4 байта - размер пакета ядра
            bufferSend.prepend((iPackSize >> 24) & 0xFF);
            bufferSend.prepend((iPackSize >> 16) & 0xFF);
            bufferSend.prepend((iPackSize >> 8)  & 0xFF);
            bufferSend.prepend((iPackSize) & 0xFF);

            dataSend.append(bufferSend);

            qDebug() << "dataSend.length()" << dataSend.length();// << "bufferSend" << bufferSend;
            //qDebug() << dataSend;
            bufferSend.clear(); // очищаем буфер

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
    }
}
