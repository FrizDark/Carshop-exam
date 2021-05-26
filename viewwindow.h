#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>

#include "Table.h"

#include "addwindow.h"

enum ViewType {
    manager, model, car, carManager
};

namespace Ui {
class ViewWindow;
}

class ViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewWindow(QWidget *parent = nullptr);
    explicit ViewWindow(ViewType type, QWidget *parent = nullptr);
    ~ViewWindow();

signals:

    void closed();

private slots:
    void on_exitBtn_clicked();

    void on_addBtn_clicked();

    void getData(map<string, ElementValue>);

    void on_delBtn_clicked();

    void on_editBox_stateChanged(int arg1);

    void onDataChange(QString data);

private:
    Ui::ViewWindow *ui;

    ViewType type;

//    void print(QString elements);
    void print(list<Model*> models);

};

#endif // VIEWWINDOW_H
