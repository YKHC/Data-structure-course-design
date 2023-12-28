#include "insert1.h"
#include "ui_insert1.h"
#include "QIcon"
#include "QMessageBox"
#include <QString>
#include <qdebug.h>
#include <QPainter>

insert1::insert1(Graph *graph, Function *fun, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::insert1),
    graph(graph),
    fun(fun)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 800);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));
    setWindowTitle("套汇程序");

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));

    connect(ui->pushButton_2, &QPushButton::clicked, this, [=](){
        if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty())
        {
            QMessageBox::warning(this, "错误", "请重新输入");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
        }
        else
        {
            graph->addRate(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString(), ui->lineEdit_3->text().toDouble());
            QMessageBox::information(this, "祝贺", "添加成功");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
        }
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        this->hide();
        this->fun->show();
    });
}

insert1::~insert1()
{
    delete ui;
}

void insert1::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/background.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void insert1::setGraph(Graph* newGraph) {
    graph = newGraph;
}
