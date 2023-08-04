#include "game_level.h"

game_level::game_level(int tile_size)
{
    this->tile_size = tile_size;
}


void game_level::clear() {
    for(int i = 0; i < 1000; i++)   {
        for(int j = 0; j < 1000; j++)   {
            World[j][i] = 0;
        }
    }
}

void game_level::setValue(int value, int x, int y)  {
    World[x][y] = value;
}

void game_level::setMainImage(QString main_image)   {
    MAIN_IMAGE.load(main_image);
    bMainImageEnabled = true;
}

void game_level::setFrontImage(QString front_image) {
    FRONT_IMAGE.load(front_image);
    bFrontImageEnabled = true;
}

void game_level::setMainImageFromMap()  {
    qDebug() << "level create render";
    QImage img(size.x()*tile_size,size.y()*tile_size,QImage::Format_RGB32);
    QPainter p(&img);
    p.setPen((QPen(Qt::black)));
    p.setBrush((QBrush(Qt::black)));
    p.drawRect(0,0,img.width(),img.height());
    p.setPen(QPen(Qt::white));
    p.setBrush((QBrush(Qt::NoBrush)));
    for(int y = 0; y < size.y(); y++)    {
        for(int x = 0; x < size.x(); x++)    {
            if(World[y][x] == 1)    {
                p.drawRect(tile_size*x, tile_size*y, tile_size, tile_size);
            }
        }
    }
    MAIN_IMAGE = img.copy();
    bMainImageEnabled = true;
}

void game_level::setWorld(int *map, int height, int width)  {
    clear();
    size.setX(width);
    size.setY(height);
    for(int i = 0; i < size.y(); i++) {
        for(int j = 0; j < size.x(); j++) {
            setValue(*(map + i*size.x() + j),i,j);
        }
    }
}

bool game_level::getCollide(int x, int y)   {
    try    {
        int tile_x = x/tile_size;
        int tile_y = y/tile_size;
        if(tile_x < 0 || tile_x > size.x()
                || tile_y < 0 || tile_y > size.y())
            return true;
        else
            return World[tile_y][tile_x];
    } catch (...) {
        return true;
    }
    return false;
}
bool game_level::getXCollide(game_body body)    {
    int col_x,col_y;
    for(int i = 1; i < 10; i++)     {
        col_x = body.x();
        col_y = body.y() + (body.height()/i);
        if(getCollide(col_x,col_y)) return true;
        col_y = body.y() + body.height() - (body.height()/i);
        if(getCollide(col_x,col_y)) return true;
    }
    for(int i = 1; i < 10; i++)     {
        col_x = body.x() + body.width();
        col_y = body.y() + (body.height()/i);
        if(getCollide(col_x,col_y)) return true;
        col_y = body.y() + body.height() - (body.height()/i);
        if(getCollide(col_x,col_y)) return true;
    }
    return false;
}
bool game_level::getYCollide(game_body body)    {
    int col_x,col_y;
    for(int i = 1; i < 10; i++)     {
        col_x = body.x() + (body.width()/i);
        col_y = body.y();
        if(getCollide(col_x,col_y)) return true;
        col_x = body.x() + body.width() - (body.width()/i);
        if(getCollide(col_x,col_y)) return true;
    }
    for(int i = 1; i < 10; i++)     {
        col_x = body.x() + (body.width()/i);
        col_y = body.y() + body.height();
        if(getCollide(col_x,col_y)) return true;
        col_x = body.x() + body.width() - (body.width()/i);
        if(getCollide(col_x,col_y)) return true;
    }
    return false;
}

void game_level::renderMain(QPoint camera, QPainter *p)    {
    p->drawImage(camera.x(),camera.y(),MAIN_IMAGE);
}

void game_level::renderFront(QPoint camera, QPainter *p)   {
    p->drawImage(camera.x(),camera.y(),FRONT_IMAGE);
}
















