#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMessageBox>
#include <QMouseEvent>
#include "game.h"
#include <QDebug>                   //debug.................

#define GS_MENU 0
#define GS_LEVEL_SELECT 1
#define GS_GAMING 2
#define GS_COMPLETE 3

const int carBlockWide = 66;
const int carMapBegX = 42;
const int carMapBegY = 230;
const int dXOfRectX = 0;
const int dXOfRectY = -5;
const int wideOfRect = 3;
const int levelCount = 8;
const int maxCarCount = 9;
const int ccar12 = 5;
const int ccar13 = 2;
const int ccar21 = 5;
const int ccar31 = 2;

struct CarPixmap
{
    Car carInfo;
    QGraphicsPixmapItem *pixmap;
};

class gameView : public QGraphicsView
{
public:
    gameView();
    gameView(QWidget *&w);
    void loadImg();
    //绘制界面
    void drawGameMenu();
    void drawLevelSelect();
    void drawGame();
    void drawComplete();
    //数据处理
    QPointF covPntFromLogicToPhysic(int x,int y);
    QPoint covPntFromPhysicToLogic(int x,int y);
    //操作响应
    void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QGraphicsScene scene;
    int gameState;
    //GS_MENU
    QGraphicsPixmapItem *bg,*bgt;
    QGraphicsPixmapItem *button1;
    QGraphicsPixmapItem *button2;
    QGraphicsPixmapItem *button3;
    //GS_LEVEL_SELECT
    QGraphicsPixmapItem *levelSelection[levelCount];
    //GS_GAMING
    bool ifCrazy;
    bool ifHuckIt;
    int huckItStep;
    QPoint movBeg;
    int movStep;
    int movDir;
    int movCarNum;
    bool isMoving;
    int carCount;
    QGraphicsRectItem *selectRect;
    QPoint rectPos;
    game g;
    int gameLevel;
    int mapBx,mapBy;
    const int blockBx = 44, blockBy = 229, blockWidth = 65, blockHeight = 65;
    QGraphicsPixmapItem *backPaint,*background;
    QGraphicsPixmapItem *mainCar;
    QGraphicsPixmapItem *buttonFram;
    QGraphicsPixmapItem *btnHuckIt,*btnRestart,*btnMainMenu;
    CarPixmap carList[maxCarCount];
    //GS_COMPLETE
    QGraphicsPixmapItem *completeBack;
    QGraphicsPixmapItem *completeMenu;
    QGraphicsPixmapItem *completeResetBut;
    QGraphicsPixmapItem *completeMainMenu;
    QGraphicsPixmapItem *completeNextBut;
    //图片资源
    QPixmap menu_background,menu_title;//标题
    QPixmap buttonPixmapStart,buttonPixmapCrazy,buttonPixmapExit;//按钮
    QPixmap levelSelImg[levelCount];//关卡选择
    QPixmap gameBackground,gameMap;//游戏
    QPixmap car12[ccar12];
    QPixmap car13[ccar13];
    QPixmap car21[ccar21];
    QPixmap car31[ccar31];
    QPixmap carTarget;
    QPixmap btnFram;
    QPixmap buttonMainMenu;
    QPixmap buttonRestart;
    QPixmap buttonHuckIt;
    QPixmap cmpBack;
    QPixmap cmpMenu;
    QPixmap cmpRestBut;
    QPixmap cmpNextBut;
};

#endif // GAMEVIEW_H
