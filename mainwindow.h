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
private:
    Ui::MainWindow *ui;
    std::shared_ptr<Matrix> m;
    Director* director = new Director(10, 10);
    QTimer* timer = new QTimer();
    int dim;
    QStandardItemModel* model = nullptr;
    const Matrix::MatrixType* curMatrix = nullptr;
    Matrix::MatrixType modelMatrix;
    bool running = false;
    QColor aliveColor;
    QColor dieColor;
    int historyLimit;

    void SetItemState(int x, int y, bool cellState);
    void InitTableView(int dim);
    int GetDimFromEdit();
    int GetHistoryLimitFromEdit();
    void InitHistorySlider(int limit);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Update();
private slots:
    void on_pushButton_clicked();
    void updateMatrixView();
    void on_tableView_clicked(const QModelIndex &index);

    void on_okButton_clicked();

    void on_setAsInitButton_clicked();

    void on_clearButton_clicked();

    void on_startButton_clicked();
    void on_historySlide_valueChanged(int value);
    void on_historyLimit_textChanged(const QString &arg1);
    void on_exportButton_clicked();
    void on_importButton_clicked();
};

#endif // MAINWINDOW_H
