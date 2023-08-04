#ifndef QGAMETEXT_H
#define QGAMETEXT_H

/*
 This module works with QPainter.

 For using this you need:
 - copy this "qgametext.h" file and "font.qrc" into your project folder;
 - copy the font folder into your project folder;
 - add font.qrc to your widget project;
 - include the header to your code.

 It contains a basic russian and english symbols and supports only white color for now.
*/

#include <QImage>
#include <QPainter>

enum GameTextSize    {
    font_size_micro,
    font_size_small,
    font_size_meduim,
    font_size_large
};

class gametextQ {
public:

    gametextQ() {
        sSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,!:+-?* abcdefghijklmnopqrstuvwxyzАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя=%";
        FONT_IMAGE.load(":/font/font_micro.png");
        for(int i = 0; i < 137; i++) {
            imgSymbol_micro[i] = FONT_IMAGE.copy(i*8,0,8,8);
        }
        FONT_IMAGE.load(":/font/font_small.png");
        for(int i = 0; i < 137; i++) {
            imgSymbol_small[i] = FONT_IMAGE.copy(i*15,0,15,16);
        }
        FONT_IMAGE.load(":/font/font_medium.png");
        for(int i = 0; i < 137; i++) {
            imgSymbol_medium[i] = FONT_IMAGE.copy(i*30,0,30,32);
        }
        FONT_IMAGE.load(":/font/font_big.png");
        for(int i = 0; i < 137; i++) {
            imgSymbol_big[i] = FONT_IMAGE.copy(i*45,0,45,48);
        }
    };

    void drawText(int x, int y, GameTextSize size, QString text, QPainter *p)   {
        switch (size) {
            case GameTextSize::font_size_micro:
                for(int cnt = 0; cnt < text.size(); cnt++)    {
                    for(int i = 0; i < sSymbols.size(); i++)    {
                        if(text.at(cnt) == sSymbols.at(i))    {
                            p->drawImage(x + (cnt)*imgSymbol_micro[i].width(),y,imgSymbol_micro[i]);
                        }
                    }
                }
            break;
            case GameTextSize::font_size_small:
                for(int cnt = 0; cnt < text.size(); cnt++)    {
                    for(int i = 0; i < sSymbols.size(); i++)    {
                        if(text.at(cnt) == sSymbols.at(i))    {
                            p->drawImage(x + (cnt)*imgSymbol_small[i].width(),y,imgSymbol_small[i]);
                        }
                    }
                }
            break;
            case GameTextSize::font_size_meduim:
                for(int cnt = 0; cnt < text.size(); cnt++)    {
                    for(int i = 0; i < sSymbols.size(); i++)    {
                        if(text.at(cnt) == sSymbols.at(i))    {
                            p->drawImage(x + (cnt)*imgSymbol_medium[i].width(),y,imgSymbol_medium[i]);
                        }
                    }
                }
            break;
            case GameTextSize::font_size_large:
                for(int cnt = 0; cnt < text.size(); cnt++)    {
                    for(int i = 0; i < sSymbols.size(); i++)    {
                        if(text.at(cnt) == sSymbols.at(i))    {
                            p->drawImage(x + (cnt)*imgSymbol_big[i].width(),y,imgSymbol_big[i]);
                        }
                    }
                }
            break;
        }
    };

private:
    QString sSymbols;
    QImage imgSymbol_medium[136];
    QImage imgSymbol_small[136];
    QImage imgSymbol_micro[136];
    QImage imgSymbol_big[136];
    QImage FONT_IMAGE;
};

#endif // QGAMETEXT_H
