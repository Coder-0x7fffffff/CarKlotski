#include "gameview.h"

gameView::gameView()
{
    setScene(&scene);
    loadImg();
}

gameView::gameView(QWidget *&w)
{
    setScene(&scene);
    loadImg();
    QGraphicsView((QWidget*)w);
}

void gameView::loadImg()
{
    //加载资源
    g.loadMap();
    menu_background.load(":/img/image/menu_bg.png");
    menu_title.load(":/img/image/bg_mmenu.png");
    buttonPixmapStart.load(":/img/image/options_btn_HL_Start.png");
    buttonPixmapExit.load(":/img/image/options_btn_HL_Exit.png");
    buttonPixmapCrazy.load(":/img/image/options_btn_HL_Crazy.png");
    for(int i=0;i<levelCount;++i)
    {
        levelSelImg[i].load(":/img/image/levelSelection.png");
        QString text = QString::number(i+1);
        QFont font;
        font.setFamily("微软雅黑");
        font.setPixelSize((levelSelImg[i].size().width()+levelSelImg[i].size().height())/3);
        QFontMetrics fm(font);
        int bx = (levelSelImg[i].size().width() - fm.width(text))/2;
        int by = (levelSelImg[i].size().height() - fm.height())/2;
        QPainter paint(&levelSelImg[i]);
        paint.setFont(font);
        paint.setPen(Qt::white);
        paint.drawText(QRect(bx,by,fm.width(text),fm.height()),text);
    }
    gameBackground.load(":/img/image/menu_bg.png");
    gameMap.load(":/img/image/background.png");
    car12[0].load(":/img/image/car121.png");
    car12[1].load(":/img/image/car122.png");
    car12[2].load(":/img/image/car123.png");
    car12[3].load(":/img/image/car124.png");
    car12[4].load(":/img/image/car125.png");
    car13[0].load(":/img/image/car131.png");
    car13[1].load(":/img/image/car132.png");
    car21[0].load(":/img/image/car211.png");
    car21[1].load(":/img/image/car212.png");
    car21[2].load(":/img/image/car213.png");
    car21[3].load(":/img/image/car214.png");
    car21[4].load(":/img/image/car215.png");
    car31[0].load(":/img/image/car311.png");
    car31[1].load(":/img/image/car312.png");
    btnFram.load(":/img/image/buttonFram.png");
    buttonMainMenu.load(":/img/image/mainmenu.png");
    buttonRestart.load(":/img/image/restart.png");
    buttonHuckIt.load(":/img/image/huckit.png");
    cmpBack.load(":/img/image/menu_bg.png");
    cmpMenu.load(":/img/image/completeBack.png");
    QString text = "Good Job!";
    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(cmpMenu.width()/8);
    QFontMetrics fm(font);
    int bx = (cmpMenu.width() - fm.width(text))/2;
    int by = (cmpMenu.height() - fm.height())/2;
    QPainter paint(&cmpMenu);
    paint.setFont(font);
    paint.setPen(Qt::white);
    paint.drawText(QRect(bx,by,fm.width(text),fm.height()),text);
    cmpRestBut.load(":/img/image/resetLevel.png");
    cmpNextBut.load(":/img/image/nextLevel.png");
}

void gameView::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug()<<event->pos();
    switch(gameState)
    {
    case GS_MENU:
        {
            QGraphicsItem *clicked = itemAt(event->pos());
            if(clicked == button1)
            {
                drawLevelSelect();
            }
            else if(clicked == button2)
            {
                //加载游戏
                g.initCrazy();
                ifCrazy = true;
                drawGame();
            }
            else if(clicked == button3)
            {
                exit(0);
            }
        }
        break;
    case GS_LEVEL_SELECT:
        {
            QGraphicsItem *clicked = itemAt(event->pos());
            int i=0;
            for(i=0;i<levelCount;i++)//查找选中的关卡
            {
                if(levelSelection[i]==clicked)
                    break;
            }
            if(i<levelCount)//查找到关卡
            {
                //开始游戏
                gameLevel = i;
                //加载游戏
                g.initGame(gameLevel);
                drawGame();
            }
        }
        break;
    case GS_GAMING:
        {
            if(ifHuckIt && !ifCrazy)
            {
                std::vector<MapRecord> mr = g.getMapList();
                if(huckItStep==mr[gameLevel].steps.size())
                {
                    ifHuckIt=false;
                    //绘制结束界面
                    drawComplete();
                    break;
                }
                CheatStep cs = mr[gameLevel].steps[huckItStep];
                huckItStep++;
                int tcarNum = g.getCarNumber(cs.x,cs.y);
                switch(cs.direction)
                {
                case MOVE_UP:
                    carList[tcarNum].carInfo.y-=cs.step;
                    break;
                case MOVE_DOWN:
                    carList[tcarNum].carInfo.y+=cs.step;
                    break;
                case MOVE_LEFT:
                    carList[tcarNum].carInfo.x-=cs.step;
                    break;
                case MOVE_RIGHT:
                    carList[tcarNum].carInfo.x+=cs.step;
                    break;
                }
                g.move(cs.x,cs.y,cs.direction,cs.step);
                carList[tcarNum].pixmap->setPos(covPntFromLogicToPhysic(carList[tcarNum].carInfo.x,carList[tcarNum].carInfo.y));
            }
            QGraphicsItem *clicked = itemAt(event->pos());
            if(clicked==btnHuckIt)
            {
                //加载游戏
                g.initGame(gameLevel);
                drawGame();
                ifHuckIt=true;
                huckItStep=0;
            }
            else if(clicked==btnRestart)
            {
                //开始游戏
                //加载游戏
                if(ifCrazy)
                    g.initCrazy();
                else g.initGame(gameLevel);
                drawGame();
            }
            else if(clicked==btnMainMenu)
            {
                drawGameMenu();
            }
        }
        break;
    case GS_COMPLETE:
        {
            QGraphicsItem *clicked = itemAt(event->pos());
            if(clicked==completeResetBut)
            {
                //开始游戏
                //加载游戏
                g.initGame(gameLevel);
                drawGame();
            }
            else if(clicked==completeMainMenu)
            {
                drawGameMenu();
            }
            else if(clicked==completeNextBut)
            {
                //下一关

                if(!ifCrazy)
                {
                    gameLevel++;
                    //加载游戏
                    g.initGame(gameLevel);
                }
                else g.initCrazy();
                drawGame();
            }
        }
        break;
    }

}

void gameView::keyPressEvent(QKeyEvent *event)
{
    if(!isMoving)
    {
        switch(gameState)
        {
        case GS_GAMING:
            switch(event->key())
            {
            case Qt::Key_W:
                if(rectPos.y()>0)
                    rectPos.setY(rectPos.y()-1);
                break;
            case Qt::Key_S:
                if(rectPos.y()<MAPY-1)
                    rectPos.setY(rectPos.y()+1);
                break;
            case Qt::Key_A:
                if(rectPos.x()>0)
                    rectPos.setX(rectPos.x()-1);
                break;
            case Qt::Key_D:
                if(rectPos.x()<MAPX-1)
                    rectPos.setX(rectPos.x()+1);
                break;
            case Qt::Key_Space:
                {
                    int carNum = g.getCarNumber(rectPos.x(),rectPos.y());
                    if(carNum!=-1)
                    {
                        isMoving = true;
                        if(carList[carNum].carInfo.direction==DIRECTION_ROW)
                        {
                            selectRect->setPen(QPen(QBrush(QColor(0,0,255)),wideOfRect));
                            selectRect->setRect(dXOfRectX,dXOfRectY,carBlockWide * carList[carNum].carInfo.length,carBlockWide);
                        }
                        else
                        {
                            selectRect->setPen(QPen(QBrush(QColor(0,0,255)),wideOfRect));
                            selectRect->setRect(dXOfRectX,dXOfRectY,carBlockWide,carBlockWide * carList[carNum].carInfo.length);
                        }
                        movDir = 0;
                        movStep = 0;
                        rectPos.setX(carList[carNum].carInfo.x);
                        rectPos.setY(carList[carNum].carInfo.y);
                        movBeg = rectPos;
                        movCarNum = carNum;
                    }
                }
                break;
            }
            selectRect->setPos(covPntFromLogicToPhysic(rectPos.x(),rectPos.y()));
            break;
        }
    }
    else
    {
        switch(gameState)
        {
        case GS_GAMING:
            switch(event->key())
            {
            case Qt::Key_W:
                if(movDir == 0)
                    movDir = DIRECTION_COL;
                if(movDir == DIRECTION_COL)
                {
                    if(g.movable(movBeg.x(),movBeg.y(),movStep-1>0?MOVE_DOWN:MOVE_UP,abs(movStep-1)))
                    {
                        movStep--;
                        carList[movCarNum].carInfo.y--;
                        rectPos.setY(rectPos.y()-1);
                    }
                }
                break;
            case Qt::Key_S:
                if(movDir == 0)
                    movDir = DIRECTION_COL;
                if(movDir == DIRECTION_COL)
                {
                    if(g.movable(movBeg.x(),movBeg.y(),movStep+1>0?MOVE_DOWN:MOVE_UP,abs(movStep+1)))
                    {
                        movStep++;
                        carList[movCarNum].carInfo.y++;
                        rectPos.setY(rectPos.y()+1);
                    }
                }
                break;
            case Qt::Key_A:
                if(movDir == 0)
                    movDir = DIRECTION_ROW;
                if(movDir == DIRECTION_ROW)
                {
                    if(g.movable(movBeg.x(),movBeg.y(),movStep-1>0?MOVE_RIGHT:MOVE_LEFT,abs(movStep-1)))
                    {
                        movStep--;
                        carList[movCarNum].carInfo.x--;
                        rectPos.setX(rectPos.x()-1);
                    }
                }
                break;
            case Qt::Key_D:
                if(movDir == 0)
                    movDir = DIRECTION_ROW;
                if(movDir == DIRECTION_ROW)
                {
                    if(g.movable(movBeg.x(),movBeg.y(),movStep+1>0?MOVE_RIGHT:MOVE_LEFT,abs(movStep+1)))
                    {
                        movStep++;
                        carList[movCarNum].carInfo.x++;
                        rectPos.setX(rectPos.x()+1);
                    }
                }
                break;
            case Qt::Key_Space:
                if(movDir == DIRECTION_COL)
                    g.move(movBeg.x(),movBeg.y(),movStep>0?MOVE_DOWN:MOVE_UP,abs(movStep));
                else if(movDir == DIRECTION_ROW)
                    g.move(movBeg.x(),movBeg.y(),movStep>0?MOVE_RIGHT:MOVE_LEFT,abs(movStep));
                movDir = 0;
                movStep = 0;
                movBeg = QPoint(0,0);
                isMoving = false;
                selectRect->setPen(QPen(QBrush(QColor(255,0,0)),wideOfRect));
                selectRect->setRect(dXOfRectX,dXOfRectY,carBlockWide,carBlockWide);
                if(g.ifGameComplete())
                {
                    //绘制结束界面
                    drawComplete();
                }
                break;
            }
            break;
        }
        selectRect->setPos(covPntFromLogicToPhysic(rectPos.x(),rectPos.y()));
        carList[movCarNum].pixmap->setPos(covPntFromLogicToPhysic(carList[movCarNum].carInfo.x,carList[movCarNum].carInfo.y));
    }
}

void gameView::drawGameMenu()
{
    gameState = GS_MENU;
    int sceneWide;
    int sceneHeight;
    //设置大小
    resize(menu_background.size());
    scene.addPixmap(menu_background);
    sceneWide = menu_background.size().width();
    sceneHeight = menu_background.size().height();
    //加载背景
    bg = new QGraphicsPixmapItem();
    bgt = new QGraphicsPixmapItem();
    bg->setPixmap(menu_background);
    bgt->setPixmap(menu_title);
    bgt->setPos(0,menu_background.size().height()/8);
    scene.addItem(bg);
    scene.addItem(bgt);
    //加载按钮
    button1 = new QGraphicsPixmapItem();
    button2 = new QGraphicsPixmapItem();
    button3 = new QGraphicsPixmapItem();
    button1->setPixmap(buttonPixmapStart);
    button2->setPixmap(buttonPixmapCrazy);
    button3->setPixmap(buttonPixmapExit);
    //设置名称
    int buttonWide = button1->pixmap().size().width();
    int buttonHight = button1->pixmap().size().height();
    button1->setPos(sceneWide/2-buttonWide/2,sceneHeight*3/5-buttonHight/2);
    button2->setPos(sceneWide/2-buttonWide/2,sceneHeight*3/5-buttonHight/2 + buttonHight*1.2);
    button3->setPos(sceneWide/2-buttonWide/2,sceneHeight*3/5-buttonHight/2 + buttonHight*2.4);
    scene.addItem(button1);
    scene.addItem(button2);
    scene.addItem(button3);
}

void gameView::drawLevelSelect()
{
    gameState = GS_LEVEL_SELECT;
    int countEachRow = 4;
    //加载背景
    bg = new QGraphicsPixmapItem();
    bg->setPixmap(menu_background);
    scene.addItem(bg);
    //创建关卡对象
    for(int i=0;i<levelCount;++i)
    {
        levelSelection[i] = new QGraphicsPixmapItem();
        levelSelection[i]->setPixmap(levelSelImg[i]);
    }
    //绘制
    int countEachCol = levelCount/countEachRow;
    int blockWide = levelSelImg[0].size().width();
    int blockHeight = levelSelImg[0].size().height();
    int frameGap = blockWide/3;
    int frameWide = (size().width() - blockWide*countEachRow - frameGap*(countEachRow-1))/2;
    int frameHeight = (size().height()- blockHeight*countEachCol - frameGap*(countEachRow-1))/2;
    for(int i=0;i<countEachCol;++i)
    {
        for(int j=0;j<countEachRow;++j)
        {
            (*(levelSelection+i*countEachRow+j))->setPos(frameWide+j*blockWide+j*frameGap,frameHeight+i*blockHeight+i*frameGap);
            scene.addItem(*(levelSelection+i*countEachRow+j));
        }
    }
}

void gameView::drawGame()
{
    gameState = GS_GAMING;
    //绘制背景
    backPaint = new QGraphicsPixmapItem();
    background = new QGraphicsPixmapItem();
    backPaint->setPixmap(gameBackground);
    background->setPixmap(gameMap);
    scene.addItem(backPaint);
    mapBx = size().width()-gameMap.width();
    mapBy = size().height()/2-gameMap.height()/2;
    background->setPos(mapBx,mapBy);
    scene.addItem(background);
    //绘制按钮
    buttonFram = new QGraphicsPixmapItem();
    buttonFram->setPixmap(btnFram);
    buttonFram->setPos(backPaint->pixmap().size().width()/2-buttonFram->pixmap().size().width()/2,backPaint->pixmap().size().height()*4/5);
    scene.addItem(buttonFram);
    btnHuckIt = new QGraphicsPixmapItem();
    btnRestart = new QGraphicsPixmapItem();
    btnMainMenu = new QGraphicsPixmapItem();
    btnHuckIt->setPixmap(buttonHuckIt);
    btnRestart->setPixmap(buttonRestart);
    btnMainMenu->setPixmap(buttonMainMenu);
    btnHuckIt->setPos(backPaint->pixmap().size().width()/2-buttonFram->pixmap().size().width()/3-btnHuckIt->pixmap().size().width()/2,backPaint->pixmap().size().height()*4/5+buttonFram->pixmap().size().height()/5);
    btnRestart->setPos(backPaint->pixmap().size().width()/2-btnRestart->pixmap().size().width()/2,backPaint->pixmap().size().height()*4/5+buttonFram->pixmap().size().height()/5);
    btnMainMenu->setPos(backPaint->pixmap().size().width()/2+buttonFram->pixmap().size().width()/3-btnMainMenu->pixmap().size().width()/2,backPaint->pixmap().size().height()*4/5+buttonFram->pixmap().size().height()/5);
    if(!ifCrazy)
    {
        scene.addItem(btnHuckIt);
    }
    scene.addItem(btnRestart);
    scene.addItem(btnMainMenu);
    //绘制车辆
    std::vector<Car> cars = g.getCarList();
    carCount = cars.size();
    //构建车辆数组
    srand(time(0));
    for(int i=1;i<=cars.size();i++)
    {
        carList[i].carInfo = cars[i];
        carList[i].pixmap = new QGraphicsPixmapItem();
        if(cars[i].direction == DIRECTION_COL && cars[i].length == 2)//12
        {
            carList[i].pixmap->setPixmap(car12[rand()%ccar12]);
        }
        else if(cars[i].direction == DIRECTION_COL && cars[i].length == 3)//12
        {
            carList[i].pixmap->setPixmap(car13[rand()%ccar13]);
        }
        else if(cars[i].direction == DIRECTION_ROW && cars[i].length == 2)//21
        {
            carList[i].pixmap->setPixmap(car21[rand()%ccar21]);
        }
        else if(cars[i].direction == DIRECTION_ROW && cars[i].length == 3)//31
        {
            carList[i].pixmap->setPixmap(car31[rand()%ccar31]);
        }
        carList[i].pixmap->setPos(covPntFromLogicToPhysic(cars[i].x,cars[i].y));
        scene.addItem(carList[i].pixmap);
    }
    selectRect = new QGraphicsRectItem();
    selectRect->setRect(dXOfRectX,dXOfRectY,carBlockWide,carBlockWide);
    selectRect->setPen(QPen(QBrush(QColor(255,0,0)),wideOfRect));
    rectPos.setX(carList[1].carInfo.x);
    rectPos.setY(carList[1].carInfo.y);
    selectRect->setPos(covPntFromLogicToPhysic(rectPos.x(),rectPos.y()));
    scene.addItem(selectRect);
}

void gameView::drawComplete()
{
    gameState = GS_COMPLETE;
    completeBack = new QGraphicsPixmapItem();
    completeMenu = new QGraphicsPixmapItem();
    completeResetBut = new QGraphicsPixmapItem();
    completeMainMenu = new QGraphicsPixmapItem();
    completeNextBut = new QGraphicsPixmapItem();
    completeBack->setPixmap(cmpBack);
    completeMenu->setPixmap(cmpMenu);
    completeResetBut->setPixmap(cmpRestBut);
    completeMainMenu->setPixmap(buttonMainMenu);
    completeNextBut->setPixmap(cmpNextBut);
    completeMenu->setPos((completeBack->pixmap().size().width()-completeMenu->pixmap().size().width())/2,(completeBack->pixmap().size().height()-completeMenu->pixmap().size().height())/2);
    completeResetBut->setPos(completeBack->pixmap().size().width()*2/6 - completeResetBut->pixmap().size().width(),completeBack->pixmap().size().height()*3/5);
    completeMainMenu->setPos(completeBack->pixmap().size().width()*3/6 - completeMainMenu->pixmap().size().width()/2,completeBack->pixmap().size().height()*3/5);
    completeNextBut->setPos(completeBack->pixmap().size().width()*4/6,completeBack->pixmap().size().height()*3/5);
    scene.addItem(completeBack);
    scene.addItem(completeMenu);
    if(!ifCrazy)
        scene.addItem(completeResetBut);
    scene.addItem(completeMainMenu);
    if(gameLevel<levelCount-1 || ifCrazy)
        scene.addItem(completeNextBut);
}

QPointF gameView::covPntFromLogicToPhysic(int x,int y)
{
    int px,py;
    px = carMapBegX;
    py = carMapBegY;
    int blockWide = carBlockWide;
    px += x*blockWide;
    py += y*blockWide;
    QPointF pnt;
    pnt.setX(px);
    pnt.setY(py);
    return pnt;
}

QPoint gameView::covPntFromPhysicToLogic(int x,int y)
{
    int lx,ly;
    x-=42;
    y-=230;
    QPoint pnt;
    pnt.setX(x/66);
    pnt.setY(y/66);
    return pnt;
}
