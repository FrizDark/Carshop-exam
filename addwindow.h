#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QMainWindow>

#include "SqlTable.h"

#include <QtWidgets>
#include <QDialog>
#include <QQueue>

namespace Ui {
class AddWindow;
}

struct InputStyle {

    map<string, TypeName> fields;
    map<string, string> id_fields;
    map<string, list<Model*>> items;

    InputStyle();
    InputStyle(map<string, TypeName> f, map<string, string> i_f, map<string, list<Model*>> i) {
        fields = f;
        id_fields = i_f;
        items = i;
    }

    bool contains(string key) {
        for (auto i : items) {
            if (i.first == key) return true;
        }
        return false;
    }
    pair<string, list<Model*>> at(string key) {
        for (const auto& i : items) {
            if (i.first == key) return i;
        }
        return make_pair("", list<Model*>());
    }

};


class AddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    explicit AddWindow(InputStyle* is, QWidget *parent = nullptr);
    ~AddWindow();

private slots:

    void onSubmit();
    void onCancel();

signals:

    void sendData(map<string, ElementValue>);

protected:

    InputStyle* menu;

    QQueue<pair<QLabel*, QWidget *>> inputs;
    QPushButton* submit;

    map<string, ElementValue> create();

private:
    Ui::AddWindow *ui;

    void generate(map<string, TypeName> fields, function<void(pair<string, TypeName>)> f);

};

#endif // ADDWINDOW_H
