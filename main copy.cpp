#include "Menu.h"

int main() {
    setlocale(LC_ALL, "rus");

    ModelTable::instance().load();
    CarTable::instance().load();
    ManagerTable::instance().load();
    CarManagerTable::instance().load();

    Menu m;

    m.MainMenu();

    return 0;
}
