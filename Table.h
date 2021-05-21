//
// Created by Dmitriy Steshenko on 30.04.2020.
//

#ifndef FINALPROJECTCPP_TABLE_H
#define FINALPROJECTCPP_TABLE_H

#include "BaseTable.h"

template<class T>
class Table: public BaseTable {
protected:
    void loadObject(T *m, pt::ptree root) {
        T *m1 = new T();
        string objectName;
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tobject) {
                objectName = i.first;
                break;
            }
        }
        if (objectName.empty()) {
            return;
        }
        for(auto &i : root.get_child(objectName)) {
            for (auto &j : i.second) {
                if (i.first == objectName) {
                    loadObject(m1, j.second);
                } else {
                    (*m1).insert(std::make_pair(j.first, j.second.data()));
                }
            }
        }
        (*m)[objectName] = m1;
    }
    void loadArray(T *m, pt::ptree root) {
        T *m1 = new T();
        vector<ElementValue> v;
        string arrayName;
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tarray) {
                arrayName = i.first;
                break;
            }
        }
        if (arrayName.empty()) {
            return;
        }
        for(auto &i : root.get_child(arrayName)) {
            if (i.first == arrayName) {
                loadArray(m1, i.second);
                v.emplace_back((*m1)[i.first].value.tarray);
            } else {
                v.emplace_back(i.second.data());
            }
        }
        (*m)[arrayName] = v;
    }

public:
    Table(const std::string& name):BaseTable(name) {}

//    static auto& instance() {
//        static Table<T> table;
//        return table;
//    }

    inline const std::string name() { return m_name; }

    void add(T& m) {
        m_elements.push_back(&m);
    }
    void print(const Model& m) {
        double buf;
        int space = 0;
        for (auto &t : m.Fields()) {
            if (t.second.Description == "ID") continue;
            space = m[t.first].asString().size();
            cout << "| " << m[t.first].asString();
            for (int i = 0; i < 20 - space; i++) cout << " ";
            space = 0;
        }
        cout << "|" << endl;
    }
    void print(vector<T> *vv = NULL) {
        int length = 0;
        T mm;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            length += 22;
        }
        length++;
        for (int i = 0; i < length; i++) cout << "–";
        cout << endl;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            cout << "| " << i.second.Description;
            int j = i.second.Description.length() / 2;
            for (int a = 0; a < 20 - j; a++) cout << " ";
        }
        cout << "|" << endl;
        for (int i = 0; i < length; i++) cout << "–";
        cout << endl;
        if (vv) {
            for (auto &m : *vv) {
                print(m);
            }
        } else {
            for (auto &m : m_elements) {
                print(*m);
            }
        }
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
    }
    void printM(const Model& m) {
        int length = 0;
        T mm;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            length += 22;
        }
        length++;
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") continue;
            cout << "| " << i.second.Description;
            int j = i.second.Description.length() / 2;
            for (int a = 0; a < 20 - j; a++) cout << " ";
        }
        cout << "|" << endl;
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
        print(m);
        for (int i = 0; i < length; i++) {
            cout << "–";
        }
        cout << endl;
    }
    bool load() {
        pt::ptree root;
        string fileName = searchPath + name() + "Data.json";
        pt::read_json(fileName, root);
        T *m = new T();
        T mm;

        string arrayName = "Array";
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tarray) {
                arrayName = i.first;
                break;
            }
        }

        string objectName = "Object";
        for (auto &i : (*m).Fields()) {
            if (i.second.Type == tobject) {
                objectName = i.first;
                break;
            }
        }

        for(auto &i : root.get_child(name())) {
            for(auto &a : i.second) {
                if (a.first == arrayName) {
                    loadArray(m, i.second);
                } else if (a.first == objectName) {
                    loadObject(m, i.second);
                } else {
                    if (a.second.data() == "null" || a.second.data() == "NULL") {
                        (*m)[a.first] = ElementValue();
                    } else if (a.second.data() == "true" || a.second.data() == "TRUE") {
                        (*m)[a.first] = ElementValue(true);
                    } else if (a.second.data() == "false" || a.second.data() == "FALSE") {
                        (*m)[a.first] = ElementValue(false);
                    } else {
                        std::string::size_type sz;
                        try {
                            auto el = ElementValue(std::stoi(a.second.data(), &sz));
                            if (sz < a.second.data().size()) {
                                throw std::invalid_argument(a.second.data());
                            }
                            (*m)[a.first] = el;
                            continue;
                        } catch(...) {}
                        try {
                            auto el = ElementValue(std::stod(a.second.data(), &sz));
                            if (sz < a.second.data().size()) {
                                throw std::invalid_argument(a.second.data());
                            }
                            (*m)[a.first] = el;
                            continue;
                        } catch(...) {}
                        (*m)[a.first] = ElementValue(a.second.data());
                    }
                }
            }
            m_elements.push_back(new T(*m));
            (*m).clear();
        }
        return true;
    }
    bool remove(T m) {
        int pos = 0;
        bool IsHere = false;
        int ihNum = 0;
        for (auto &i : m_elements) {
            for (auto j : i->Fields()) {
                if (j.second.Description == "ID") continue;
                if (*m[j.first].value.tstring == *(*i)[j.first].value.tstring) {
                    ihNum++;
                }
            }
            if (ihNum == i->Fields().size() - 1) {
                IsHere = true;
                break;
            } else {
                ihNum = 0;
            }
            pos++;
        }
        if (IsHere) {
            auto delPos = m_elements.begin();
            advance(delPos, pos);
            m_elements.erase(delPos);
            return true;
        }
        return false;
    }
    vector<T> find(std::function<bool(const T&)> filter = NULL, vector<T> *vv = NULL) {
        vector<T> output;
        if (vv) {
            for (auto &m : *vv) {
                if (filter && !filter(m)) {
                    continue;
                }
                output.push_back(m);
            }
        } else {
            for (auto &m : m_elements) {
                if (filter && !filter(*((T*)m))) {
                    continue;
                }
                output.push_back(*((T*)m));
            }
        }
        return output;
    }
    bool update(T m) {
        int pos = 0;
        bool IsHere = false;
        for (auto &i : m_elements) {
            if (*m["ID"].value.tstring == *(*i)["ID"].value.tstring) {
                IsHere = true;
                break;
            }
            pos++;
        }
        if (IsHere) {
            int pos2 = 0;
            for (auto &i : m_elements) {
                if (pos2 == pos) {
                    i = new T(m);
                    break;
                }
                pos2++;
            }
            return true;
        }
        return false;
    }
};





class ModelTable: public Table<ModelModel> {
public:
    static auto& instance() {
        static ModelTable table;
        return table;
    }

    ModelTable(): Table<ModelModel>("Models") {}
};

class CarTable: public Table<CarModel> {
public:
    static auto& instance() {
        static CarTable table;
        return table;
    }

    CarTable():Table<CarModel>("Cars") {}
};

class ManagerTable: public Table<ManagerModel> {
public:
    static auto& instance() {
        static ManagerTable table;
        return table;
    }

    ManagerTable():Table<ManagerModel>("Managers") {}
};

class CarManagerTable: public Table<CarManagerModel> {
public:
    static auto& instance() {
        static CarManagerTable table;
        return table;
    }

    CarManagerTable():Table<CarManagerModel>("CarManagers") {}
};

#endif //FINALPROJECTCPP_TABLE_H
