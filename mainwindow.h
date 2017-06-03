#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Matrix.h>
#include <Director.h>
#include <QStandardItemModel>
#include <QTimer>
#include <QColor>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::shared_ptr<Matrix> m;
    Director* director = new Director(1, 1);
    QTimer* timer = new QTimer();
    int dim;
    QStandardItemModel* model = nullptr;
    const Matrix::MatrixType* curMatrix = nullptr;
    Matrix::MatrixType modelMatrix;
    bool running = false;
    QColor aliveColor;
    QColor dieColor;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void test();
private slots:
    void on_pushButton_clicked();
    void updateMatrixView();
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_okButton_clicked();

    void on_setAsInitButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
