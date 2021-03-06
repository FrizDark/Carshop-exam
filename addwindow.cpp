#include "addwindow.h"
#include "ui_addwindow.h"

#include <QDebug>

void AddWindow::generate(map<string, TypeName> fields, function<void(pair<string, TypeName>)> f) {

    setWindowTitle("Добавление");

    auto grid = new QGridLayout;

    inputs = QQueue<pair<QLabel*, QWidget *>>();

    for (auto field : fields) {
        if (field.first != "ID") f(field);
    }

    for (int i = 0; i < inputs.count(); i++) {
        grid->addWidget(inputs[i].first, i, 0);
        grid->addWidget(inputs[i].second, i, 1);
    }

    QPushButton* cancel = new QPushButton("Отмена");
    connect(cancel, SIGNAL(clicked()), this, SLOT(onCancel()));
    grid->addWidget(cancel, inputs.count(), 0);

    submit = new QPushButton("Добавить");
    connect(submit, SIGNAL(clicked()), this, SLOT(onSubmit()));
    grid->addWidget(submit, inputs.count(), 1);

    auto w = new QWidget(this);
    this->setCentralWidget(w);
    w->setLayout(grid);

}


AddWindow::AddWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddWindow)
{
    ui->setupUi(this);
}

AddWindow::AddWindow(InputStyle* is, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AddWindow)
{
    menu = is;

    generate(menu->fields, [=](pair<string, TypeName> field) {

        QWidget * input;

        switch (field.second.Type) {
        case tstring:
            if (field.second.Description == "ID") {

                for (auto i : menu->items) {
                    if (i.first == field.first) {
                        auto menuBox = new QComboBox;

                        for (auto j : i.second) {
                            menuBox->addItem(j->asString().c_str());
                        }

                        input = menuBox;
                    }
                }

            }
            else input = new QLineEdit();
        break;
        case tnumber:
            if (field.first == "Price") {
                input = new QDoubleSpinBox();
                ((QDoubleSpinBox*)input)->setMaximum(999999.99);
            } else {
                input = new QSpinBox();
                ((QSpinBox*)input)->setMaximum(120);
            }
        break;
        }

        auto label = new QLabel();
        if (menu->contains(field.first)) {
            label->setText(menu->id_fields.find(field.first)->second.c_str());
        } else {
            label->setText(field.second.Description.c_str());
        }
        label->setObjectName(field.first.c_str());
        inputs.enqueue(make_pair(label, input));

    });

}

AddWindow::~AddWindow()
{
    delete ui;
    delete submit;
    delete menu;
    for (auto& i : inputs) {
        delete i.first;
        delete i.second;
    }
}

map<string, ElementValue> AddWindow::create() {
    map<string, ElementValue> values;
    for (auto input : inputs) {

        string type = input.second->metaObject()->className();

        string str = input.first->objectName().toStdString();
        if (type == "QLineEdit") {
            ElementValue ev = ElementValue(((QLineEdit*)input.second)->text().toStdString());
            if (ev.asString().length() == 0) {
                QMessageBox::about(this, "Предупреждение", "Заполните все поля!");
                return map<string, ElementValue>();
            }
            values.insert(make_pair(str, ev));
        } else if (type == "QSpinBox") {
            ElementValue ev = ElementValue(((QSpinBox*)input.second)->text().toInt());
            if (ev.asString().length() == 0) {
                QMessageBox::about(this, "Предупреждение", "Заполните все поля!");
                return map<string, ElementValue>();
            }
            values.insert(make_pair(str, ev));
        } else if (type == "QDoubleSpinBox") {
            ElementValue ev = ElementValue(((QDoubleSpinBox*)input.second)->text().replace(",", ".").toDouble());
            if (ev.asString().length() == 0) {
                QMessageBox::about(this, "Предупреждение", "Заполните все поля!");
                return map<string, ElementValue>();
            }
            values.insert(make_pair(str, ev));
        } else if (type == "QComboBox") {
            string selected = ((QComboBox*)input.second)->currentText().toStdString();
            string id;

            for (auto i : menu->items) {
                for (auto j : i.second) {
                    if (j->asString() == selected) {
                        ElementValue ev = ElementValue(j->Values().find("ID")->second);
                        values.insert(make_pair(str, ev));
                    }
                }
            }

        }

    }
    return values;
}

void AddWindow::onSubmit() {
    emit sendData(create());
    this->close();
}

void AddWindow::onCancel() {

    this->close();

}
