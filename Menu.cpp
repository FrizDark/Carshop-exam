//
// Created by Dmitriy Steshenko on 09.05.2020.
//

#include "Menu.h"

void Saver() {
    ModelTable::instance().save();
    CarTable::instance().save();
    ManagerTable::instance().save();
    CarManagerTable::instance().save();
}

void print_Success() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Успех                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_NotFound() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                           Не найдено                            |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_TryAgain() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                          Попробуйте ещё                         |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_Search() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                              Поиск                              |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}
void print_Create() {
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
    cout << "|                             Создать                             |" << endl;
    cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
}

void Menu::MainMenu() {
    bool Exit = true;
    string menu;

    CarModelView cmv;
    CarManagerView cmmv;

    while (Exit) {
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "|                              Главное меню                             |" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1                       Менеджеры | Модели                          2 |" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3                          Машины | Машины и менеджеры              4 |" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5        Показать машины и модели | Показать машины и менеджеров    6 |" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "| 0                               Выход                               0 |" << endl;
        cout << "––––––––––––––––––––––———————————–––-––––––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        if (menu.size() > 1) print_TryAgain();
        else {
            switch (menu[0]) {
                case '0':
                    while (true) {
                        cout << "Сохранить (y, n) -> "; cin >> menu;
                        if (menu == "y") {
                            Saver();
                            break;
                        } else if (menu == "n") {
                            break;
                        } else {
                            print_TryAgain();
                        }
                    }
                    Exit = false;
                    break;
                case '1':
                    ManagerMenu();
                    break;
                case '2':
                    CarModelMenu();
                    break;
                case '3':
                    CarMenu();
                    break;
                case '4':
                    CarManagerMenu();
                    break;
                case '5':
                    cmv.print();
                    break;
                case '6':
                    cmmv.print();
                    break;
                default:
                    print_TryAgain();
            }
        }
    }
}

void Menu::CarModelMenu() {

    bool Exit = true;
    std::string menu;
    ModelModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ModelModel()> Finder = [](){
        print_Search();
        ModelTable::instance().print();
        string s;
        ModelModel m;
        vector<ModelModel> a;
        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) ModelTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = ModelTable::instance().find([i, find](ModelModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = ModelTable::instance().find([i, find](ModelModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                ModelTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ModelModel*()> Adder = [](){
        print_Create();
        random_generator gen;
        string s;
        auto *m = new ModelModel();

        for (auto &i : m->Fields()) {
            if (i.second.Description == "ID") {
                (*m)["ID"] = to_string(gen());
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                             Модель                              |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "–––––––––––––––––––———————————–––-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        if (menu.size() > 1) {
            print_TryAgain();
        } else {
            switch (menu[0]) {
                case '0':
                    Exit = false;
                    break;
                case '1':
                    ModelTable::instance().add(*Adder());
                    break;
                case '2':
                    if (ModelTable::instance().remove(Finder())) {
                        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                        cout << "|                              Успех                              |" << endl;
                        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    } else {
                        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                        cout << "|                           Не найдено                            |" << endl;
                        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
                    }
                    break;
                case '3':
                    m = Finder();
                    s = *m["ID"].value.tstring;
                    m = *Adder();
                    m["ID"] = s;
                    if (ModelTable::instance().update(m)) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '4':
                    ModelTable::instance().print();
                    break;
                case '5':
                    if (ModelTable::instance().save()) {
                        print_Success();
                    }
                    break;
                case '6':
                    if (ModelTable::instance().load()) {
                        print_Success();
                    }
                    break;
                case '7':
                    ModelTable::instance().printM(Finder());
                    break;
                default:
                    print_TryAgain();
            }
        }
    }
}

void Menu::CarMenu() {

    bool Exit = true;
    std::string menu;
    CarModel m;
    string s, field, value;
    int n = 1;
    int pos;

    std::function<CarModel()> Finder = [](){
        print_Search();
        CarTable::instance().print();
        string s;
        CarModel m;
        vector<CarModel> a;

        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) CarTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = CarTable::instance().find([i, find](CarModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = CarTable::instance().find([i, find](CarModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                CarTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<CarModel*(ModelTable carModelTable)> Adder = [](ModelTable carModelTable){
        print_Create();
        random_generator gen;
        string s;
        auto *m = new CarModel();

        ModelModel cmm;

        carModelTable.print();
        vector<ModelModel> a;
        for (auto &i : cmm.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) carModelTable.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = ModelTable::instance().find([i, find](ModelModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = ModelTable::instance().find([i, find](ModelModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                carModelTable.print(&a);
                cmm = a[0];
                break;
            }
            cmm[i.first] = s;
        }
        cmm["ID"] = *a[0]["ID"].value.tstring;
        a.clear();
        s.clear();

        for (auto &i : (*m).Fields()) {
            if (i.second.Description == "ID") {
                if (i.first == "ID") {
                    (*m)["ID"] = to_string(gen());
                } else {
                    (*m)["Model_ID"] = *cmm["ID"].value.tstring;
                }
                continue;
            }
            if (i.first == "Mark") {
                (*m)["Mark"] = *cmm["Mark"].value.tstring;
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                            Автомобиль                           |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "–––––––––––––––––––———————————–––-–––––––––––––––––––––––––––––––––" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        if (menu.size() > 1) {
            print_TryAgain();
        } else {
            switch (menu[0]) {
                case '0':
                    Exit = false;
                    break;
                case '1':
                    CarTable::instance().add(*Adder(ModelTable::instance()));
                    break;
                case '2':
                    if (CarTable::instance().remove(Finder())) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '3':
                    m = Finder();
                    s = *m["ID"].value.tstring;
                    m = *Adder(ModelTable::instance());
                    m["ID"] = s;
                    if (CarTable::instance().update(m)) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '4':
                    CarTable::instance().print();
                    break;
                case '5':
                    if (CarTable::instance().save()) {
                        print_Success();
                    }
                    break;
                case '6':
                    if (ModelTable::instance().elements().empty()) ModelTable::instance().load();
                    if (CarTable::instance().load()) {
                        print_Success();
                    }
                    break;
                case '7':
                    CarTable::instance().printM(Finder());
                    break;
                default:
                    print_TryAgain();
            }
        }
    }
}

void Menu::ManagerMenu() {

    bool Exit = true;
    std::string menu;
    ManagerModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<ManagerModel()> Finder = [](){
        print_Search();
        ManagerTable::instance().print();
        string s;
        ManagerModel m;
        vector<ManagerModel> a;
        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) ManagerTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = ManagerTable::instance().find([i, find](ManagerModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = ManagerTable::instance().find([i, find](ManagerModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                ManagerTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };
    std::function<ManagerModel*()> Adder = [](){
        print_Create();
        random_generator gen;
        string s;
        int n;
        auto *m = new ManagerModel();

        for (auto &i : (*m).Fields()) {
            if (i.second.Description == "ID") {
                (*m)["ID"] = to_string(gen());
                continue;
            }
            cout << i.second.Description << " -> ";
            cin >> s;
            (*m)[i.first] = s;
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                            Менеджер                             |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "–––––––––––––––––––———————————–––-–––––––––––––––––––––––––––––––––" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        if (menu.size() > 1) {
            print_TryAgain();
        } else {
            switch (menu[0]) {
                case '0':
                    Exit = false;
                    break;
                case '1':
                    ManagerTable::instance().add(*Adder());
                    break;
                case '2':
                    if (ManagerTable::instance().remove(Finder())) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '3':
                    m = Finder();
                    s = *m["ID"].value.tstring;
                    m = *Adder();
                    m["ID"] = s;
                    if (ManagerTable::instance().update(m)) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '4':
                    ManagerTable::instance().print();
                    break;
                case '5':
                    if (ManagerTable::instance().save()) {
                        print_Success();
                    }
                    break;
                case '6':
                    if (ManagerTable::instance().load()) {
                        print_Success();
                    }
                    break;
                case '7':
                    ManagerTable::instance().printM(Finder());
                    break;
                default:
                    print_TryAgain();
            }
        }
    }
}

void Menu::CarManagerMenu() {

    bool Exit = true;
    std::string menu;
    CarManagerModel m;
    string s;

    int n = 1;
    int pos = 0;
    string field, value;

    std::function<CarManagerModel()> Finder = [](){
        print_Search();
        CarManagerTable::instance().print();
        string s;
        CarManagerModel m;
        vector<CarManagerModel> a;

        for (auto &i : m.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) CarManagerTable::instance().print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = CarManagerTable::instance().find([i, find](CarManagerModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = CarManagerTable::instance().find([i, find](CarManagerModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                CarManagerTable::instance().print(&a);
                return a[0];
            }
            m[i.first] = s;
        }
        return m;
    };

    std::function<CarManagerModel*(ManagerTable managerTable, CarTable carTable)> Adder = [](ManagerTable managerTable, CarTable carTable){
        print_Create();
        random_generator gen;
        string s;
        auto *m = new CarManagerModel();

        CarModel cm;
        ManagerModel mm;

        carTable.print();
        vector<CarModel> a;
        for (auto &i : cm.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!a.empty()) carTable.print(&a);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                a = CarTable::instance().find([i, find](CarModel el) { return el[i.first].asString() == find.asString(); }, &a);
            } else {
                a = CarTable::instance().find([i, find](CarModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (a.empty()) {
                print_NotFound();
            }
            if (a.size() == 1) {
                carTable.print(&a);
                cm = a[0];
                break;
            }
            cm[i.first] = s;
        }
        cm["ID"] = *a[0]["ID"].value.tstring;
        a.clear();
        s.clear();

        managerTable.print();
        vector<ManagerModel> aa;
        for (auto &i : mm.Fields()) {
            if (i.second.Description == "ID") {
                continue;
            }
            if (!aa.empty()) managerTable.print(&aa);
            cout << i.second.Description << " -> ";
            cin >> s;

            auto find = ElementValue();

            do {
                if (s == "null" || s == "NULL") {
                    find = ElementValue();
                } else if (s == "true" || s == "TRUE") {
                    find = ElementValue(true);
                } else if (s == "false" || s == "FALSE") {
                    find = ElementValue(false);
                } else {
                    std::string::size_type sz;
                    try {
                        find = ElementValue(std::stoi(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    try {
                        find = ElementValue(std::stod(s, &sz));
                        if (sz < s.size()) {
                            throw std::invalid_argument(s);
                        }
                        break;
                    } catch(...) {}
                    find = ElementValue(s);
                    break;
                }
            } while (true);

            if (!a.empty()) {
                aa = ManagerTable::instance().find([i, find](ManagerModel el) { return el[i.first].asString() == find.asString(); }, &aa);
            } else {
                aa = ManagerTable::instance().find([i, find](ManagerModel el) { return el[i.first].asString() == find.asString(); });
            }
            if (aa.empty()) {
                print_NotFound();
            }
            if (aa.size() == 1) {
                managerTable.print(&aa);
                mm = aa[0];
                break;
            }
            mm[i.first] = s;
        }
        mm["ID"] = *aa[0]["ID"].value.tstring;
        aa.clear();
        s.clear();

        for (auto &i : (*m).Fields()) {
            if (i.second.Description == "ID") {
                if (i.first == "Car_ID") {
                    (*m)["Car_ID"] = *cm["ID"].value.tstring;
                } else {
                    (*m)["Manager_ID"] = *mm["ID"].value.tstring;
                }
                continue;
            }
            if (i.first == "LastName") {
                (*m)["LastName"] = *mm["LastName"].value.tstring;
                continue;
            } else if (i.first == "City") {
                (*m)["City"] = *mm["City"].value.tstring;
                continue;
            } else if (i.first == "MobileNumber") {
                (*m)["MobileNumber"] = *mm["MobileNumber"].value.tstring;
                continue;
            } else if (i.first == "Mark") {
                (*m)["Mark"] = *cm["Mark"].value.tstring;
                continue;
            }
        }

        return m;
    };

    while (Exit) {
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "|                         Машины и менеджеры                      |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "–––––––––––––––––––———————————–––-–––––––––––––––––––––––––––––––––" << endl;
        cout << "| 1 |                   Добавить | Удалить                    | 2 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 3 |              Редактировать | Показать                   | 4 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 5 |                  Сохранить | Загрузить                  | 6 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "| 7 |                      Найти | Назад                      | 0 |" << endl;
        cout << "––––––––––––––––––––––———————————-–––––––––––––––––––––––––––––––——" << endl;
        cout << "-> "; cin >> menu;
        if (menu.size() > 1) {
            print_TryAgain();
        } else {
            switch (menu[0]) {
                case '0':
                    Exit = false;
                    break;
                case '1':
                    CarManagerTable::instance().add(*Adder(ManagerTable::instance(), CarTable::instance()));
                    break;
                case '2':
                    if (CarManagerTable::instance().remove(Finder())) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '3':
                    m = Finder();
                    m = *Adder(ManagerTable::instance(), CarTable::instance());
                    if (CarManagerTable::instance().update(m)) {
                        print_Success();
                    } else {
                        print_NotFound();
                    }
                    break;
                case '4':
                    CarManagerTable::instance().print();
                    break;
                case '5':
                    if (CarManagerTable::instance().save()) {
                        print_Success();
                    }
                    break;
                case '6':
                    if (ModelTable::instance().elements().empty()) ModelTable::instance().load();
                    if (CarTable::instance().load()) {
                        print_Success();
                    }
                    break;
                case '7':
                    CarManagerTable::instance().printM(Finder());
                    break;
                default:
                    print_TryAgain();
            }
        }
    }
}
