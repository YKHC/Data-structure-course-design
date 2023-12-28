#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Graph.cpp"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("套汇程序");
    this->setFixedSize(1000, 800);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));
    graph = new Graph();
    graph->Action = 0;
    function = new Function(graph, this);
    connect(this, &MainWindow::graphChanged, function, &Function::updateGraphReference);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));

    /*第一种输入方式*/
    this->insert1Window = new insert1(graph, function, this);
    connect(ui->pushButton_2, &QPushButton::clicked, this, [=](){
        this->hide();
        this->insert1Window->show();
    });

    /*第二种输入方式*/
    this->insert2Window = new insert2(graph, function, this);
    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        this->hide();
        this->insert2Window->show();
    });

    /*第三种输入方式*/
    this->insert3Window = new insert3(graph, function, this);
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){
        this->hide();
        this->insert3Window->show();
    });

    /*第一种输入返回*/
    connect(this->insert1Window, &insert1::back, this, [=]{
        this->insert1Window->hide();
        this->show();
    });

    /*第二种输入返回*/
    connect(this->insert2Window, &insert2::back, this, [=]{
        this->insert2Window->hide();
        this->show();
    });

    /*第三种输入返回*/
    connect(this->insert3Window, &insert3::back, this, [=]{
        this->insert3Window->hide();
        this->show();
    });

    connect(this->function, &Function::back, this, [=]{
        this->function->hide();
        this->show();
    });
    connect(this->function, &Function::back, this, [=]() {
        qDebug() << "Back signal received";
        this->function->hide();
        this->show();
        graph->resetGraph();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph;     // 释放 graph 资源
    delete function;  // 释放 function 资源
}

void MainWindow::ReplaceGraph(Graph *newGraph) {
    qDebug() << "ReplaceGraph called with newGraph";

    if (newGraph == nullptr) {
        qDebug() << "Received newGraph is nullptr";
        return;
    }

    if (graph != newGraph) {
        qDebug() << "Deleting old graph and setting new graph";
        emit graphChanged(newGraph, graph);
        graph = newGraph;
        // 调用 function 的公共方法来更新 UI
        function->setGraph(newGraph);
        insert1Window->setGraph(newGraph);
        insert2Window->setGraph(newGraph);
        insert3Window->setGraph(newGraph);
        qDebug() << "Graph replaced successfully";
    } else {
        qDebug() << "New graph is the same as the old graph";
    }
}



void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/background.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "pushButton clicked, emitting back signal";
    emit back();
}
