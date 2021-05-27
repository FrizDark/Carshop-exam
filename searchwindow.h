#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QMainWindow>

#include "addwindow.h"

namespace Ui {
class SearchWindow;
}

class SearchWindow : public AddWindow
{
    Q_OBJECT

public:
    explicit SearchWindow(QWidget *parent = nullptr);
    explicit SearchWindow(pair<InputStyle*, list<Model*>> data, QWidget *parent = nullptr);
    ~SearchWindow();

private slots:

    void onSearch();

private:
    Ui::SearchWindow *ui;

    map<string, ElementValue> create();

    QTableWidget* table;

    list<Model*> models;

    void print(list<pair<int, Model*>> models);

};

#endif // SEARCHWINDOW_H
