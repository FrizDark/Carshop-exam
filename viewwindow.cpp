#include "viewwindow.h"
#include "ui_viewwindow.h"

#include <QDebug>

ViewWindow::ViewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewWindow)
{
    ui->setupUi(this);
}

ViewWindow::ViewWindow(ViewType type, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewWindow)
{
    ui->setupUi(this);

    switch (type) {

    case manager:

        this->setWindowTitle("Менеджеры");
        ManagerTable::instance().load();
        print(ManagerTable::instance().asString().c_str());

    break;

    case model:

        this->setWindowTitle("Модели");
        ModelTable::instance().load();
        print(ModelTable::instance().asString().c_str());

    break;

    case car:

        this->setWindowTitle("Машины");
        CarTable::instance().load();

        print(CarTable::instance().asString().c_str());

    break;

    case carManager:

        this->setWindowTitle("Машины и менеджеры");
        CarManagerTable::instance().load();

        print(CarManagerTable::instance().asString().c_str());

    break;

    }

}

ViewWindow::~ViewWindow()
{
    delete ui;
}

//void ViewWindow::print(list<Model*> elements) {

//    QStringList list;
//    int i = 0, j = 0;
//    QTableWidgetItem* item = new QTableWidgetItem;

//    for (auto field : (*elements.begin())[0].Fields()) {
//        if (field.first == "ID") continue;
//        list.append(field.second.Description.c_str());
//    }
//    ui->tableWidget->setColumnCount(list.size());
//    ui->tableWidget->setHorizontalHeaderLabels(list);

//    ui->tableWidget->setRowCount(elements.size());
//    for (auto element : elements) {
//        for (auto value : element->Values()) {
//            if (value.first == "ID") continue;

//            item = new QTableWidgetItem;
//            item->setText(value.second.asString().c_str());

//            j = 0;
//            for (auto field : element->Fields()) {
//                if (field.second.Description == "ID") continue;
//                if (field.first == value.first) {
//                    ui->tableWidget->setItem(i, j, item);
//                    break;
//                }
//                j++;
//            }
//        }
//        i++;
//    }
//}

//void ViewWindow::print(View view) {

//    QStringList list;
//    int i = 0, j = 0;
//    QTableWidgetItem* item = new QTableWidgetItem;

//    for (auto field : view.Fields()) {
//        if (field.second.Description == "ID") continue;
//        list.append(field.second.Description.c_str());
//    }
//    ui->tableWidget->setColumnCount(list.size());
//    ui->tableWidget->setHorizontalHeaderLabels(list);

//    ui->tableWidget->setRowCount(view.Values().size());
//    for (auto element : view.Values()) {
//        for (auto value : element->Values()) {
//            if (value.first == "ID") continue;

//            item = new QTableWidgetItem;
//            item->setText(value.second.asString().c_str());

//            j = 0;
//            for (auto field : element->Fields()) {
//                if (field.second.Description == "ID") continue;
//                if (field.first == value.first) {
//                    ui->tableWidget->setItem(i, j, item);
//                    break;
//                }
//                j++;
//            }
//        }
//        i++;
//    }

//}

void ViewWindow::print(QString elements) {
    int i = 0, j = 0;
    QTableWidgetItem* item = new QTableWidgetItem;

    QStringList head_body = elements.split("/");

    QStringList head = head_body.at(0).split("|");
    QStringList body = head_body.at(1).split("=");

    ui->tableWidget->setColumnCount(head.size());
    ui->tableWidget->setHorizontalHeaderLabels(head);

    ui->tableWidget->setRowCount(body.size());

    for (auto row : body) {
        for (auto cell : row.split("|")) {

            item = new QTableWidgetItem;
            item->setText(cell);
            ui->tableWidget->setItem(i, j, item);

            j++;
        }
        j = 0;
        i++;
    }

}

void ViewWindow::on_exitBtn_clicked()
{

    emit closed();
    close();

}
