#ifndef GAME_NETWORK_FUNCTIONS_H
#define GAME_NETWORK_FUNCTIONS_H

#include <QtCore>

// game network function
namespace gnf {

inline QByteArray pushDouble(double value)  {
    QByteArray arr;
    arr.clear();
    QDataStream stream(&arr, QIODevice::WriteOnly);
    stream << value;
    return arr;
}
inline double popDouble(QByteArray &arr)
{
    double value;
    QDataStream stream(arr);
    stream >> value;
    arr.remove(0,sizeof (value));
    return value;
}
inline QByteArray pushInt32(qint32 value) {
    QByteArray arr;
    arr.clear();
    qint64 v = (qint64) value;
    arr.append(static_cast<char>((v >> 56) & 0xFF));
    arr.append(static_cast<char>((v >> 48) & 0xFF));
    arr.append(static_cast<char>((v >> 40) & 0xFF));
    arr.append(static_cast<char>((v >> 32) & 0xFF));
    arr.append(static_cast<char>((v >> 24) & 0xFF));
    arr.append(static_cast<char>((v >> 16) & 0xFF));
    arr.append(static_cast<char>((v >> 8) & 0xFF));
    arr.append(static_cast<char>((v >> 0) & 0xFF));
    return arr;
}
inline QByteArray pushInt64(qint64 value) {
    QByteArray arr;
    arr.clear();
    arr.append(static_cast<char>((value >> 56) & 0xFF));
    arr.append(static_cast<char>((value >> 48) & 0xFF));
    arr.append(static_cast<char>((value >> 40) & 0xFF));
    arr.append(static_cast<char>((value >> 32) & 0xFF));
    arr.append(static_cast<char>((value >> 24) & 0xFF));
    arr.append(static_cast<char>((value >> 16) & 0xFF));
    arr.append(static_cast<char>((value >> 8) & 0xFF));
    arr.append(static_cast<char>((value >> 0) & 0xFF));
    return arr;
}
inline QByteArray pushUInt(uint value) {
    QByteArray arr;
    arr.clear();
    qint64 v = (qint64) value;
    arr.append(static_cast<char>((v >> 56) & 0xFF));
    arr.append(static_cast<char>((v >> 48) & 0xFF));
    arr.append(static_cast<char>((v >> 40) & 0xFF));
    arr.append(static_cast<char>((v >> 32) & 0xFF));
    arr.append(static_cast<char>((v >> 24) & 0xFF));
    arr.append(static_cast<char>((v >> 16) & 0xFF));
    arr.append(static_cast<char>((v >> 8) & 0xFF));
    arr.append(static_cast<char>((v >> 0) & 0xFF));
    return arr;
}
inline qint64 popInt(QByteArray &arr)  {
    qint64 value;
    QDataStream stream(arr);
    stream >> value;
    arr.remove(0,sizeof (value));
    return value;
}
inline QByteArray pushBool(bool value) {
    QByteArray arr;
    arr.clear();
    arr.append((char)value);
    return arr;
}
inline bool popBool(QByteArray &arr)   {
    bool value;
    bool status;
    value = arr.mid(0,1).toHex().toInt(&status, 16);
    return value;
}




























}

#endif // GAME_NETWORK_FUNCTIONS_H
