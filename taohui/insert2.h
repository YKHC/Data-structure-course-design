#ifndef INSERT2_H
#define INSERT2_H

#include <QWidget>
#include "Graph.h"
#include "function.h"
#include <QNetworkAccessManager>

namespace Ui {
class insert2;
}

class insert2 : public QWidget
{
    Q_OBJECT

public:
    explicit insert2(Graph* graph, Function *fun, QWidget *parent = nullptr);
    ~insert2();

    void FetchRates();
    void OnRatesFetched(QNetworkReply *reply);

    void paintEvent(QPaintEvent *event);

    void setGraph(Graph* newGraph);

private:
    Ui::insert2 *ui;
    Graph* graph;
    Function *fun;
    QNetworkAccessManager *manager;

signals:
    void back(void);
};

#endif // INSERT2_H
