#include "insert2.h"
#include "ui_insert2.h"
#include "QIcon"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <cmath>
#include <QPainter>

insert2::insert2(Graph *graph, Function *fun, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::insert2),
    graph(graph),
    fun(fun),
    manager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->setFixedSize(1000, 800);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));
    setWindowTitle("套汇程序");

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));

    connect(ui->pushButton_3, &QPushButton::clicked, this, &insert2::FetchRates);

    connect(manager, &QNetworkAccessManager::finished, this, &insert2::OnRatesFetched);

    connect(ui->pushButton_2, &QPushButton::clicked, this, [=](){
        this->hide();
        this->fun->show();
    });
}

void insert2::FetchRates()
{
    qDebug() << "OpenSSL支持情况:" << QSslSocket::supportsSsl();
    qDebug() << "OpenSSL版本:" << QSslSocket::sslLibraryBuildVersionString();
    QStringList baseCurrencies = {"EUR", "USD", "CAD", "JPY", "CNY"};

    QStringList urls =
    {
        "https://api.currencyapi.com/v3/latest?apikey=cur_live_5bFN7er0XYTU2rLDqEy6o8IjkZWujww6mJyQH6Rk&currencies=USD%2CCAD%2CJPY%2CCNY&base_currency=EUR",
        "https://api.currencyapi.com/v3/latest?apikey=cur_live_5bFN7er0XYTU2rLDqEy6o8IjkZWujww6mJyQH6Rk&currencies=JPY%2CEUR%2CCNY%2CCAD",
        "https://api.currencyapi.com/v3/latest?apikey=cur_live_5bFN7er0XYTU2rLDqEy6o8IjkZWujww6mJyQH6Rk&currencies=JPY%2CCNY%2CEUR%2CUSD&base_currency=CAD",
        "https://api.currencyapi.com/v3/latest?apikey=cur_live_5bFN7er0XYTU2rLDqEy6o8IjkZWujww6mJyQH6Rk&currencies=CNY%2CEUR%2CUSD%2CCAD&base_currency=JPY",
        "https://api.currencyapi.com/v3/latest?apikey=cur_live_5bFN7er0XYTU2rLDqEy6o8IjkZWujww6mJyQH6Rk&currencies=JPY%2CEUR%2CUSD%2CCAD&base_currency=CNY"
    };

    for (int i = 0; i < urls.size(); ++i)
    {
        QUrl url(urls[i]);
        QNetworkRequest request(url);
        request.setAttribute(QNetworkRequest::User, QVariant(baseCurrencies[i]));
        manager->get(request);
    }
}



void insert2::OnRatesFetched(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << "Error:" << reply->errorString();
        return;
    }

    QString baseCurrency = reply->request().attribute(QNetworkRequest::User).toString();
    QByteArray responseBytes = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseBytes);
    QJsonObject responseObject = jsonResponse.object();

    if (!responseObject.contains("data"))
    {
        qDebug() << "Response does not contain 'data' field";
        return;
    }

    QJsonObject dataObject = responseObject["data"].toObject();

    for (const QString &targetCurrency : dataObject.keys())
    {
        QJsonObject rateObject = dataObject[targetCurrency].toObject();
        double rate = rateObject["value"].toDouble();

        if (rate <= 0 || std::isinf(rate))
        {
            qDebug() << "Invalid rate for" << targetCurrency;
            continue;
        }

        graph->addRate(baseCurrency.toStdString(), targetCurrency.toStdString(), rate);

        // 更新UI (如果需要)
        QString rateText = QString("%1 to %2 rate: %3").arg(baseCurrency, targetCurrency, QString::number(rate));
        ui->textBrowser->append(rateText);
    }

    reply->deleteLater();
}

insert2::~insert2()
{
    delete ui;
    if (manager) {
        delete manager;
    }
}

void insert2::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/background.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void insert2::setGraph(Graph* newGraph)
{
    graph = newGraph;
}
