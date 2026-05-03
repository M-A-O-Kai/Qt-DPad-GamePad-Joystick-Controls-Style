






#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTouchEvent>

#include <QMainWindow>
#include <windows.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateMovement();
    void setsty();

    void contructDot();
    void updateDot(bool dot);
    void updateDot();
public slots:
    void onPhysicalUpPressed();
    void onPhysicalDownReleased();
    void onPhysicalDownPressed();
    void onPhysicalLeftReleased();
    void onPhysicalLeftPressed();
    void onPhysicalRightReleased();
    void onPhysicalRightPressed();
    void onPhysicalUpReleased();

protected:
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event);
   // void keyPressEvent(QKeyEvent *event) override;
   //void keyReleaseEvent(QKeyEvent *event) override;
    void mouseHoldingSwitch(QMouseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    bool processOverlapClick_eventFilter(QObject *obj, QEvent *event);
   // void processTouchAt(QPoint globalPos);
    bool tevent (QEvent *event);
    bool eventtch(QEvent *event);
    bool touchDraggingFunction_eventFilter(QObject *obj, QEvent *event);
    void handleTouch(QTouchEvent *te);
    bool isTouchActive(QTouchEvent *te);
private slots:
    void on_pushButton_pressed();
    //void repeatup();
    void on_pushButton_released();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

    void on_pushButton_4_pressed();

    void on_pushButton_4_released();

    void on_pushButton_3_pressed();

    void on_pushButton_3_released();
    //void processOverlapClick_eventFilter();


    void on_pushButton_5_pressed();

    void on_pushButton_5_released();

private:
    QTimer *keyTimer;
    Ui::MainWindow *ui;
    void pressKey(WORD key);
    void releaseKey(WORD key);
    bool upHold = false;
    bool downHold = false;
    bool leftHold = false;
    bool rightHold = false;
    bool dragging = false;
    QPoint dragPosition;
    void on_any_released();
    void on_updownleftright_pressed(int key);
    void pressKey2(int key);
    void releaseKey2(int key);
    void switchKey(WORD newKey);
};

#endif // MAINWINDOW_H

