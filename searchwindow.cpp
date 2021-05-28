#include "searchwindow.h"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(QWidget *parent) :
    AddWindow(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
}

SearchWindow::SearchWindow(pair<InputStyle*, list<Model*>> data, QWidget *parent) :
    AddWindow(data.first, parent),
    ui(new Ui::SearchWindow)
{

    models = data.second;

    setWindowTitle("Поиск");

    for (auto input : inputs) {
        if (input.second->metaObject()->className() == QString("QComboBox")) {
            auto box = ((QComboBox*)input.second);
            box->insertItem(0, "None");
            box->setCurrentIndex(0);
        }
    }

    disconnect(submit, SIGNAL(clicked()), this, SLOT(onSubmit()));
    connect(submit, SIGNAL(clicked()), this, SLOT(onSearch()));
    submit->setText("Поиск");

    QGridLayout* grid = new QGridLayout;
    table = new QTableWidget;
    grid->addWidget(table, 0, 0);
    QVBoxLayout* vStack = new QVBoxLayout;
    auto widget = new QWidget;
    widget->setLayout(grid);
    vStack->addWidget(widget, 0);
    vStack->addWidget(AddWindow::centralWidget(), 1);
    widget = new QWidget;
    widget->setLayout(vStack);
    setCentralWidget(widget);

}

SearchWindow::~SearchWindow()
{
    delete ui;
    delete table;
}

map<string, ElementValue> SearchWindow::create() {
    map<string, ElementValue> values;
    for (auto input : inputs) {

        string type = input.second->metaObject()->className();

        string str = input.first->objectName().toStdString();
        if (type == "QLineEdit") {
            ElementValue ev = ElementValue(((QLineEdit*)input.second)->text().toStdString());
            if (ev.asString().length() != 0) values.insert(make_pair(str, ev));
        } else if (type == "QSpinBox") {
            ElementValue ev = ElementValue(((QSpinBox*)input.second)->text().toInt());
            if (ev.asString().length() != 0 && ev.asString() != "0") values.insert(make_pair(str, ev));
        } else if (type == "QDoubleSpinBox") {
            ElementValue ev = ElementValue(((QDoubleSpinBox*)input.second)->text().replace(",", ".").toDouble());
            if (ev.asString().length() != 0 && ev.asString() != "0") values.insert(make_pair(str, ev));
        } else if (type == "QComboBox") {
            string selected = ((QComboBox*)input.second)->currentText().toStdString();
            string id;

            for (auto i : menu->items) {
                for (auto j : i.second) {
                    if (j->asString() == selected) {
                        ElementValue ev = ElementValue(j->Values().find("ID")->second);
                        if (selected != "None") values.insert(make_pair(str, ev));
                    }
                }
            }

        }

    }
    return values;
}

void SearchWindow::print(list<pair<int, Model*>> models) {
    int i = 0, j = 0;
    QTableWidgetItem* item;
    QStringList columns;

    auto f = [](Model* m) -> string {
        QStringList list;
        for (auto i : m->Values()) {
            if (m->Fields().find(i.first)->second.Description == "ID") continue;
            list.append(i.second.asString().c_str());
        }
        return list.join(" | ").toStdString();
    };

    for (auto head : menu->fields) {
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
    table->setColumnCount(columns.size());
    table->setHorizontalHeaderLabels(columns);

    table->setRowCount(models.size());
    if (models.size() > 0) {

        for (auto model : models) {
            for (auto value : model.second->Values()) {
                auto field = *menu->fields.find(value.first);
                if (field.first == "ID") continue;

                item = new QTableWidgetItem;
                table->setItem(i, j, item);

                if (field.second.Description == "ID") {
                    if (field.first == "Model_ID") {
                        for (auto model : ModelTable::instance().elements()) {
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                item->setText(f(model).c_str());
                            }
                        }
                    }
                    else if (field.first == "Car_ID") {
                        for (auto model : CarTable::instance().elements()) {
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                item->setText(f(model).c_str());
                            }
                        }
                    }
                    else if (field.first == "Manager_ID") {
                        for (auto model : ManagerTable::instance().elements()) {
                            if (model->Values().find("ID")->second.asString() == value.second.asString()) {
                                item->setText(f(model).c_str());
                            }
                        }
                    }
                } else {
                    item->setText(value.second.asString().c_str());
                }

                j++;
            }
            j = 0;
            i++;
        }

    } else {
        table->setRowCount(0);
    }
}


void SearchWindow::onSearch() {

    int num = 0;

    auto f = [](string s) -> string {
        string result = "";
        for (auto c : s) {
            result += tolower(c);
        }
        return result;
    };

    list<pair<int, Model*>> arr;

    for (auto model : models) {
        for (auto value : create()) {
            if (f(model->Values().find(value.first)->second.asString()) == f(value.second.asString())) {
                num++;
            }
        }
        if (num != 0) {
            arr.push_back(make_pair(num, model));
            num = 0;
        }
    }
    arr.sort([](pair<int, Model*> left, pair<int, Model*> right) {
        return left.first > right.first;
    });

    print(arr);

}
