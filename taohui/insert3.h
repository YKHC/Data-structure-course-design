#ifndef INSERT3_H
#define INSERT3_H

#include <QWidget>
#include "Graph.h"
#include "function.h"

namespace Ui {
class insert3;
}

class insert3 : public QWidget
{
    Q_OBJECT

public:
    explicit insert3(Graph* graph, Function *fun, QWidget *parent = nullptr);
    ~insert3();
    void handleButtonClicked();
    void paintEvent(QPaintEvent *event);
    void setGraph(Graph* newGraph);

private:
    Ui::insert3 *ui;
    Graph* graph;
    Function *fun;

signals:
    void back(void);
private slots:
    void on_pushButton_5_clicked();
};

#endif // INSERT3_H
