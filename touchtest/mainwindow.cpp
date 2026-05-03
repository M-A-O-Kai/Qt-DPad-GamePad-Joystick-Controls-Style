#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_AcceptTouchEvents);
    this->installEventFilter(this); //this
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::TouchBegin:
            qDebug() << "Touch In";
            break;

        case QEvent::TouchEnd:
            qDebug() << "Touch End";
            break;

        case QEvent::MouseButtonRelease:
            qDebug() << "Mouse Release fallback";
            break;

        case QEvent::TouchCancel:
            qDebug() << "Touch Cancel (rare)";
            break;

        default:
            break;
    }

    return false;
}
