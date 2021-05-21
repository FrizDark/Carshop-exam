//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#ifndef FINALPROJECTCPP_VIEW_H
#define FINALPROJECTCPP_VIEW_H

#include "Table.h"

class ViewModel: public Model {
protected:
    map<std::string, TypeName> m_fields;

public:
    ViewModel(map<std::string, TypeName> fields, map<std::string, ElementValue> values) {
        m_fields = fields;
        for (auto &v : values) {
            insert(v);
        }
    }
    ViewModel(const ViewModel& src) {
        m_fields = src.m_fields;
        for (auto &v : src.Values()) {
            insert(v);
        }
    }

    const map<std::string, TypeName> Fields() const;
    Model* clone();
};

class View {
protected:
    struct JoinFields {
        BaseTable *leftTable;
        std::string leftField;
        BaseTable *rightTable;
        std::string rightField;
    };

    list<JoinFields> _joins;

    map<std::string, TypeName> m_fields;
    vector<ViewModel*> m_values;
    vector<std::string> m_printFields;

    void join(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield);

private:
    virtual void print(pair<std::string, ElementValue> t);

public:
    View(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield);

    vector<pair<std::string, TypeName>> Fields();

    vector<ViewModel*> Values();

    virtual void print();
    vector<ViewModel*> find(std::function<bool(ViewModel*)> filter = NULL);
};

class CarModelView: public View {
public:
    CarModelView():View(CarTable::instance(), "Model_ID", ModelTable::instance(), "ID") {
        m_printFields.push_back(CarTable::instance().name() + ".Color");
        m_printFields.push_back(CarTable::instance().name() + ".Price");

        m_printFields.push_back(ModelTable::instance().name() + ".Mark");
        m_printFields.push_back(ModelTable::instance().name() + ".Model");
        m_printFields.push_back(ModelTable::instance().name() + ".Type");
    }
};

class CarManagerView: public View {
public:
    CarManagerView():View(CarManagerTable::instance(), "Car_ID", CarTable::instance(), "ID") {
        join(CarManagerTable::instance(), "Manager_ID", ManagerTable::instance(), "ID");
        m_printFields.push_back(CarTable::instance().name() + ".Mark");
        m_printFields.push_back(CarTable::instance().name() + ".Color");
        m_printFields.push_back(CarTable::instance().name() + ".Price");

        m_printFields.push_back(ManagerTable::instance().name() + ".FirstName");
        m_printFields.push_back(ManagerTable::instance().name() + ".LastName");
        m_printFields.push_back(ManagerTable::instance().name() + ".Age");
        m_printFields.push_back(ManagerTable::instance().name() + ".City");
        m_printFields.push_back(ManagerTable::instance().name() + ".MobileNumber");
    }
};

#endif //FINALPROJECTCPP_VIEW_H
