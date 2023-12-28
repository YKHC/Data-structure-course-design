#include "logon.h"
#include "ui_logon.h"
#include "QMessageBox"
#include <QPainter>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QTime>
#include <QRandomGenerator>

QString currentCaptcha;

Logon::Logon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Logon)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    setWindowTitle("套汇程序");
    this->setFixedSize(1100, 660);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));

    this->refreshCaptcha();
    QPixmap *pix = new QPixmap(":/new/prefix1/1.png");
    QSize sz = ui->label->size();
    ui->label->setPixmap(pix->scaled(sz));
    this->initDatabase();

    /*QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(-3, 0);
    shadow->setColor(QColor("#888888"));
    shadow->setBlurRadius(30);
    ui->label->setGraphicsEffect(shadow);*/

    this->new_window = new MainWindow();
    connect(this->new_window, &MainWindow::back, this, &Logon::resetInputs);
    connect(this->new_window, &MainWindow::back, this, [=]{
        this->new_window->hide();
        this->show();
    }
    );

    connect(ui->pushButton_2, &QPushButton::clicked, this, &Logon::onExitClicked);
}

Logon::~Logon()
{
    delete ui;
    QSqlDatabase::removeDatabase("QSQLITE");
}

void Logon::resetInputs(){
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void Logon::onExitClicked() {
    QApplication::quit();
}

void Logon::switchToMainWindow()
{
    QRect startGeometry = this->geometry();
    new_window->setGeometry(startGeometry);

    QPropertyAnimation *animation = new QPropertyAnimation(new_window, "geometry");
    animation->setDuration(500);
    animation->setStartValue(startGeometry);

    QRect endGeometry = this->geometry();
    endGeometry.moveLeft(endGeometry.left() + 10);
    animation->setEndValue(endGeometry);

    animation->start(QPropertyAnimation::DeleteWhenStopped);

    new_window->show();
    this->hide();
}


void Logon::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    QString captchaInput = ui->lineEdit_3->text();
    QString sql=QString("select * from user where username='%1' and password='%2'")
            .arg(username).arg(password);
    //创建执行语句对象
    QSqlQuery query(sql);
    //判断执行结果
    if(!query.next())
    {
        qDebug()<<"Login error";
        QMessageBox::information(this,"登录认证","登录失败,账户或者密码错误");
        refreshCaptcha();
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
    }
    else if (captchaInput != currentCaptcha)
    {
        QMessageBox::warning(this, "验证失败", "验证码错误");
        refreshCaptcha();
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
    }
    else
    {
        qDebug()<<"Login success";
        QMessageBox::information(this,"登录认证","登录成功");
        //登录成功后可以跳转到其他页面
        this->new_window = new MainWindow();
        this->switchToMainWindow();
    }

}

void Logon::on_pushButton_3_clicked()
{
    this->close();
    signup *s = new signup;
    s->show();

}

void Logon::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    if (!db.open()) {
        qDebug() << "Error: unable to open database";
    } else {
        // 如果需要，创建表
        this->createTables();
    }
}

void Logon::createTables()
{
    QString createsql = QString("CREATE TABLE IF NOT EXISTS user (id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "username TEXT UNIQUE NOT NULL, "
                                "password TEXT NOT NULL)");
    QSqlQuery query;
    if (!query.exec(createsql)) {
        qDebug() << "Error: unable to create table";
    }
}

void Logon::refreshCaptcha()
{
    currentCaptcha = generateCaptchaString(6); // 生成6位数验证码
    QImage captchaImage = generateCaptchaImage(QSize(100, 50), currentCaptcha);
    QPixmap pixmap = QPixmap::fromImage(captchaImage);
    ui->label_3->setPixmap(pixmap);
}

// 生成随机字符串作为验证码
QString Logon::generateCaptchaString(int length)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    QString randomString;
    for(int i = 0; i < length; ++i)
    {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

// 将字符串绘制到图像上
QImage Logon::generateCaptchaImage(const QSize &size, const QString &captchaString)
{
    QImage image(size, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(), Qt::white);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(image.rect(), Qt::AlignCenter, captchaString);
    return image;
}
