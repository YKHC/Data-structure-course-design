#ifndef LOGON_H
#define LOGON_H

#include <QWidget>
#include <mainwindow.h>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "signup.h"


namespace Ui {
class Logon;
}

class Logon : public QWidget
{
    Q_OBJECT

public:
    explicit Logon(QWidget *parent = nullptr);
    ~Logon();
    void onExitClicked();
    void switchToMainWindow();
    void initDatabase();
    void createTables();
    void refreshCaptcha();
    QString generateCaptchaString(int length);
    QImage generateCaptchaImage(const QSize &size, const QString &captchaString);

private:
    Ui::Logon *ui;
    MainWindow *new_window;
    QString validUsername = "";
    QString validPassword = "";

private slots:
    void resetInputs();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();


};

#endif // LOGON_H
