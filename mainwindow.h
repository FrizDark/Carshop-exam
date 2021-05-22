#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "viewwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_exitBtn_clicked();

    void on_managerBtn_clicked();

    void closed();

    void on_modelBtn_clicked();

    void on_carBtn_clicked();

    void on_carManagerBtn_clicked();

private:
    Ui::MainWindow *ui;

    void openView(ViewType type);
};
#endif // MAINWINDOW_H
