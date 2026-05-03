#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "QSystemTrayIcon"
#include "QMenu"
#include "QTimer"
#include "QDebug"
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>


//------------------------------SHOW PHYSICAL KEYBOARD PRESSES---------------------------------------------------

HHOOK keyboardHook;
MainWindow *globalWindow = nullptr;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT *)lParam;

        if (globalWindow)
        {
            if (kbd->vkCode == VK_UP){
                if (wParam == WM_KEYDOWN)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalUpPressed");
                else if (wParam == WM_KEYUP)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalUpReleased");
            }
            if (kbd->vkCode == VK_DOWN){
                if (wParam == WM_KEYDOWN)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalDownPressed");
                else if (wParam == WM_KEYUP)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalDownReleased");
            }
            if (kbd->vkCode == VK_LEFT){
                if (wParam == WM_KEYDOWN)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalLeftPressed");
                else if (wParam == WM_KEYUP)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalLeftReleased");
            }
            if (kbd->vkCode == VK_RIGHT){
                if (wParam == WM_KEYDOWN)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalRightPressed");
                else if (wParam == WM_KEYUP)
                    QMetaObject::invokeMethod(globalWindow, "onPhysicalRightReleased");
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}
void MainWindow::onPhysicalUpPressed(){ ui->pushButton->setStyleSheet("background: yellow;");}
void MainWindow::onPhysicalDownPressed(){ ui->pushButton_2->setStyleSheet("background: yellow;");}
void MainWindow::onPhysicalLeftPressed(){ ui->pushButton_4->setStyleSheet("background: yellow;");}
void MainWindow::onPhysicalRightPressed(){ ui->pushButton_3->setStyleSheet("background: yellow;");}

void MainWindow::onPhysicalUpReleased(){ ui->pushButton->setStyleSheet(""); }
void MainWindow::onPhysicalDownReleased(){ ui->pushButton_2->setStyleSheet(""); }
void MainWindow::onPhysicalLeftReleased(){ ui->pushButton_4->setStyleSheet(""); }
void MainWindow::onPhysicalRightReleased(){ ui->pushButton_3->setStyleSheet(""); }

//----------------------------SHOW PHYSICAL KEYBOARD PRESSES----------------------------------------------





















//-------------------------------CONSTRUCTOR--------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_ShowWithoutActivating);
    this->setStyleSheet(R"(   QMainWindow { background-color: rgba(20, 20, 20, 0);  } )");

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    QMenu *trayMenu = new QMenu(this);

    QAction *showAction = new QAction("Show", this);
    QAction *hideAction = new QAction("Hide", this);
    QAction *exitAction = new QAction("Exit", this);

    connect(showAction, &QAction::triggered, this, &QWidget::show);
    connect(hideAction, &QAction::triggered, this, &QWidget::hide);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayMenu->addAction(showAction);
    trayMenu->addAction(hideAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();


    keyTimer = new QTimer(this);
    keyTimer->setInterval(16);

    //connect(keyTimer, &QTimer::timeout, this, &MainWindow::repeatup);
    //connect(ui->pushButton,&QPushButton::pressed, this, &MainWindow::on_pushButton_pressed);
    //connect(ui->pushButton,&QPushButton::released, this, &MainWindow::on_pushButton_released);

    //connect(keyTimer, &QTimer::timeout, this, &MainWindow::updateMovement);
    //keyTimer->start();


    globalWindow = this;
    keyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, KeyboardProc, NULL,0); //SHOW PHYSICAL KEYBOARD PRESSES




    setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this); //this

    ui->pushButton->installEventFilter(this);
    ui->pushButton_2->installEventFilter(this);
    ui->pushButton_3->installEventFilter(this);
    ui->pushButton_4->installEventFilter(this);
    ui->pushButton_5->installEventFilter(this);

    MainWindow::setsty();
    MainWindow::contructDot();
this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);



}
//---------------------------CONTRUCTOR----------------------------------------




























//-----------------------MOUSE DRAGGING EVENTS----------------------------------------
bool mouseHeld = false;QPoint lastGlobalPos;

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
        //mouseHeld = true;qDebug() << "HELDTRUE";
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event2)
{
    lastGlobalPos = event2->globalPos()- dragPosition;
    if (dragging && (event2->buttons() & Qt::LeftButton)) {
        this->move(event2->globalPos() - dragPosition);
        event2->accept();
    }
   // if( mouseHeld)    MainWindow::mouseHoldingSwitch(event2);
}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    dragging = false;
    //mouseHeld = false; qDebug() << "HELDFALSE";
}


MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::updateMovement(){
    if(upHold) pressKey(VK_UP);
    if(downHold) pressKey(VK_DOWN);
    if(leftHold) pressKey(VK_LEFT);
    if(rightHold) pressKey(VK_RIGHT);

}
*/

void MainWindow::setsty(){
    QString style = R"(
        QPushButton {
            background-color: transparent;
            border: 2px solid white;
            color: white;
            border-radius: 6px;
        }

        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.08);
        }

        QPushButton:pressed {
            background-color: rgba(255, 255, 255, 0.15);
        }

        QPushButton:focus {
            outline: none;
            background-color: transparent;
        }
    )";

    ui->pushButton->setStyleSheet(style);
    ui->pushButton_2->setStyleSheet(style);
    ui->pushButton_3->setStyleSheet(style);
    ui->pushButton_4->setStyleSheet(style);

}


//------------------MOUSE DRAGGING EVENTS------------------------------------------------------



//-----------------------KEY PRESS EVENTS AND FUCTIONS-------------------------------------
    void MainWindow::pressKey(WORD key){
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
        //input.ki.dwFlags = 0;
        SendInput(1, &input, sizeof(INPUT));
        MainWindow::setsty();

    }

    void MainWindow::releaseKey(WORD key){
        INPUT input = {0};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        //input.ki.dwFlags = KEYEVENTF_KEYUP;
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT)); MainWindow::setsty();
}

void MainWindow::showEvent(QShowEvent *event){
    QMainWindow::showEvent(event);
    HWND hwnd = (HWND)this->winId();

    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE| WS_EX_TOOLWINDOW);
}



void MainWindow::on_pushButton_pressed()
{
    //qDebug() << "Pressed"; keyTimer->start(); upHold = true;

    //MainWindow::pressKey(VK_UP);
    //MainWindow::on_updownleftright_pressed(VK_UP);
}

void MainWindow::on_pushButton_released()
{
    //keyTimer->stop();
   //releaseKey(VK_UP);
    //upHold = false;
   //MainWindow::releaseKey(VK_UP);
    //MainWindow::on_any_released();
}

void MainWindow::on_pushButton_2_pressed()
{
   // downHold = true;
   // MainWindow::pressKey(VK_DOWN);
    //MainWindow::on_updownleftright_pressed(VK_DOWN);

}

void MainWindow::on_pushButton_2_released()
{
    //downHold = false;
   // MainWindow::releaseKey(VK_DOWN);
  //  MainWindow::on_any_released();
}

void MainWindow::on_pushButton_4_pressed()
{
   // leftHold = true;
   // MainWindow::pressKey(VK_LEFT);
    //MainWindow::on_updownleftright_pressed(VK_LEFT);
}

void MainWindow::on_pushButton_4_released()
{
    //leftHold = false;
   // MainWindow::releaseKey(VK_LEFT);
   // MainWindow::on_any_released();
}

void MainWindow::on_pushButton_3_pressed()
{
    //rightHold = true;
    // MainWindow::pressKey(VK_RIGHT);
    //MainWindow::on_updownleftright_pressed(VK_RIGHT);


}

void MainWindow::on_pushButton_3_released()
{
    //rightHold = false;
    //MainWindow::releaseKey(VK_RIGHT);MainWindow::on_any_released();
}


void MainWindow::on_pushButton_5_pressed()
{
    //MainWindow::pressKey(VK_UP);
}

void MainWindow::on_pushButton_5_released()
{
   // MainWindow::releaseKey(VK_UP);MainWindow::on_any_released();
}


//---------------------------KEY PRESS EVENTS AND FUNCTIONS--------------------------------





//----------------------------MOUSE & KEY SWITCHING FUNCTION--------------------------------------------
WORD currentKey = 0; QSet<WORD> activeKeys;QSet<QString> previousHits;

void MainWindow::switchKey(WORD newKey)
{
    /*
    if (newKey == 0) return;
    if (activeKeys.contains(newKey))
        return;
    if (activeKeys.size() >= 4){
        WORD first = *activeKeys.begin(); // oldest (approx)
        releaseKey(first);
        activeKeys.remove(first);
    }

    activeKeys.insert(newKey);
   // pressKey(newKey);
    */
}

void MainWindow::on_any_released()
{/*

    for (WORD key : activeKeys) {
        releaseKey(key);
    }

    activeKeys.clear();*/
}






bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //   MainWindow::tevent(event);
    MainWindow::touchDraggingFunction_eventFilter(obj, event);


       processOverlapClick_eventFilter(obj, event);




    if (event->type() == QEvent::MouseButtonPress)
    {
        mouseHeld = true;
        if (obj == ui->pushButton)
            switchKey(VK_UP);
        else if (obj == ui->pushButton_4)
            switchKey(VK_LEFT);
        else if (obj == ui->pushButton_3)
            switchKey(VK_RIGHT);
        else if (obj == ui->pushButton_2)
            switchKey(VK_DOWN);
        else if (obj == ui->pushButton_5)
              switchKey(VK_UP);
        return true;
    }

    if (event->type() == QEvent::MouseMove && mouseHeld)
    {
        QPoint globalPos = QCursor::pos();
        QWidget *w = QApplication::widgetAt(globalPos);
        if (!w) return false;
        if (w == ui->pushButton)
            switchKey(VK_UP);
        else if (w == ui->pushButton_4)
            switchKey(VK_LEFT);
        else if (w == ui->pushButton_3)
            switchKey(VK_RIGHT);
        else if (w == ui->pushButton_2)
            switchKey(VK_DOWN);
        else if (w == ui->pushButton_5)
                    switchKey(VK_UP);
        return true;
    }

    if (event->type() == QEvent::MouseButtonRelease){
        mouseHeld = false;
        on_any_released();
        return true;
    }

    return QMainWindow::eventFilter(obj, event);


}
//----------------------------MOUSE & KEY SWITCHING FUNCTION--------------------------------------------







































//----------------------------TOUCH DRAGGING--------------------------
#include "QLabel"
QLabel *debugDot = nullptr;
 QPoint localPos;   QPoint globalPos;QPoint farAway(100000, 100000);
 bool dotIsActive = false;

    QTouchEvent *te;QPoint globalPosTouch; QPointF globalCurrentDragTrackingTouchPos;
bool MainWindow::touchDraggingFunction_eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate)
    {
        te = static_cast<QTouchEvent*>(event);
        if (te->touchPoints().isEmpty())
            return false;

         const auto touchPoints = te->touchPoints();
         for (const QTouchEvent::TouchPoint &point : touchPoints)
         {
             globalPos = point.screenPos().toPoint();
             globalPosTouch = point.screenPos().toPoint();
             QPointF pos = point.pos();              // current position while dragging
             globalCurrentDragTrackingTouchPos = pos;
         }
         te->accept();


       // lastGlobalPos = globalPos;
        QWidget *w = QApplication::widgetAt(globalPosTouch);


        if (w == ui->pushButton ||  w == ui->pushButton_2 || w == ui->pushButton_3 ||w == ui->pushButton_4 ||w == ui->pushButton_5)
        {
           dragging = false;

            if (w == ui->pushButton)       switchKey(VK_UP);
            else if (w == ui->pushButton_4) switchKey(VK_LEFT);
            else if (w == ui->pushButton_3) switchKey(VK_RIGHT);
            else if (w == ui->pushButton_2) switchKey(VK_DOWN);
            else if (w == ui->pushButton_5) switchKey(VK_UP);

            return true; // we handled input
        }


        dragging = true;
        dragPosition =  globalPosTouch - frameGeometry().topLeft();


        if (dragging)  {
            move(globalPosTouch - dragPosition);
            //move(lastGlobalPos);
        }

        return true;
    }

    if (event->type() == QEvent::TouchEnd)
    {
        dragging = false;
        on_any_released();
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}













































//-------------------------------TOUCH DRAGGING-------------------------------

//---------------------OVERLAP------------------------------

#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <QLabel>


bool MainWindow::processOverlapClick_eventFilter(QObject *obj, QEvent *event)
{



    globalPos = QCursor::pos();
       localPos = this->mapFromGlobal(globalPos); //allows drag, but delayed taps
    QSet<QString> currentHits;

     if (event->type() == QEvent::MouseMove || event->type() == QEvent::TouchBegin)
     {
        if (event->type() == QEvent::TouchBegin){
            localPos = this->mapFromGlobal(globalPosTouch); //allows Taps, no drags
            globalPos = globalPosTouch;


        }
        dotIsActive=true; updateDot();



        debugDot->move(localPos);


        QRect upRect    = ui->pushButton->rect().translated(ui->pushButton->mapToGlobal(QPoint(0,0)));
        QRect leftRect  = ui->pushButton_4->rect().translated(ui->pushButton_4->mapToGlobal(QPoint(0,0)));
        QRect downRect  = ui->pushButton_2->rect().translated(ui->pushButton_2->mapToGlobal(QPoint(0,0)));
        QRect rightRect = ui->pushButton_3->rect().translated(ui->pushButton_3->mapToGlobal(QPoint(0,0)));
        QRect upRect2    = ui->pushButton_5->rect().translated(ui->pushButton_5->mapToGlobal(QPoint(0,0)));
        QList<QString> hits;

        if (upRect.contains(globalPos))
            currentHits.insert("UP");

        if (leftRect.contains(globalPos))
            currentHits.insert("LEFT");

        if (rightRect.contains(globalPos))
            currentHits.insert("RIGHT");

        if (downRect.contains(globalPos))
            currentHits.insert("DOWN");

        if (upRect2.contains(globalPos))
            currentHits.insert("UP");

        qDebug() << "Move:" << globalPos << "Hits:" << currentHits;

        QSet<QString> entered = currentHits - previousHits;
        QSet<QString> exited  = previousHits - currentHits;

        for (const QString &k : exited)
        {

            if (k == "UP") releaseKey(VK_UP);
            if (k == "LEFT") releaseKey(VK_LEFT);
            if (k == "RIGHT") releaseKey(VK_RIGHT);
            if (k == "DOWN") releaseKey(VK_DOWN);
             /**/
        }

        if(dotIsActive){
            for (const QString &k : entered)
            {

                if (k == "UP") pressKey(VK_UP);
                if (k == "LEFT") pressKey(VK_LEFT);
                if (k == "RIGHT") pressKey(VK_RIGHT);
                if (k == "DOWN") pressKey(VK_DOWN);
              /*    */
               // pressKey();
            }
            previousHits = currentHits;
        }else{
             releaseKey(VK_UP);
        }

    }

     if (event->type() ==   QEvent::MouseButtonRelease ){
       dotIsActive = false; updateDot();

           if (!previousHits.isEmpty())
           {
               for (const QString &k : previousHits)
               {
                   if (k == "UP") releaseKey(VK_UP);
                   if (k == "LEFT") releaseKey(VK_LEFT);
                   if (k == "RIGHT") releaseKey(VK_RIGHT);
                   if (k == "DOWN") releaseKey(VK_DOWN);
               }
               previousHits.clear();
           }


         qDebug() << "Mouse Release fallback";

     }




 return true;

}



void MainWindow::contructDot(){
    debugDot = new QLabel(this);
    debugDot->setFixedSize(10, 10);
     debugDot->setProperty("active", dotIsActive);
    debugDot->setStyleSheet(R"(
                            QWidget[active="true"] { background: yellow; border-radius:5px; }
                            QWidget[active="false"] { background: red; border-radius:5px; }
                        )");
    debugDot->setAttribute(Qt::WA_TransparentForMouseEvents);
    debugDot->show();
}

void MainWindow::updateDot(){
     debugDot->setProperty("active", dotIsActive);
    debugDot->setStyleSheet(R"(
                            QWidget[active="true"] { background: yellow; border-radius:5px; }
                            QWidget[active="false"] { background: red; border-radius:5px; }
                        )");

}
//----------------------------OVERLAP-------------------------------


