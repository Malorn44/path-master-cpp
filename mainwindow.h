#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QMainWindow>
#include <QTableWidget>
#include <QMouseEvent>
#include <QtCore>
#include <QtGui>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void onItemClicked(QTableWidgetItem *item);

    void on_sizeUp_clicked();

    void on_sizeDown_clicked();

    void on_colRowDown_clicked();

    void on_colRowUp_clicked();

private:
    Ui::MainWindow *ui;

    // handling board modifications
    // pressColor: 0-nothing select; 1-white select; 2-gray select; 3-green select; 4-red select
    // pressed: stores if the mouse button is pressed or not
    // prevLoc: keeps track of past location for moving start and end
    // eventFilter: for allowing the grid to use MouseButtonRelease to reset pressColor to 0
    int pressColor;
    bool pressed;
    std::pair<int, int> prevLoc;
    bool eventFilter(QObject *o, QEvent *e);


    // table stats
    int row;
    int col;
    int cellSize;

    void resizeCells();
};

#endif // MAINWINDOW_H
