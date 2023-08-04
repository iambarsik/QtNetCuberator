#include "game_client.h"

game_client::game_client(game_network_node NodeInfo, bool NA_mode)
    : bConnected(false)
{
    socket = new QTcpSocket(this);

    Node = NodeInfo;
    bNA_MODE = NA_mode;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket,SIGNAL(connected()),this,SLOT(slotStopReConnect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(slotStartReConnect()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getErrorCode(QAbstractSocket::SocketError)));

    m_timer = new QTimer;
    m_timer->setInterval(100);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    reconnect_timer = new QTimer;
    reconnect_timer->setInterval(1000);
    connect(reconnect_timer,SIGNAL(timeout()),this,SLOT(connectToServer()));
    reconnect_timer->start();
}

game_client::~game_client() {
    socket->disconnectFromHost();
    socket->deleteLater();
}

void game_client::addCommand(game_network_command command)  {
   bufferSend.clear();
   bufferSend.append((command.code >> 24) & 0xFF);
   bufferSend.append((command.code >> 16) & 0xFF);
   bufferSend.append((command.code >> 8)  & 0xFF);
   bufferSend.append((command.code) & 0xFF);
   bufferSend.append(command.par1.data);
   bufferSend.append(command.par2.data);

   qDebug() << QString("Command is sent: %1 ").arg(command.code) << command.par1.data << command.par2.data;
   sendMessage(game_network_command_type::com_buffer);
}

void game_client::OnTimer() {
    if(bConnected == true)  {
        sendMessage(game_network_command_type::com_ping);
    }
}

void game_client::discardSocket()   {
    qDebug() << "Disconnected from server!";
}

void game_client::getErrorCode(QAbstractSocket::SocketError errorCode)  {
    qDebug() << "Socket Error = " << errorCode;
}

void game_client::readyRead()   {
    if( socket->state() == QAbstractSocket::ConnectedState )    {

        bufferRead.clear();
        bufferRead = socket->readAll();

        //qDebug() << "Input package length " << bufferRead.length();

        if(bNA_MODE == false)    {
            if(bufferRead.length() >= 32)   {

                bool status;
                int iPackageFlag = bufferRead.mid(0,1).toHex().toInt(&status,16);

                if(iPackageFlag == 0)   {

                    while(bufferRead.length() > 0)  {

                        QByteArray header;
                        header = bufferRead.mid(0,8);

                        int iSize = header.mid( 1, 1).toHex().toInt(&status, 16);
                        int iClientNode = header.mid( 3, 1).toHex().toInt(&status, 16);
                        int iNetworkFlag = header.mid( 4, 1).toHex().toInt(&status, 16);
                        int iFrameType = header.mid( 5, 1).toHex().toInt(&status, 16);

                        Q_UNUSED(iSize);
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

                                //qDebug() << "Input command from server : " << iCommand << " p1: " << p1.data << " p2: " << p2.data << " time: " << iTime;
                                emit signalCommandFromServer({iCommand, p1, p2, iTime});
                            } else {
                                qDebug() << "wrong frame size " << frame.length();
                            }

                        } else {
                            qDebug() << "wrong pack size " << iSize;
                        }
                        bufferRead.remove(0,iSize);
                    }

                } else {
                    /*
                    // have got core package flag
                    bool status;
                    QByteArray package = bufferRead;
                    package.remove(0,8); // delete header
                    int iPackageSize = package.mid(0,4).toHex().toInt(&status,16);
                    Q_UNUSED(iPackageSize);
                    //qDebug() << "Package size = " << iPackageSize;
                    package.remove(0,4); // delete 4 bytes about package size
                    emit signalPackageFromServer(package);
                    */
                    qDebug() << "WHAT THE FUCK?????";
                }
            }

        } else {

        }
    }
}

void game_client::connectToServer() {
    qDebug() << " trying to connect";
    socket->connectToHost(Node.host,Node.port);
    if(!socket->waitForConnected(10))  {
        qDebug() << "System :: connecting ERROR";
        return;
    }
    bConnected = true;
    reconnect_timer->stop();
    m_timer->start();
    qDebug() << "Connected to Server";
    qDebug() << QString("System :: connected to server");
    //sendMessage(game_network_command_type::com_init);
    sendMessage(game_network_command_type::com_ping);
    emit clientConnected();
}

void game_client::slotStopReConnect()   {
    reconnect_timer->stop();
    m_timer->start();
}

void game_client::slotStartReConnect()  {
    bConnected = false;
    emit clientDisconnected();
    reconnect_timer->start();
    m_timer->stop();
}

void game_client::sendMessage(game_network_command_type type)   {
    switch(type)    {
        case com_init: {
            if(socket)  {
                if(socket->isOpen())    {

                    dataSend.clear();

                    dataSend.append((char) 0x00);
                    dataSend.append((char) 13 + Node.name.size());
                    dataSend.append((char) Node.ID);
                    dataSend.append((char) Node.mainNode);
                    dataSend.append((char) 0x01);
                    dataSend.append((char) Node.frameType);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x00);

                    dataSend.append((char) 0xC0);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x00);
                    dataSend.append((char) 0x03);

                    dataSend.append(Node.name);
                    dataSend.append((char) 0x00);

                    qDebug() << "Init command is sent";

                    socket->write(dataSend);
                    socket->flush();
                    socket->waitForBytesWritten(10000);

                } else {
                    qDebug() << "Socket doesn't seem to be opened";
                }
            } else {
                qDebug() << "Not connected";
            }
        } break;
        case com_ping: {
            dataSend.clear();
            // 8 байт - заголовок кадра
            dataSend.append((char) 0x00);           // 1 байт - 0
            dataSend.append((char) 0x08);           // 2 байт - текущая длинна кадра в байтах (включая размер заголовка)
            dataSend.append((char) Node.ID);  		// 3 байт - идентификатор получателя
            dataSend.append((char) Node.mainNode);  // 4 байт - идентификатор отправителя (номер узла)
            dataSend.append((char) 0x01);           // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
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
            dataSend.append((char) 0x01);                       // 5 байт - флаг кадра ( 0 - я сервер, 1 - я клинет )
            dataSend.append((char) Node.frameType);             // 6 байт - тип кадра
            dataSend.append((char) 0x00);                       // 7 байт - зарезервировано
            dataSend.append((char) 0x00);                       // 8 байт - зарезервировано

                // 4 байта - модельное время команды в миллисекундах
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);
            bufferSend.append((char) 0x00);

            dataSend.append(bufferSend);

            //qDebug() << "Length: " << dataSend.length();

            bufferSend.clear(); // очищаем буфер

            socket->write(dataSend);
            socket->flush();
            socket->waitForBytesWritten(10000);
        } break;
        case game_network_command_type::com_package: {

        } break;
    }
}
