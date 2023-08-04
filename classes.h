#ifndef CLASSES_H
#define CLASSES_H

#include <QtWidgets>

struct mySettings   {
    bool bDevMode;
    bool bEnglish;
};

class TouchButton   {
public:
    QRect Geometry;
    QImage imgPressed, imgUnpressed;
    bool Value, oldValue, Enable, bImageEnabled;
    int Code;

    TouchButton(QRect geometry,int code)   {
        Geometry = geometry;
        Value = false;
        oldValue = false;
        Code = code;
        Enable = true;
        bImageEnabled = false;
    }

    void SetImage(QImage pressed, QImage unpressed) {
        imgPressed = pressed;
        imgUnpressed = unpressed;
        bImageEnabled = true;
    }

    void Draw(QPainter *pnt)    {
        if(bImageEnabled)   {
            if(Value)   {
                pnt->drawImage(Geometry.x(),Geometry.y(),imgPressed);
            } else {
                pnt->drawImage(Geometry.x(),Geometry.y(),imgUnpressed);
            }
        } else {
            if(Value)   {
                pnt->setPen(QPen(Qt::red));
                pnt->setBrush(QBrush(Qt::red));
                pnt->setBrush(Qt::SolidPattern);
            } else {
                pnt->setPen(QPen(Qt::white));
                pnt->setBrush(QBrush(Qt::white));
                pnt->setBrush(Qt::NoBrush);
            }
            pnt->drawEllipse(Geometry.x(),Geometry.y(),Geometry.width(),Geometry.height());
        }
    }
};


#endif // CLASSES_H
