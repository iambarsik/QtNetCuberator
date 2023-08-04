#include "widget.h"

void Widget::SetStartValues()   {
    Settings.bDevMode = true;
    Settings.bEnglish = true;



    game->clear();
    game->setup();















}

bool Widget::event(QEvent *event)   {
    switch (event->type())  {
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:  {
            QTouchEvent* pte = static_cast<QTouchEvent*>(event);
            listKeys = pte->touchPoints();
            update();
        } break;
        default: return QWidget::event(event);
    }
    return true;
}

void Widget::OnKeys(int Key, bool press)    {

    switch(Key) {
        case Qt::Key_Up:
        case Qt::Key_W: {
            myKey[BUTTON_UP] = press;
        } break;
        case Qt::Key_Right:
        case Qt::Key_D: {
            myKey[BUTTON_RIGHT] = press;
        } break;
        case Qt::Key_Down:
        case Qt::Key_S: {
            myKey[BUTTON_DOWN] = press;
        } break;
        case Qt::Key_Left:
        case Qt::Key_A: {
            myKey[BUTTON_LEFT] = press;
        } break;
        case Qt::Key_Space: {

        } break;
        case Qt::Key_Return:
        case Qt::Key_J: {
            myKey[BUTTON_ACT1] = press;
        } break;
        case Qt::Key_K: {
            myKey[BUTTON_ACT2] = press;
        } break;
        case Qt::Key_U: {
            myKey[BUTTON_ACT3] = press;
        } break;
        case Qt::Key_I: {
            myKey[BUTTON_ACT4] = press;
        } break;
        case Qt::Key_Q: {
            myKey[BUTTON_ACT5] = press;
        } break;
        case Qt::Key_E: {
            myKey[BUTTON_ACT6] = press;
        } break;
        case Qt::Key_H: {
            myKey[BUTTON_ACT7] = press;
        } break;
        case Qt::Key_Y: {
            myKey[BUTTON_ACT8] = press;
        } break;
        case Qt::Key_P: {

        } break;
        case Qt::Key_L: {

        } break;
        case Qt::Key_Escape: {
           exit(0);
        } break;
    }
}

void Widget::OnTouch()  {
    for(int i = 0; i < BUTTON_COUNT; i++) {
        TouchKeys[i]->Value = false;
    }
    int tpx;
    int tpy;
    double tpdX;
    double tpdY;
    foreach (QTouchEvent::TouchPoint tp, listKeys)  {
        if(bRotateScreen == false)  {
            tpx = tp.pos().x();
            tpy = tp.pos().y();
            tpdX = dX;
            tpdY = dY;
        } else {
            tpx = tp.pos().y();
            tpy = (this->width() - tp.pos().x());
            tpdX = dY;
            tpdY = dX;
        }
        switch (tp.state()) {
            case Qt::TouchPointReleased:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = false;
                    }
                }
            break;
            case Qt::TouchPointStationary:
            case Qt::TouchPointPressed:
            case Qt::TouchPointMoved:
                for(int i = 0; i < BUTTON_COUNT; i++) {
                    if(
                      (tpx > TouchKeys[i]->Geometry.x()*tpdX && tpx < (TouchKeys[i]->Geometry.x() + TouchKeys[i]->Geometry.width())*tpdX)&&
                      (tpy > TouchKeys[i]->Geometry.y()*tpdY && tpy < (TouchKeys[i]->Geometry.y() + TouchKeys[i]->Geometry.height())*tpdY)
                      ) {
                            TouchKeys[i]->Value = true;
                    }
                }
            break;
        }
    }
    for(int i = 0; i < BUTTON_COUNT; i++) {
        if(TouchKeys[i]->oldValue != TouchKeys[i]->Value)    {
            OnKeys(TouchKeys[i]->Code,TouchKeys[i]->Value);
            TouchKeys[i]->oldValue = TouchKeys[i]->Value;
        }
    }
}

void Widget::setColor(QColor brush, QColor pen, Qt::BrushStyle style, QPainter *qScr) {
    qScr->setBrush(QBrush(brush, style));
    qScr->setPen(QPen(pen));
}

void Widget::keyPressEvent(QKeyEvent *event)    {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),true);
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)  {
    if(!event->isAutoRepeat())
        OnKeys(event->key(),false);
    QWidget::keyReleaseEvent(event);
}
