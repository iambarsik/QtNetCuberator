#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QGLWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setAttribute((Qt::WA_AcceptTouchEvents));

        // get available screens and scale widget
    QScreen* screen = QApplication::screens().at(0);
    QSize size = screen->availableSize();
    //this->setGeometry(0,0,size.width(),size.height());
    this->setGeometry(50,50,40*TILE_SIZE,20*TILE_SIZE);

    GameField = QRect(0,0,SCREEN_TILE_WIDTH*TILE_SIZE,SCREEN_TILE_HEIGHT*TILE_SIZE);

    #ifdef Q_OS_WINDOWS
        //this->setWindowFlags(Qt::FramelessWindowHint);
    #endif

    Resolution.setX(GameField.width());
    Resolution.setY(GameField.height());

        // rotation flag for mobile devices and calculating aspect ration
    bRotateScreen = this->width() > this->height() ? false : true;
    if(bRotateScreen == false)  {
        dX = (double)(size.width())/(double)Resolution.x();
        dY = (double)(size.height())/(double)Resolution.y();
    } else {
        dX = (double)size.width()/(double)Resolution.y();
        dY = (double)(size.height())/(double)Resolution.x();
    }

        // Определяем соотношение экрана, чтоб знать, относительно чего растягивать канву
    bStretchHorizontal = this->width()/this->height() < SCREEN_TILE_WIDTH/SCREEN_TILE_HEIGHT ? true : false;
    if(bRotateScreen == false)  {
        if(bStretchHorizontal == true) {
            fImageZoom = (double)this->width()/(double)Resolution.x();
            fImageX = 0;
            fImageY = (this->height() - ((double)Resolution.y()*fImageZoom))/(2*fImageZoom);
        } else {
            fImageZoom = (double)this->height()/(double)Resolution.y();
            fImageX = (this->width() - ((double)Resolution.x()*fImageZoom))/(2*fImageZoom);
            fImageY = 0;
        }
    } else {
        if(bStretchHorizontal == true) {
            fImageZoom = (double)this->width()/((double)Resolution.y());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        } else {
            fImageZoom = (double)this->height()/((double)Resolution.x());
            fImageX = 0;   // доработать
            fImageY = 0;   // доработать
        }
    }

        // init buttons
    QImage imON;
    QImage imOFF;
    imON.load(":/images/button_on.png");
    imOFF.load(":/images/button_off.png");
    for(int i = 0; i < BUTTON_COUNT; i++)  {
        TouchKeys[i] = new TouchButton(QRect(
                TouchButtonGeometry[i][0]*TILE_SIZE,
                TouchButtonGeometry[i][1]*TILE_SIZE,
                TouchButtonGeometry[i][2]*TILE_SIZE,
                TouchButtonGeometry[i][3]*TILE_SIZE
                ),
                TouchButtonGeometry[i][4]);
        TouchKeys[i]->SetImage(imON,imOFF);
        myKey[i] = false;
    }

    game = new game_main_class(TILE_SIZE, GLOBAL_G);

    SetStartValues();

    iForGame = 0;
    iForAnimation = 0;
    GameLoop = new QTimer;
    GameLoop->setTimerType(Qt::PreciseTimer);
    connect(GameLoop, SIGNAL(timeout()), SLOT(GamePlayFunction()));
    GameLoop->setInterval(0);
    GameLoop->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::GamePlayFunction() {

    QApplication::processEvents();
    QTime currentTime= QTime::currentTime();
    int msCurrent = currentTime.msecsSinceStartOfDay();

    if(msCurrent - iForGame > ON_GAME_PERIOD) {
        OnGame();
        iForGame = msCurrent;
    }
    if(msCurrent - iForAnimation > ON_ANIMATION_PERIOD) {
        OnAnimation();
        iForAnimation = msCurrent;
    }
}

void Widget::paintEvent(QPaintEvent *event) {

        // init buffer image
    QImage img(Resolution.x(),Resolution.y(),QImage::Format_RGB32);
    QPainter qScreen;
    qScreen.begin(&img);
    qScreen.setRenderHint(QPainter::Antialiasing,false);

    setColor(Qt::black,Qt::black, Qt::SolidPattern, &qScreen);
    qScreen.drawRect(0,0,Resolution.x(),Resolution.y());

        // ====================================== start user drawing ===============================
        // before zooming

    // draw something

        // zooming
    qScreen.save();
    //qScreen.scale(Camera.Zoom,Camera.Zoom);

        // draw something

    game->render(&qScreen);

        // after zooming
    qScreen.restore();
        // after zooming

    // draw something




        // ====================================== end user drawing =================================


        // Рисуем сенсорные кнопки, если экран перевёрнут
    if(bRotateScreen)   {
        for(int i = 0; i < BUTTON_COUNT; i++) {
            if(TouchKeys[i]->Enable) TouchKeys[i]->Draw(&qScreen);
            if(Settings.bDevMode)    {
                if(TouchKeys[i]->Value) {

                }
            }
        }
    }

    qScreen.end();

        // transposing buffer image to main widget
    QPainter p;
    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing,false);
    setColor(Qt::black,Qt::black, Qt::SolidPattern, &p);
    p.drawRect(0,0,this->width(),this->height());
    if(bRotateScreen == false)   {
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(fImageX,fImageY,img);
    } else {
        p.save();
        p.translate(this->width(),0);
        p.rotate(90);
        p.scale(fImageZoom,fImageZoom);
        p.drawImage(0,0,img);
        p.restore();
    }
    p.end();


    QWidget::paintEvent(event);


}
