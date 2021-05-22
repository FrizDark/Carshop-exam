#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "Menu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ModelTable::instance().load();
    CarTable::instance().load();
    ManagerTable::instance().load();
    CarManagerTable::instance().load();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitBtn_clicked()
{
    close();
}
