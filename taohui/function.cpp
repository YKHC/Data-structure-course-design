#include "function.h"
#include "ui_function.h"
#include <QIcon>
#include <qdebug.h>
#include <vector>
#include <string>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QTextCodec>
#include <QPainter>

Function::Function(Graph* graph, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::Function),
    graph(graph)
{
    ui->setupUi(this);
    this->setFixedSize(1000, 800);
    this->setWindowIcon(QIcon(":/new/prefix1/1.png"));
    setWindowTitle("套汇程序");

    ui->lineEdit->setStyleSheet(
        "background-image: url(E:/keshe/taohui/1.png);"
        "background-repeat: no-repeat;"
        "background-position: center;"
    );


    ui->lineEdit->clear();
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();

    ui->lineEdit->setEnabled(false);

    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SIGNAL(back()));

    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        qDebug() << "Back button clicked";
        emit back();
        ui->textBrowser->clear();
        ui->textBrowser_2->clear();
        ui->textBrowser_3->clear();
        ui->lineEdit->clear();
    });
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Function::onExitClicked);

    connect(ui->pushButton_3, &QPushButton::clicked, this, [=](){
        std::vector<std::string> currencies = graph->GetAllCurrencies();
        QString display;

        ui->textBrowser->clear();
        for (const std::string &currency : currencies) {
            display += QString::fromStdString(currency) + " ";
        }

        ui->textBrowser->append(display);
    });

    connect(ui->pushButton_13, &QPushButton::clicked, this, [=](){
        QString filePath = ":/new/prefix1/function.txt";
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            in.setCodec(QTextCodec::codecForName("UTF-8"));
            ui->textBrowser_3->setText(in.readAll());
            file.close();
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        }
    });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(true);
        graph->Action = 1;
    });

    connect(ui->pushButton_5, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(true);
        graph->Action = 2;
    });

    connect(ui->pushButton_6, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(false);
        graph->Action = 3;
    });

    connect(ui->pushButton_7, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("输入起始货币和目标货币，格式为：USD,EUR");
        graph->Action = 4;
    });

    connect(ui->pushButton_11, &QPushButton::clicked, this, [=]() {
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("请输入起始货币、目标货币、起始金额和目标金额，格式为：USD,EUR,100,1000");
        graph->Action = 5;
    });

    connect(ui->pushButton_12, &QPushButton::clicked, this, [=](){
        ui->lineEdit->clear();
        ui->textBrowser_2->clear();
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText("请输入要检验的货币序列");
        graph->Action = 8;
    });

    connect(ui->pushButton_9, &QPushButton::clicked, this, [=]() {
        static double startAmount = 0.0;
        static double targetAmount = 0.0;
        static std::string startCurrency;
        static std::string targetCurrency;

        if (graph->Action == 1)
        {
            ui->lineEdit->setEnabled(false);
            std::string startCurrency = ui->lineEdit->text().toStdString();
            bool result = graph->CanArbitrage(startCurrency);
            QString output = result ? "可以实现套汇" : "无法实现套汇";
            ui->textBrowser_2->setText(output);
        }
        else if(graph->Action == 2)
        {
            QString inputCurrency = ui->lineEdit->text();
            startCurrency = inputCurrency.toStdString();
            auto loops = graph->FindArbitrageLoops(startCurrency);
            QString output;
            for (const auto& loop : loops)
            {
                output += "环路: ";
                for (const auto& currency : loop.first)
                {
                    output += QString::fromStdString(currency + " -> ");
                }
                output += QString(" 倍数: %1\n").arg(loop.second);
            }
            if (loops.empty())
            {
                output = "没有找到套汇环路";
            }
            ui->textBrowser_2->setText(output);
        }
        else if (graph->Action == 3)
        {
            ui->lineEdit->setEnabled(false);
            std::vector<std::string> arbitrageCurrencies = graph->FindArbitrageCurrencies();
            QString result;

            for (const auto& currency : arbitrageCurrencies)
            {
                result += QString::fromStdString(currency) + "\n";
            }

            ui->textBrowser_2->setText(result);
        }
        else if (graph->Action == 4)
        {
            ui->lineEdit->setEnabled(false);
            QString input = ui->lineEdit->text();
            QStringList currencies = input.split(',');

            if (currencies.size() == 2)
            {
                std::string fromCurrency = currencies.at(0).trimmed().toStdString();
                std::string toCurrency = currencies.at(1).trimmed().toStdString();

                auto result = graph->FindBestExchangePath(fromCurrency, toCurrency);
                std::vector<std::string> path = result.first;
                double rate = result.second;

                QString pathStr;
                for (const auto& currency : path)
                {
                    pathStr += QString::fromStdString(currency) + " -> ";
                }
                pathStr.chop(4); // Remove the last " -> "

                ui->textBrowser_2->setText(pathStr + QString("\n汇率: %1").arg(rate));
             }
            else
            {
                ui->textBrowser_2->setText("输入格式错误，请按格式输入：USD,EUR");
               }
        }
        else if (graph->Action == 5)
        {
            QString input = ui->lineEdit->text();
            QStringList parts = input.split(',');
            if (parts.size() == 4)
            {
                startCurrency = parts.at(0).trimmed().toStdString();
                targetCurrency = parts.at(1).trimmed().toStdString();
                startAmount = parts.at(2).trimmed().toDouble();
                targetAmount = parts.at(3).trimmed().toDouble();

                auto result = graph->FindArbitragePath(startCurrency, targetCurrency, startAmount, targetAmount);
                std::vector<std::string> path = result.first;
                int times = result.second;

                QString pathStr;

                for (const auto& currency : path)
                {
                    pathStr += QString::fromStdString(currency) + " -> ";
                }
                pathStr.chop(4); // 移除最后的" -> "
                ui->textBrowser_2->setText(pathStr + QString("\n交易次数: %1").arg(times));

                graph->Action = 0;
            }
            else
            {
                ui->textBrowser_2->setText("输入格式错误，请按格式输入：USD,EUR,100,1000");
            }
        }

        else if (graph->Action == 6)
        {
             ui->lineEdit->setEnabled(false);
             bool ok;
             int nodecount = ui->lineEdit->text().toInt(&ok);
             if (ok && nodecount > 0){
                 Graph* subGraph = graph->GenerateRandomSubgraph(static_cast<std::size_t>(nodecount));
                 emit this->replaceGraphRequested(subGraph);
                 qDebug() << "replaceGraphRequested signal emitted with subGraph";
             }

        }
        else if (graph->Action == 7)
        {
            ui->lineEdit->setEnabled(false);
            QString input = ui->lineEdit->text();
            QStringList currencies = input.split(',');
            std::set<std::string> currencySequence;

            for (const auto& qStr : currencies)
            {
                std::string str = qStr.trimmed().toStdString(); // 去除前后空格
                currencySequence.insert(str);
            }
            Graph* subgraph = graph->ExtractSubgraph(currencySequence);
            emit this->replaceGraphRequested(subgraph);
        }
        else if (graph->Action == 8)
        {
            ui->lineEdit->setEnabled(false);
            QString input = ui->lineEdit->text();
            QStringList currencies = input.split(',');
            std::vector<std::string> currencySequence;

            for (const auto& qStr : currencies)
            {
                std::string str = qStr.trimmed().toStdString(); // 去除前后空格
                currencySequence.push_back(str);
            }
            if (graph->IsValidSequence(currencySequence))
            {
                if (graph->CanArbitrageOnSequence(currencySequence) > 1.0)
                {
                    float result = graph->CanArbitrageOnSequence(currencySequence);
                    QString text = "可以套汇，每次循环获得的利润为： " + QString::number(result, 'f', 2);
                    ui->textBrowser_2->append(text);
                }
                else if(graph->CanArbitrageOnSequence(currencySequence) <= 1.0 && graph->CanArbitrageOnSequence(currencySequence) > 0.0)
                {
                    float result = graph->CanArbitrageOnSequence(currencySequence);
                    QString text = "无法套汇，每次循环减小为原来的： " + QString::number(result, 'f', 2);
                    ui->textBrowser_2->append(text);
                }
                else
                {
                    ui->textBrowser_2->setText("序列无效,缺乏足够的数据。");
                }
            }
            else
            {
                QMessageBox::warning(this, "错误", "请重新输入");
            }

        }
        graph->Action = 0;
    });

    connect(ui->pushButton_14, &QPushButton::clicked, this, [=](){
        // 获取 textBrowser_2 和 textBrowser_3 的内容
        QString textContent2 = ui->lineEdit->text();
        QString textContent3 = ui->textBrowser_2->toPlainText();

        QString filePath = ":/new/prefix1/document.txt";

        // 打开或创建文件
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QTextStream out(&file);
            out << textContent2 << "\n";
            out << "\n";
            out << textContent3 << "\n";
            file.close();
        }
    });

    connect(ui->pushButton_8, &QPushButton::clicked, this, [=](){
        graph->displayRatesInTextBrowser(ui->textBrowser_2);
    });
}

Function::~Function()
{
    delete ui;
}

void Function::onExitClicked() {
    QApplication::quit();
}

/*void Function::ReplaceGraph(Graph*& currentGraph, Graph* newGraph) {
    if (currentGraph != nullptr) {
        delete currentGraph;  // 删除现有的对象
    }
    currentGraph = newGraph;  // 将指针指向新的对象
}*/

void Function::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/new/prefix1/thumb.jpg");
    painter.drawPixmap(this->rect(), pixmap);
    QWidget::paintEvent(event);
}

void Function::setGraph(Graph* newGraph) {
    if (newGraph != nullptr) {
        graph = newGraph;
        UpdateUIWithGraphInfo(newGraph); // 更新 UI
    }
}

void Function::UpdateUIWithGraphInfo(const Graph* graph)
{
    ui->textBrowser->clear();
    if (graph != nullptr)
    {
        std::vector<std::string> currencies = graph->GetAllCurrencies();
        QString display;
        for (const std::string &currency : currencies)
        {
            display += QString::fromStdString(currency) + " ";
        }
        ui->textBrowser->setText(display);
    }
}


void Function::updateGraphReference(Graph* newGraph, Graph* graph)
{
    graph = newGraph;
    UpdateUIWithGraphInfo(newGraph);
}
