#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <Matrix.h>
#include <QDoubleSpinBox>
#include <iostream>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aliveColor(0, 0, 255),
    dieColor(255, 255, 255)
{
    ui->setupUi(this);
    Matrix::MatrixType matrix = {
                         {1,0,1,0,1},
                         {0,1,0,1,0},
                         {1,0,1,0,1},
                         {0,1,0,1,0},
                         {1,0,1,0,1},
                 };
    m = std::shared_ptr<Matrix>(new Matrix(matrix.size()));
    m->AssignInitState(matrix);
    dim = m->getcurrentMatrix().size();

    director->SetMatrix(m);

    model = new QStandardItemModel();
    model->setColumnCount(dim);
    model->setRowCount(dim);
    ui->tableView->setModel(model);

    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateMatrixView()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    if (!curMatrix)
    {
        return;
    }
    for (int i = 0; i < dim; i++)
        for (int j = 0; j < dim ; j++)
        {
            model->setItem(i,j, new QStandardItem(""));
            if ((*curMatrix)[i][j])
                model->item(i, j)->setBackground(QBrush(aliveColor));
            else
                model->item(i, j)->setBackground(QBrush(dieColor));
        }
}

void MainWindow::on_pushButton_clicked()
{
    timer->stop();
    modelMatrix = *this->curMatrix;
    ui->okButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->setAsInitButton->setEnabled(true);
    running = false;
    director->StopProducing();
}

void MainWindow::updateMatrixView()
{
    this->curMatrix = director->PopCurrentMatrix();
    test();
    double speed = ui->speedSpin->value();
    timer->start((int)(speed * 1000));
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (running)
        return;
    int x = index.row(), y = index.column();
    model->setItem(x, y, new QStandardItem(""));
    if (modelMatrix[x][y])
    {
        model->item(x, y)->setBackground(QBrush(dieColor));
    }
    else
    {
        model->item(x, y)->setBackground(QBrush(aliveColor));
    }
    modelMatrix[x][y] = !modelMatrix[x][y];
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->okButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->setAsInitButton->setEnabled(false);
    timer->start(100);
    running = true;
    director->StartProducing();
}

void MainWindow::on_okButton_clicked()
{
    bool ok = false;
    dim = ui->dimEdit->text().toInt(&ok);
    if (!ok)
    {
        dim = 10;
        ui->dimEdit->setText(QString::number(dim));
    }
    model->setColumnCount(dim);
    model->setRowCount(dim);

    modelMatrix = Matrix::MatrixType(dim, std::vector<bool>(dim));
    m = std::shared_ptr<Matrix>(new Matrix(dim));
    m->AssignInitState(modelMatrix);
    director->SetMatrix(m);

}

void MainWindow::on_setAsInitButton_clicked()
{
    m->AssignInitState(modelMatrix);
}

void MainWindow::on_clearButton_clicked()
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim ; j++)
        {
            model->setItem(i,j, new QStandardItem(""));
            model->item(i, j)->setBackground(QBrush(dieColor));
            modelMatrix[i][j] = false;
        }
    }
}
