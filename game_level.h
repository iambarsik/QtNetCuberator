#ifndef GAME_LEVEL_H
#define GAME_LEVEL_H

#include <QObject>
#include <QPainter>
#include <QtCore>
#include <QImage>

#include "game_additional_classes.h"

class game_level : public QObject
{
    Q_OBJECT
public:
    game_level(int tile_size);

    int tile_size;
    int World[1000][1000];
    QPoint size;

    void clear();
    void setValue(int value, int x, int y);
    void setMainImage(QString main_image);
    void setFrontImage(QString front_image);
    void setMainImageFromMap();
    void setWorld(int *map, int height, int width);

    bool getCollide(int x, int y);
    bool getXCollide(game_body body);
    bool getYCollide(game_body body);

    void renderMain(QPoint camera, QPainter *p);
    void renderFront(QPoint camera, QPainter *p);

    bool isMainImageEnabled()   { return bMainImageEnabled; }
    bool isFrontImageEnabled()   { return bFrontImageEnabled; }



private:

    bool bMainImageEnabled, bFrontImageEnabled;
    QImage MAIN_IMAGE, FRONT_IMAGE;


};

#endif // GAME_LEVEL_H
