#include "insert3.h"
#include "ui_insert3.h"
#include "QIcon"
#include "QMessageBox"
#include <QString>
#include <QPainter>
#include <QFileDialog>
#include <QtDebug>

insert3::insert3(Graph *graph, Function *fun, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::insert3),
    graph(graph),
    fun(fun)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 800);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));
    setWindowTitle("套汇程序");
    ui->lineEdit->clear();

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));

    connect(ui->pushButton_2, &QPushButton::clicked, this, &insert3::handleButtonClicked);

    connect(graph, &Graph::fileOpenError, this, [this]() {
        ui->lineEdit->clear();
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
    });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){
        this->hide();
        this->fun->show();
    });

}

insert3::~insert3()
{
    delete ui;
}

void insert3::handleButtonClicked() {
    QString text = ui->lineEdit->text();
    if (text.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入正确文件的地址。");
    }
    else
    {
        std::string filename = text.toStdString();
        if (graph != nullptr)
        {  // 确保graph对象存在
            graph->ReadRatesFromFile(filename, *graph, this);
            QMessageBox::information(this, "祝贺", "添加成功");
        }
        else
        {
            QMessageBox::critical(this, "错误", "无法成功导入");
        }
    }
}

void insert3::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/background.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void insert3::setGraph(Graph* newGraph) {
    graph = newGraph;
}

void insert3::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("请选择文件"));
    qDebug()<<"open dialog";
    if (!filename.isEmpty())
    {
        ui->lineEdit->setText(filename);
    }
}
