//
// Created by Dmitriy Steshenko on 14.05.2020.
//

#include "View.h"

pair<std::string, std::string> Token(std::string src) {
    boost::char_separator<char> sep{"."};
    tokenizer tok(src, sep);
    std::string first = "";
    std::string second = "";
    for (auto t = tok.begin(); t != tok.end(); t++) {
        first = *t;
        t++;
        second = *t;
        break;
    }
    return make_pair(first, second);
}

View::View(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield) {
    join(ltable, lfield, rtable, rfield);
}

void View::join(BaseTable& ltable, const std::string& lfield, BaseTable& rtable, const std::string& rfield) {
    JoinFields jf = {&ltable, lfield, &rtable, rfield};
    _joins.push_back(jf);
}

vector<ViewModel*> View::find(std::function<bool(ViewModel*)> filter) {
    vector<ViewModel*> output;
    for (auto &m : Values()) {
        if (filter && !filter(m)) {
            continue;
        }
        output.push_back(m);
    }
    return output;
}

vector<pair<std::string, TypeName>> View::Fields() {
    vector<pair<std::string, TypeName>> f;
    for (auto &i : _joins) {
        for (auto &j : i.leftTable->elements()) {
            for (auto &h : j->Fields()) {
                f.push_back(make_pair(h.first, h.second));
            }
            break;
        }
        for (auto &j : i.rightTable->elements()) {
            for (auto &h : j->Fields()) {
                if (h.first == i.rightField) {
                    f.push_back(make_pair(i.leftField, h.second));
                    continue;
                }
                f.push_back(make_pair(h.first, h.second));
            }
            break;
        }
    }
    return f;
}

vector<ViewModel*> View::Values() {
    vector<ViewModel*> elements;

    for (auto &left : _joins.begin()->leftTable->elements()) {
        map<std::string, TypeName> f;
        map<std::string, ElementValue> v;
        for (auto &fv : left->Fields()) {
            f.insert(make_pair(_joins.begin()->leftTable->name() + "." + fv.first, fv.second));
        }
        for (auto &vv : left->Values()) {
            v.insert(make_pair(_joins.begin()->leftTable->name() + "." + vv.first, vv.second));
        }
        for (auto &join : _joins) {
            auto lid = *(*left)[join.leftField].value.tstring;
            auto vals = join.rightTable->find([join, lid](const Model *el) {return *(*el)[join.rightField].value.tstring == lid;});
            for (auto &m : vals) {
                for (auto &fv : m->Fields()) {
                    f.insert(make_pair(join.rightTable->name() + "." + fv.first, fv.second));
                }
                for (auto &vv : m->Values()) {
                    v.insert(make_pair(join.rightTable->name() + "." + vv.first, vv.second));
                }
                break;
            }
        }
        elements.push_back(new ViewModel(f, v));
    }

    return elements;
}

void View::print(pair<std::string, ElementValue> t) {
    int space = 0;
    cout << "| " << t.second.asString();
    space = t.second.asString().size();
    for (int i = 0; i < 20 - space; i++) cout << " ";
}

void View::print() {
    for (auto &i : Fields()) m_fields.insert(i);
    for (auto &i : Values()) m_values.push_back(i);
    int length = 0;
    for (auto &i : m_fields) {
        if (i.second.Description == "ID") continue;
        length += 22;
    }
    length++;
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;
    for (auto &t : m_printFields) {
        for (auto &i : m_fields) {
            if (Token(t).second == i.first) {
                cout << "| " << i.second.Description;
                int j = i.second.Description.length() / 2;
                for (int a = 0; a < 20 - j; a++) cout << " ";
            }
        }
    }
    cout << "|" << endl;
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;

    for (auto &v : m_values) {
        for (auto &t : m_printFields) {
            for (auto &val : v->Values()) {
                if (val.first == t) {
                    print(val);
                    break;
                }
            }
        }
        cout << "|" << endl;
    }
    for (int i = 0; i < length; i++) cout << "–";
    cout << endl;
}

const map<std::string, TypeName> ViewModel::Fields() const {
    return m_fields;
}

Model *ViewModel::clone() {
    return new ViewModel(*this);
}
