//
// Created by Dmitriy Steshenko on 28.04.2020.
//

#ifndef FINALPROJECTCPP_DATESTRUCT_H
#define FINALPROJECTCPP_DATESTRUCT_H

#include <iostream>
using namespace std;

struct Date {
    int year = 0;
    int month = 0;
    int day = 0;

    void fillDate() {
        cout << "Год: ";
        cin >> year;
        cout << "Месяц: ";
        cin >> month;
        cout << "День: ";
        cin >> day;
    }
    void setDate(int y, int m, int d){
        year = y; month = m; day = d;
    }
    void getNowDate() {
        time_t tt = time(NULL);
        struct tm *tim = localtime(&tt);
        year = 1900 + tim->tm_year;
        month = 1 + tim->tm_mon;
        day = tim->tm_mday;
    }
    void printDate(){
        cout << year << "." << month / 10 << month%10 << "." << day << endl;
    }
    char* toString() {
        char* str = new char[11];
        sprintf(str, "%d.%d%d.%d%d", year, month / 10,
                month%10, day/10,day%10); return str;
    }
    bool isLeapYear() {
        if (year % 4 == 0) return true;
        else return false;
    }
    int monthDays() {
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) return 31;
        else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        else if (month == 2) {
            if (isLeapYear()) return 29;
            else return 28;
        } else return 0;
    }
    void toNextDate() {
        day++;
        if (day > monthDays()) {
            month++;
            day = 1;
            if (month > 12) {
                year++;
                month = 1;
            }
        }
    }
    void toPrevDate() {
        day--;
        if (day < 1) {
            month--;
            if (month < 1) {
                month = 12;
                year--;
            }
            day = monthDays();
        }
    }
    void addDays(int d) {
        day += d;
        while (day > monthDays()) {
            d = day - monthDays();
            month++;
            day = d;
            if (month > 12) {
                year++;
                month = 1;
            }
        }
    }
    void delDays(int d) {
        day -= d;
        if (day < 1) {
            d = day * -1;
            month--;
            if (month < 1) {
                month = 12;
                year--;
            }
            day = monthDays() - d;
        }
    }
    int cmpDate(Date date) {
        int i = 0;
        int d = day, m = month, y = year;
        while (year > date.year || month > date.month || day > date.day) {
            toPrevDate();
            i--;
        }
        while (year < date.year || month < date.month || day < date.day) {
            toNextDate();
            i++;
        }
        setDate(y, m, d);
        return i;
    }
};

#endif //FINALPROJECTCPP_DATESTRUCT_H
