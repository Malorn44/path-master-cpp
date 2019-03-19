#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // allows the board to detect mouse presses
    ui->board->viewport()->installEventFilter(this);

    pressed = false;
    pressColor = 0;
    prevLoc = std::make_pair(0,0);

    row = 20;
    col = 20;
    cellSize = 20;

    // set the amount of rows and columns
    ui->board->setRowCount(row);
    ui->board->setColumnCount(col);

    // remove table headers
    ui->board->horizontalHeader()->setVisible(false);
    ui->board->verticalHeader()->setVisible(false);

    // table styling
    ui->board->setFocusPolicy(Qt::NoFocus);
    ui->board->setSelectionMode(QAbstractItemView::NoSelection);
    ui->board->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->board->horizontalHeader()->setMinimumSectionSize(10);
    ui->board->verticalHeader()->setMinimumSectionSize(10);

    // set fixed column and row width
    for (int i = 0; i < row; i++)
        ui->board->setRowHeight(i, cellSize);
    for (int i = 0; i < col; i++)
        ui->board->setColumnWidth(i, cellSize);

    // populate with items
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            ui->board->setItem(i, j, new QTableWidgetItem);
            ui->board->item(i, j)->setBackground(Qt::white);
        }
    }

    // defaults green (start) to top left and red (end) to top right
    ui->board->item(0,0)->setBackground(Qt::green);
    ui->board->item(0,col-1)->setBackground(Qt::red);

    // connecting SIGNALS to onItemClicked SLOT in order to process board modifications
    connect( ui->board, &QTableWidget::itemEntered, this, &MainWindow::onItemClicked );
    connect( ui->board, &QTableWidget::itemPressed, this, &MainWindow::onItemClicked );
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
    if (e->type() == QEvent::MouseButtonRelease) {
        pressed = false;
        if (  o == ui->board->viewport() ) {
            pressColor = 0;
            std::cout << "Button Release" << std::endl;
        }
    } else if (e->type() == QEvent::MouseButtonPress) {
        if (  o == ui->board->viewport() ) {
            pressed = true;
            std::cout << "Button Press" << std::endl;
        }
    }
    return QObject::eventFilter(o, e);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::onItemClicked(QTableWidgetItem *item) {
    // Changes the color of cells based on their current color
    // Selecting a green (start) or a red (end) cell will put it into "drag" mode
    // pressColor: 0-nothing select; 1-white select; 2-gray select; 3-green select; 4-red select
    if (!pressed) return;
    if (item->backgroundColor() == Qt::gray && pressColor <= 1) {
        pressColor = 1;
        item->setBackground(Qt::white);
    } else if (item->backgroundColor() == Qt::white && (pressColor == 0 || pressColor > 1)) {
        if (pressColor >= 3) {
            ui->board->item(prevLoc.first, prevLoc.second)->setBackground(Qt::white);
            prevLoc.first = item->row();
            prevLoc.second = item->column();
            if (pressColor == 3) {
                item->setBackground(Qt::green);
            } else {
                item->setBackground(Qt::red);
            }
        } else {
            pressColor = 2;
            item->setBackground(Qt::gray);
        }
    } else if (pressColor == 0) {
        if (item->backgroundColor() == Qt::green) {
            pressColor = 3;
        } else if (item->backgroundColor() == Qt::red){
            pressColor = 4;
        }
        prevLoc.first = item->row();
        prevLoc.second = item->column();
    }
    std::cout << item->row() << ", " << item->column() << std::endl;
}

void MainWindow::on_sizeUp_clicked()
{
    cellSize += 10;
    if (cellSize == 100)
        cellSize -= 10;
    resizeCells();
}

void MainWindow::on_sizeDown_clicked()
{
    cellSize -= 10;
    if (cellSize == 0)
        cellSize += 10;
    resizeCells();
}

void MainWindow::resizeCells() {
    std::cout << "Cell Size: " << cellSize << std::endl;
    for (int i = 0; i < row; i++)
        ui->board->setRowHeight(i, cellSize);
    for (int i = 0; i < col; i++)
        ui->board->setColumnWidth(i, cellSize);
}

void MainWindow::on_colRowDown_clicked()
{
    row -= 10;
    col -= 10;
    ui->board->setRowCount(row);
    ui->board->setColumnCount(col);
    resizeCells();
}

void MainWindow::on_colRowUp_clicked()
{
    row += 10;
    col += 10;
    ui->board->setRowCount(row);
    ui->board->setColumnCount(col);
    for (int i = 0; i < row; i++) {
        if (i < row - 10) {
            for (int j = col-10; j < col; j++) {
                ui->board->setItem(i, j, new QTableWidgetItem);
                ui->board->item(i, j)->setBackground(Qt::white);
            }
        } else {
            for (int j = 0; j < col; j++) {
                ui->board->setItem(i, j, new QTableWidgetItem);
                ui->board->item(i, j)->setBackground(Qt::white);
            }
        }
    }
    resizeCells();
}
