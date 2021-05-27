#ifndef SQLTABLE_H
#define SQLTABLE_H


#include "Table.h"

#include <QtSql>
#include <QString>
#include <QDebug>

template <typename T = ManagerModel>
class SqlTable: public Table<T> {
private:

    QSqlDatabase db;
    QSqlQuery q;

public:

    SqlTable(string name): Table<T>(name) {
        this->db = QSqlDatabase::addDatabase("QPSQL", QString(this->name().c_str()));
        db.setHostName("127.0.0.1");
        db.setPort(5432);
        db.setDatabaseName("carshop");
        db.setUserName("dmitr");
        db.setPassword("Friz123@");
    }

    ~SqlTable() {
        QSqlDatabase::removeDatabase("db");
    }

    void setDataBase(QSqlDatabase db) {
        this->db = db;
    }

    void load() {

        if (db.open()) {
            q = QSqlQuery(db);

            this->m_elements.clear();
            T *m;

            q.exec(("select * from " + this->name()).c_str());

            while(q.next()) {
                m = new T();
                for (auto field : m->Fields()) {
                    switch (field.second.Type) {
                    case tstring:
                        m->insert(make_pair(field.first, ElementValue(q.value(QString(field.first.c_str())).toString().toStdString())));
                    break;
                    case tnumber:
                        if (field.first == "Price") {
                            m->insert(make_pair(field.first, ElementValue(q.value(QString(field.first.c_str())).toString().replace(",", ".").toDouble())));
                        } else {
                            m->insert(make_pair(field.first, ElementValue(q.value(QString(field.first.c_str())).toInt())));
                        }
                    break;
                    }
                }
                this->m_elements.push_back(new T(*m));
                delete m;
            }
            db.close();
        }

    }

    void save() {

        if (db.open()) {
            q = QSqlQuery(db);

            QStringList fields;
            QStringList items;

            if (this->m_elements.size() > 0) {
                q.exec(("delete from " + this->name()).c_str());

                for (auto field : (*this->m_elements.begin())->Fields()) {
                    fields.append(field.first.c_str());
                }

                for (auto model : this->m_elements) {
                    QStringList values;
                    for (auto value : model->Values()) {
                        switch (value.second.type) {
                        case tstring:
                            values.append(("'" + value.second.asString() + "'").c_str());
                        break;
                        case tnumber:
                            if (value.first == "MobileNumber") {
                                values.append(("'" + value.second.asString() + "'").c_str());
                            } else {
                                values.append(value.second.asString().c_str());
                            }
                        break;
                        }
                    }
                    items.append(values.join(", "));
                }

                string f = fields.join(", ").toStdString();
                string v = items.join("), (").toStdString();

                q.exec(("insert into " + this->name() + "(" + f + ") values(" + v + ");").c_str());

            }
            db.close();
        }

    }

};

class ManagerTable: public SqlTable<ManagerModel> {
public:
    static auto& instance() {
        static ManagerTable table;
        return table;
    }

    ManagerTable():SqlTable<ManagerModel>("Managers") {}
};

class ModelTable: public SqlTable<ModelModel> {
public:
    static auto& instance() {
        static ModelTable table;
        return table;
    }

    ModelTable():SqlTable<ModelModel>("Models") {}
};

class CarTable: public SqlTable<CarModel> {
public:
    static auto& instance() {
        static CarTable table;
        return table;
    }

    CarTable():SqlTable<CarModel>("Cars") {}
};

class CarManagerTable: public SqlTable<CarManagerModel> {
public:
    static auto& instance() {
        static CarManagerTable table;
        return table;
    }

    CarManagerTable():SqlTable<CarManagerModel>("CarManagers") {}
};


#endif // SQLTABLE_H
