#ifndef FUNCTION_H
#define FUNCTION_H

#include <QWidget>
#include "Graph.h"

namespace Ui {
class Function;
}

class Function : public QWidget
{
    Q_OBJECT

public:
    explicit Function(Graph* graph, QWidget *parent = nullptr);
    ~Function();
    void onExitClicked();
    //void ReplaceGraph(Graph*& currentGraph, Graph* newGraph);
    void paintEvent(QPaintEvent *event);
    void setGraph(Graph* newGraph);

    void UpdateUIWithGraphInfo(const Graph* graph);

private:
    Ui::Function *ui;
    Graph* graph;

signals:
    void back(void);
    void replaceGraphRequested(Graph* newGraph);

public slots:
    void updateGraphReference(Graph* newGraph, Graph*graph);

};

#endif // FUNCTION_H
