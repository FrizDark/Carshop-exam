#include "viewwindow.h"
#include "ui_viewwindow.h"

#include <QDebug>

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGridLayout>

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

    this->type = type;

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

void ViewWindow::print(QString elements) {
    int i = 0, j = 0;
    QTableWidgetItem* item = new QTableWidgetItem;

    QStringList head_body = elements.split("/");

    QStringList head = head_body.at(0).split("|");
    ui->tableWidget->setColumnCount(head.size());
    ui->tableWidget->setHorizontalHeaderLabels(head);
    if (head_body.size() == 2) {
        ui->delBtn->setEnabled(true);
        ui->editBtn->setEnabled(true);
        ui->findBtn->setEnabled(true);

        QStringList body = head_body.at(1).split("=");
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
    } else {
        ui->tableWidget->setRowCount(0);
        ui->delBtn->setEnabled(false);
        ui->editBtn->setEnabled(false);
        ui->findBtn->setEnabled(false);
    }

}

void ViewWindow::on_exitBtn_clicked()
{

    switch (type) {

    case manager:
        ManagerTable::instance().save();
    break;
    case model:
        ModelTable::instance().save();
    break;
    case car:
        CarTable::instance().save();
    break;
    case carManager:
        CarManagerTable::instance().save();
    break;

    }

    emit closed();
    close();

}

void ViewWindow::on_addBtn_clicked()
{
    AddWindow* add_ui = nullptr;

    map<string, TypeName> fields;
    map<string, string> id_fields;
    map<string, list<Model*>> list;
    InputStyle* is;

    Model* m;

    switch (type) {

    case manager:
        m = new ManagerModel;
        add_ui = new AddWindow(m->Fields());
        connect(add_ui, SIGNAL(sendData(map<string, ElementValue>)), this, SLOT(getData(map<string, ElementValue>)));
        add_ui->show();
    break;
    case model:
        m = new ModelModel;
        add_ui = new AddWindow(m->Fields());
        connect(add_ui, SIGNAL(sendData(map<string, ElementValue>)), this, SLOT(getData(map<string, ElementValue>)));
        add_ui->show();
    break;
    case car:
        m = new CarModel;
        ModelTable::instance().load();

        id_fields.insert(make_pair("Model_ID", "Модель"));
        list.insert(make_pair("Model_ID", ModelTable::instance().elements()));

        is = new InputStyle(m->Fields(), id_fields, list);

        add_ui = new AddWindow(is);
        connect(add_ui, SIGNAL(sendData(map<string, ElementValue>)), this, SLOT(getData(map<string, ElementValue>)));
        add_ui->show();
    break;
    case carManager:
        m = new CarManagerModel;
        CarTable::instance().load();
        ManagerTable::instance().load();

        id_fields.insert(make_pair("Car_ID", "Машина"));
        id_fields.insert(make_pair("Manager_ID", "Менеджер"));
        list.insert(make_pair("Car_ID", CarTable::instance().elements()));
        list.insert(make_pair("Manager_ID", ManagerTable::instance().elements()));

        is = new InputStyle(m->Fields(), id_fields, list);

        add_ui = new AddWindow(is);
        connect(add_ui, SIGNAL(sendData(map<string, ElementValue>)), this, SLOT(getData(map<string, ElementValue>)));
        add_ui->show();
    break;

    }
}

void ViewWindow::getData(map<string, ElementValue> value) {
    string s;
    Model* m;

    auto f = [=](Model* model) {
        random_generator gen;

        for (auto &i : model->Fields()) {
            if (i.first == "ID") {
                (*model)["ID"] = to_string(gen());
                continue;
            }
            else {
                model->insert(*value.find(i.first));
            }
        }

    };

    switch (type) {
    case manager:
        m = new ManagerModel();
        f(m);
        ManagerTable::instance().add(*(ManagerModel*)m);
        print(ManagerTable::instance().asString().c_str());
    break;
    case model:
        m = new ModelModel();
        f(m);
        ModelTable::instance().add(*(ModelModel*)m);
        print(ModelTable::instance().asString().c_str());
    break;
    case car:
        m = new CarModel();
        f(m);
        CarTable::instance().add(*(CarModel*)m);
        print(CarTable::instance().asString().c_str());
    break;
    case carManager:
        m = new CarManagerModel();
        f(m);
        CarManagerTable::instance().add(*(CarManagerModel*)m);
        print(CarManagerTable::instance().asString().c_str());
    break;
    }

}

void ViewWindow::on_delBtn_clicked()
{

    int row = ui->tableWidget->selectedItems().at(0)->row();

    auto f = [=](list<Model*> models) -> Model* {
        int i = 0;
        for (auto item : models) {
            if (i == row) {
                return item;
            }
            i++;
        }
    };

    switch (type) {
    case manager:
        ManagerTable::instance().remove(*(ManagerModel*)f(ManagerTable::instance().elements()));
        print(ManagerTable::instance().asString().c_str());
    break;
    case model:
        ModelTable::instance().remove(*(ModelModel*)f(ModelTable::instance().elements()));
        print(ModelTable::instance().asString().c_str());
    break;
    case car:
        CarTable::instance().remove(*(CarModel*)f(CarTable::instance().elements()));
        print(CarTable::instance().asString().c_str());
    break;
    case carManager:
        CarManagerTable::instance().remove(*(CarManagerModel*)f(CarManagerTable::instance().elements()));
        print(CarManagerTable::instance().asString().c_str());
    break;
    }

}
