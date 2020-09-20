#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //风格设置
    view.setAlignment(Qt::AlignLeft|Qt::AlignTop);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //绑定
    view.setParent(this);
    view.show();
    //绘制图片
    view.drawGameMenu();
    this->resize(view.size().width(),view.size().height());
}

MainWindow::~MainWindow()
{
    delete ui;
}
