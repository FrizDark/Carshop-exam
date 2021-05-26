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
        print(ManagerTable::instance().elements());

    break;

    case model:

        this->setWindowTitle("Модели");
        ModelTable::instance().load();
        print(ModelTable::instance().elements());

    break;

    case car:

        this->setWindowTitle("Машины");
        ModelTable::instance().load();
        CarTable::instance().load();
        print(CarTable::instance().elements());

    break;

    case carManager:

        this->setWindowTitle("Машины и менеджеры");
        CarTable::instance().load();
        ManagerTable::instance().load();
        CarManagerTable::instance().load();
        print(CarManagerTable::instance().elements());

    break;

    }

}

ViewWindow::~ViewWindow()
{
    delete ui;
}

void ViewWindow::print(list<Model*> models) {
    int i = 0, j = 0, menuPos = 0;
    QTableWidgetItem* item;
    QLineEdit* cell;
    QComboBox* cell_cb;
    QStringList columns;

    QSignalMapper* mapper = new QSignalMapper(this);

    Model* m;

    auto f = [](Model* m) -> string {
        QStringList list;
        for (auto i : m->Values()) {
            if (m->Fields().find(i.first)->second.Description == "ID") continue;
            list.append(i.second.asString().c_str());
        }
        return list.join(" | ").toStdString();
    };

    switch (type) {
    case manager:
        m = new ManagerModel;
    break;
    case model:
        m = new ModelModel;
    break;
    case car:
        m = new CarModel;
    break;
    case carManager:
        m = new CarManagerModel;
    break;
    }
    for (auto head : m->Fields()) {
        if (head.first == "ID") continue;

        if (head.second.Description == "ID") {
            if (head.first == "Model_ID") {
                columns.append("Модель");
            }
            else if (head.first == "Car_ID") {
                columns.append("Машина");
            }
            else if (head.first == "Manager_ID") {
                columns.append("Менеджер");
            }
        } else {
            columns.append(head.second.Description.c_str());
        }
    }
    ui->tableWidget->setColumnCount(columns.size());
    ui->tableWidget->setHorizontalHeaderLabels(columns);

    ui->tableWidget->setRowCount(models.size());
    if (models.size() > 0) {
        ui->delBtn->setEnabled(true);
        ui->findBtn->setEnabled(true);

        for (auto model : models) {
            for (auto value : model->Values()) {
                auto field = *m->Fields().find(value.first);
//                if (field.second.Description == "ID") continue;
                if (field.first == "ID") continue;

                item = new QTableWidgetItem;
                ui->tableWidget->setItem(i, j, item);

                if (field.second.Description == "ID") {
                    cell_cb = new QComboBox;

                    mapper->setMapping(cell_cb, QString().asprintf("%d|%d", j, i));

                    menuPos = 0;
                    if (field.first == "Model_ID") {
                        for (auto model : ModelTable::instance().elements()) {
                            cell_cb->addItem(f(model).c_str());
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                cell_cb->setCurrentIndex(menuPos);
                            }
                            menuPos++;
                        }
                    }
                    else if (field.first == "Car_ID") {
                        for (auto model : CarTable::instance().elements()) {
                            cell_cb->addItem(f(model).c_str());
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                cell_cb->setCurrentIndex(menuPos);
                            }
                            menuPos++;
                        }
                    }
                    else if (field.first == "Manager_ID") {
                        for (auto model : ManagerTable::instance().elements()) {
                            cell_cb->addItem(f(model).c_str());
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                cell_cb->setCurrentIndex(menuPos);
                            }
                            menuPos++;
                        }
                    }
                    connect(cell_cb, SIGNAL(currentIndexChanged(int)), mapper, SLOT(map()));
                    connect(mapper, SIGNAL(mappedString(QString)), this, SLOT(onComboChange(QString)));
                    ui->tableWidget->setCellWidget(i, j, cell_cb);
                } else {
                    cell = new QLineEdit(value.second.asString().c_str());
                    cell->setObjectName(QString().asprintf("%d_%d", i, j));

                    connect(cell, SIGNAL(textChanged(QString)), this, SLOT(onDataChange(QString)));

                    if (field.second.Type == tnumber) {
                        if (field.first == "Price") {
                            cell->setText(QString().asprintf("%s", value.second.asString().c_str()).replace(".", ","));
                            cell->setValidator(new QDoubleValidator(0.00, 9999999.99, 2));
                        } else {
                            cell->setValidator(new QIntValidator(0, 99));
                        }
                    }
                    ui->tableWidget->setCellWidget(i, j, cell);
                }

                j++;
            }
            j = 0;
            i++;
        }

    } else {
        ui->tableWidget->setRowCount(0);
        ui->delBtn->setEnabled(false);
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
        print(ManagerTable::instance().elements());
    break;
    case model:
        m = new ModelModel();
        f(m);
        ModelTable::instance().add(*(ModelModel*)m);
        print(ModelTable::instance().elements());
    break;
    case car:
        m = new CarModel();
        f(m);
        CarTable::instance().add(*(CarModel*)m);
        print(CarTable::instance().elements());
    break;
    case carManager:
        m = new CarManagerModel();
        f(m);
        CarManagerTable::instance().add(*(CarManagerModel*)m);
        print(CarManagerTable::instance().elements());
    break;
    }

}

void ViewWindow::on_delBtn_clicked()
{

    if (ui->tableWidget->selectedItems().size() == 0) return;

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
        print(ManagerTable::instance().elements());
    break;
    case model:
        ModelTable::instance().remove(*(ModelModel*)f(ModelTable::instance().elements()));
        print(ModelTable::instance().elements());
    break;
    case car:
        CarTable::instance().remove(*(CarModel*)f(CarTable::instance().elements()));
        print(CarTable::instance().elements());
    break;
    case carManager:
        CarManagerTable::instance().remove(*(CarManagerModel*)f(CarManagerTable::instance().elements()));
        print(CarManagerTable::instance().elements());
    break;
    }

}

void ViewWindow::on_editBox_stateChanged(int arg1)
{
    if (arg1 == 0) {
        ui->tableWidget->setEnabled(false);
    } else {
        ui->tableWidget->setEnabled(true);
    }
}

void ViewWindow::onDataChange(QString data) {
    if (ui->tableWidget->selectedItems().size() == 0) return;
    auto cell = ui->tableWidget->selectedItems().at(0);

    auto getItem = [=](list<Model*> models) -> Model* {
        int i = 0;
        for (auto item : models) {
            if (i == cell->row()) {
                return item;
            }
            i++;
        }
        return nullptr;
    };
    auto getField = [=](map<string, TypeName> fields) -> pair<string, TypeName> {
        int i = 0;
        for (auto field : fields) {
            if (field.first == "ID") continue;
            if (i == cell->column()) {
                return field;
            }
            i++;
        }
        return make_pair("", TypeName());
    };

    Model* m;
    Model* mm;

    switch (type) {
    case manager:
        m = getItem(ManagerTable::instance().elements());
        mm = new ManagerModel;
    break;
    case model:
        m = getItem(ModelTable::instance().elements());
        mm = new ModelModel;
    break;
    case car:
        m = getItem(CarTable::instance().elements());
        mm = new CarModel;
    break;
    case carManager:
        m = getItem(CarManagerTable::instance().elements());
        mm = new CarManagerModel;
    break;
    }

    auto field = getField(m->Fields());


    for (auto value : m->Values()) {
        if (value.first == field.first) {
            switch (value.second.type) {
            case tstring:
                mm->insert(make_pair(value.first, data.toStdString()));
            break;
            case tnumber:
                if (field.first == "Price") {
                    mm->insert(make_pair(value.first, data.replace(",", ".").toDouble()));
                } else {
                    mm->insert(make_pair(value.first, data.toInt()));
                }
            break;
            }
        } else {
            mm->insert(value);
        }
    }

    *m = *mm;

}

void ViewWindow::onComboChange(QString data) {
    QStringList l = data.split("|");
    int col = l.at(0).toInt();
    int row = l.at(1).toInt();
    QComboBox* box = (QComboBox*)ui->tableWidget->cellWidget(row, col);

    auto getItem = [=](list<Model*> models) -> Model* {
        int i = 0;
        for (auto item : models) {
            if (i == row) {
                return item;
            }
            i++;
        }
        return nullptr;
    };
    auto getField = [=](map<string, TypeName> fields) -> pair<string, TypeName> {
        int i = 0;
        for (auto field : fields) {
            if (field.first == "ID") continue;
            if (i == col) {
                return field;
            }
            i++;
        }
        return make_pair("", TypeName());
    };
    auto getItemId = [=](list<Model*> models) -> string {
        int i = 0;
        for (auto item : models) {
            if (i == box->currentIndex()) {
                return item->Values().find("ID")->second.asString();
            }
            i++;
        }
        return nullptr;
    };

    Model* m;
    Model* mm;

    switch (type) {
    case manager:
        m = getItem(ManagerTable::instance().elements());
        mm = new ManagerModel;
    break;
    case model:
        m = getItem(ModelTable::instance().elements());
        mm = new ModelModel;
    break;
    case car:
        m = getItem(CarTable::instance().elements());
        mm = new CarModel;
    break;
    case carManager:
        m = getItem(CarManagerTable::instance().elements());
        mm = new CarManagerModel;
    break;
    }

    auto field = getField(m->Fields());

    for (auto value : m->Values()) {
        if (value.first == field.first) {

            if (field.first == "Model_ID") {
                mm->insert(make_pair(field.first, getItemId(ModelTable::instance().elements())));
            }
            else if (field.first == "Car_ID") {
                mm->insert(make_pair(field.first, getItemId(CarTable::instance().elements())));
            }
            else if (field.first == "Manager_ID") {
                mm->insert(make_pair(field.first, getItemId(ManagerTable::instance().elements())));
            }

        } else {
            mm->insert(value);
        }
    }

    *m = *mm;

}
