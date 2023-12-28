#ifndef INSERT1_H
#define INSERT1_H

#include <QWidget>
#include "Graph.h"
#include "function.h"

namespace Ui {
class insert1;
}

class insert1 : public QWidget
{
    Q_OBJECT

public:
    explicit insert1(Graph* graph, Function *fun, QWidget *parent = nullptr);
    ~insert1();
    void paintEvent(QPaintEvent *event);
    void setGraph(Graph* newGraph);

private:
    Ui::insert1 *ui;
    Graph* graph;
    Function *fun;

signals:
    void back(void);
};

#endif // INSERT1_H
