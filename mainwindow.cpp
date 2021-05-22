#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include "Menu.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ModelTable::instance().load();
//    CarTable::instance().load();
//    ManagerTable::instance().load();
//    CarManagerTable::instance().load();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closed() {
    this->show();
}

void MainWindow::on_exitBtn_clicked()
{
    this->close();
}

void MainWindow::openView(ViewType type) {

    auto view = new ViewWindow(type);
    connect(view, SIGNAL(closed()), this, SLOT(closed()));
    this->hide();
    view->show();

}

void MainWindow::on_managerBtn_clicked()
{
    openView(ViewType::manager);
}

void MainWindow::on_modelBtn_clicked()
{
    openView(ViewType::model);
}

void MainWindow::on_carBtn_clicked()
{
    openView(ViewType::car);
}

void MainWindow::on_carManagerBtn_clicked()
{
    openView(ViewType::carManager);
}
