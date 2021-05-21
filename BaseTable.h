//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#ifndef FINALPROJECTCPP_BASETABLE_H
#define FINALPROJECTCPP_BASETABLE_H

#include "Model.h"

class BaseTable {
protected:
    std::string m_name;
    std::string m_path;
    list<Model*> m_elements;

    BaseTable(const std::string& name):m_name(name) {}
    virtual ~BaseTable() {}

    pt::ptree saver(const Model& i);
    pt::ptree saver();
    pt::ptree saver(ElementValue i, map<std::string, TypeName> b);

public:
    static std::string searchPath;

    inline const std::string name() { return m_name; }
    inline const list<Model*> elements() { return m_elements; }

    bool save();

    vector<Model*> find(std::function<bool(const Model*)> filter );

};

#endif //FINALPROJECTCPP_BASETABLE_H