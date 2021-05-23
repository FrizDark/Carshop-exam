#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QMainWindow>

#include "View.h"

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

private:
    Ui::ViewWindow *ui;

    ViewType type;

//    void print(list<Model*> elements);
//    void print(View view);

    void print(QString elements);

};

#endif // VIEWWINDOW_H
