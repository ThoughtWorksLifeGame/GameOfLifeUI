#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <Matrix.h>
#include <QDoubleSpinBox>
#include <iostream>
#include <QLineEdit>
#include <QFileDialog>
#include <fstream>

void MainWindow::SetItemState(int x, int y, bool cellState)
{
    model->setItem(x, y, new QStandardItem(""));
    model->item(x, y)->setBackground(QBrush(cellState ? aliveColor : dieColor));
}

int MainWindow::GetDimFromEdit()
{
    bool ok = false;
    dim = ui->dimEdit->text().toInt(&ok);
    if (!ok || dim <= 0)
    {
        dim = 10;
        ui->dimEdit->setText(QString::number(dim));
    }
    return dim;
}

int MainWindow::GetHistoryLimitFromEdit()
{
    bool ok = false;
    historyLimit = ui->historyLimit->text().toInt(&ok);
    if (!ok || historyLimit <= 0)
    {
        historyLimit = 10;
        ui->historyLimit->setText(QString::number(historyLimit));
    }
    return historyLimit;
}

void MainWindow::InitHistorySlider(int limit)
{
    ui->historySlide->setRange(1, limit);
}

void MainWindow::InitTableView(int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim ; j++)
        {
            SetItemState(i, j, false);
        }
    }
    model->setColumnCount(dim);
    model->setRowCount(dim);
    modelMatrix = Matrix::MatrixType(dim, std::vector<bool>(dim));
    m = std::shared_ptr<Matrix>(new Matrix(dim));
    director->SetMatrix(m);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    aliveColor(0, 0, 255),
    dieColor(255, 255, 255)
{
    ui->setupUi(this);

    model = new QStandardItemModel();
    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    historyLimit = GetHistoryLimitFromEdit();
    director->SetHistoryLimit(historyLimit);
    InitHistorySlider(historyLimit);

    dim = GetDimFromEdit();
    InitTableView(dim);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateMatrixView()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update()
{
    if (!curMatrix)
    {
        return;
    }
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim ; j++)
        {
            SetItemState(i, j, (*curMatrix)[i][j]);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    timer->stop();
    modelMatrix = *this->curMatrix;
    ui->okButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->setAsInitButton->setEnabled(true);
    ui->historySlide->setEnabled(true);
    running = false;
    director->StopProducing();
    ui->historySlide->setValue(director->GetHistorySize());
}

void MainWindow::updateMatrixView()
{
    this->curMatrix = director->PopCurrentMatrix();
    InitHistorySlider(director->GetHistorySize());
    Update();
    double speed = ui->speedSpin->value();
    timer->start((int)(speed * 1000));
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    if (running)
        return;
    int x = index.row(), y = index.column();
    modelMatrix[x][y] = !modelMatrix[x][y];
    SetItemState(x, y, modelMatrix[x][y]);
}

void MainWindow::on_okButton_clicked()
{
    dim = GetDimFromEdit();
    InitTableView(dim);
}

void MainWindow::on_setAsInitButton_clicked()
{
    m->AssignInitState(modelMatrix);
}

void MainWindow::on_clearButton_clicked()
{
    InitTableView(dim);
}

void MainWindow::on_startButton_clicked()
{
    ui->okButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->setAsInitButton->setEnabled(false);
    ui->historySlide->setEnabled(false);
    timer->start(100);
    running = true;
    director->StartProducing();
}

void MainWindow::on_historySlide_valueChanged(int value)
{
    curMatrix = director->GetHistory(value - 1);
    Update();
}

void MainWindow::on_historyLimit_textChanged(const QString &)
{
    historyLimit = GetHistoryLimitFromEdit();
    director->SetHistoryLimit(historyLimit);
}

void MainWindow::on_exportButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Open txt"), ".", tr("txt Files(*.txt)"));
    if (path.length() != 0)
    {
        std::fstream fs(path.toStdString(), std::ios_base::trunc | std::ios_base::out);
        if (curMatrix)
            Matrix::Export(*curMatrix, fs);
        else
            Matrix::Export(modelMatrix, fs);
        fs.close();
    }
}

void MainWindow::on_importButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open txt"), ".", tr("txt Files(*.txt)"));
    if (path.length() != 0)
    {
        std::fstream fs(path.toStdString());
        modelMatrix = Matrix::Import(fs);
    }
    curMatrix = &modelMatrix;
    Update();
}
