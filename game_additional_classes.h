#ifndef GAME_ADDITIONAL_CLASSES_H
#define GAME_ADDITIONAL_CLASSES_H

#include <QtCore>
#include <QImage>
#include <QPainter>

#include "game_enums.h"
#include "game_network_functions.h"

#define BUTTON_COUNT    13

#define BUTTON_UP       0
#define BUTTON_RIGHT    1
#define BUTTON_DOWN     2
#define BUTTON_LEFT     3
#define BUTTON_ACT1     4
#define BUTTON_ACT2     5
#define BUTTON_ACT3     6
#define BUTTON_ACT4     7
#define BUTTON_ACT5     8
#define BUTTON_ACT6     9
#define BUTTON_ACT7     10
#define BUTTON_ACT8     11
#define BUTTON_MENU     12

class game_camera   {
public:
    game_camera()   {
        position.setX(0);
        position.setY(0);
    }
    int x() { return position.x(); }
    int y() { return position.y(); }
    void setX(int x) { position.setX(x); }
    void setY(int y) { position.setX(y); }
    void setXY(int x, int y) { position.setX(x); position.setY(y); }
    QPoint pos()   { return position; }

private:
    QPoint position;
};

class game_body    {
public:
    game_body() {
        m_x = 0.0;
        m_y = 0.0;
        m_width = 0.0;
        m_height = 0.0;
    }
    game_body(double x, double y, double w, double h)   {
        m_x = x;
        m_y = y;
        m_width = w;
        m_height = h;
    }
    void setX(double x) { m_x = x; }
    void setY(double y) { m_y = y; }
    void setWidth(double w) { m_width = w; }
    void setHeight(double h) { m_height = h; }
    double x() { return m_x; }
    double y() { return m_y; }
    double width() { return m_width; }
    double height() { return m_height; }

private:
    double m_x, m_y, m_width, m_height;
};

class gravity_interface   {
public:
    gravity_interface() {
        m_body = game_body(0,0,1,1);
        m_vx = 0.0;
        m_vy = 0.0;
        m_fly = false;
    }

    void update_x() {
        game_body new_body = m_body;
        m_old_body = m_body;
        new_body.setX(m_body.x() + m_vx);
        m_body = new_body;
    }

    void update_y() {
        game_body new_body = m_body;
        m_old_body = m_body;
        m_vy += m_g;
        new_body.setY(m_body.y() + m_vy);
        m_body = new_body;
        m_fly = true;
    }
    void collideX() {
        m_vx = 0;
        restore_body();
    }
    void collideY() {
        m_vy = 0;
        restore_body();
        m_fly = false;
        if(!isMoving()) {
            if(m_vx >= m_g) m_vx -= m_g;
            if(m_vx <= -m_g) m_vx += m_g;
        }
        if(m_vx < m_g && m_vx > -m_g) m_vx = 0.0;
    }
    void restore_body()     { m_body = m_old_body; }
    void update_body()  { m_old_body = m_body; }

    bool collideBody(game_body gb)  {
        if(
                (m_body.x() + m_body.width() >= gb.x() && m_body.x() <= gb.x() + gb.width()) &&
                (m_body.y() + m_body.height() >= gb.y() && m_body.y() <= gb.y() + gb.height())
          )
        {
            return true;
        }
        return false;
    }

    void setX(double x) { m_body.setX(x); }
    void setY(double y) { m_body.setY(y); }
    void set_g(double g)    { m_g = g; }
    void set_v(double vx, double vy)    { m_vx = vx; m_vy = vy; }
    void set_vx(double vx)  { m_vx = vx; }
    void set_vy(double vy)  { m_vy = vy; }
    void setGeometry(double x, double y, double w, double h)    {
        m_body = game_body(x,y,w,h);
        m_old_body = m_body;
    }
    void setMove(bool move) { m_move = move; }

    double g()  { return m_g; }
    double vx() { return m_vx; }
    double vy() { return m_vy; }
    bool isFly()    { return m_fly; }
    bool isMoving() { return  m_move; }

    double x() { return body().x(); }
    double y() { return body().y(); }
    double width() { return body().width(); }
    double height() { return body().height(); }

    game_body body()    { return m_body; }

private:
    double m_vx, m_vy, m_g;
    bool m_fly, m_move;
    game_body m_body, m_old_body;

};


class game_object : public gravity_interface    {
public:
    game_object()   {
        bImageEnabled = false;
    }

    bool isLive()   { return live; }
    bool isImageEnabled()   { return bImageEnabled; }

    void setImage(QImage image) { m_image = image; bImageEnabled = true; }

    void draw(QPoint camera, QPainter *p)  {
        QPen pen = p->pen();
        QBrush brush = p->brush();
        if(bImageEnabled)   {
            p->drawImage(camera.x() + body().x(), camera.y() + body().y(),m_image);
        } else {
            p->setBrush(QBrush(Qt::black,Qt::NoBrush));
            p->setPen(QPen(Qt::green,Qt::SolidLine));
            p->drawRect(camera.x() + body().x(), camera.y() + body().y(), body().width(), body().height());
        }
        p->setPen(pen);
        p->setBrush(brush);
    }

private:

    bool bImageEnabled, live;
    QImage m_image;

};

class game_player : public game_object  {
public:
    game_player(game_player_type type, double g, qint64 id)   {
        m_type = type;
        set_g(g);
        max_vx = 0;
        m_id = id;
        for(int i = 0; i < BUTTON_COUNT; i++)   {
            buttons[i] = false;
        }
    }
    virtual void actionUp() {};
    virtual void actionRight() {};
    virtual void actionDown() {};
    virtual void actionLeft() {};
    virtual void action1() {};
    virtual void action2() {};
    virtual void action3() {};
    virtual void action4() {};
    virtual void update()  {};
    void live()    {
        if(isButton(BUTTON_UP)) actionUp();
        if(isButton(BUTTON_DOWN)) actionDown();
        if(isButton(BUTTON_ACT1)) action1();
        if(isButton(BUTTON_ACT2)) action2();
        if(isButton(BUTTON_ACT3)) action3();
        if(isButton(BUTTON_ACT4)) action4();
        setMove(false);
        if(isButton(BUTTON_RIGHT))  {
            setMove(true);
            actionRight();
        }
        if(isButton(BUTTON_LEFT))   {
            setMove(true);
            actionLeft();
        }
        update();
    };

    qint64 id() { return m_id; }
    game_player_type type() { return m_type; }
    bool isButton(int num)  { if(num >= BUTTON_COUNT) return false; else return buttons[num]; }
    void setButton(int num, bool value) {
        if(num >= BUTTON_COUNT)
            return;
        buttons[num] = value;
    }

    QByteArray pushNetworkData() {
        QByteArray arr;
        arr.clear();
        arr.append(gnf::pushDouble(body().x()));
        arr.append(gnf::pushDouble(body().y()));

        return arr;
    }
    void popNetworkData(QByteArray arr)   {
        Q_UNUSED(arr);
    }

private:
    double max_vx;
    game_player_type m_type;
    qint64 m_id;
    bool buttons[BUTTON_COUNT];
};



#endif // GAME_ADDITIONAL_CLASSES_H
