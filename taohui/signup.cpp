#include "signup.h"
#include "ui_signup.h"
#include "logon.h"
#include "logon.cpp"

signup::signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signup)
{
    ui->setupUi(this);
    setWindowTitle("套汇程序");
    this->setFixedSize(1100, 660);
    this->setWindowIcon(QIcon("E:/keshe/taohui/1.png"));

    QPixmap *pix = new QPixmap("E:/keshe/taohui/1.png");
    QSize sz = ui->label->size();
    ui->label->setPixmap(pix->scaled(sz));
}

signup::~signup()
{
    delete ui;
}

void signup::on_pushButton_2_clicked()
{
    Logon *w = new Logon;
    w->show();
    this->close();
}

void signup::on_pushButton_clicked()
{
    QString username = ui->lineEdit_3->text();
    QString password = ui->lineEdit->text();
    QString surepass = ui->lineEdit_2->text();
    //判断密码是否一致
    if(password == surepass)
    {
        QString sql=QString("insert into user(username,password) values('%1','%2');")
                .arg(username).arg(password);
        //创建执行语句对象
        QSqlQuery query;
        //判断执行结果
        if(!query.exec(sql))
        {
            qDebug()<<"insert into error";
            QMessageBox::information(this,"注册认证","注册失败失败！");
        }
        else
        {
            qDebug()<<"insert into success";
            QMessageBox::information(this,"注册认证","注册成功，欢迎使用！");
            Logon *w = new Logon;
            w->show();
            this->close();
        }

    }
    else
    {
        QMessageBox::information(this,"注册认证","两次密码输入不一致");
    }
}
