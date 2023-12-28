#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "insert1.h"
#include "insert2.h"
#include "insert3.h"
#include "Graph.h"
#include "function.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *event);

public slots:
    void ReplaceGraph(Graph* newgraph);

private:
    Ui::MainWindow *ui;
    insert1 *insert1Window;
    insert2 *insert2Window;
    insert3 *insert3Window;
    Graph *graph;
    Function *function;

signals:
    void back();
    void graphChanged(Graph* newGraph, Graph*graph);

private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
