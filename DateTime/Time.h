#ifndef SQLENGINE_TIME_H
#define SQLENGINE_TIME_H
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;
class Time {

public:

    //constructor with default value 0
    Time(int h = 0, int m  = 0, int s = 0);
    Time(string time);
    string str() const;
    //setter function
    void setTime(int h, int m, int s);
    //print description of object in hh:mm:ss
    void printTime();
    void operator= (const Time & to_assign);
    Time operator+(const Time & to_add);
    bool operator==(const Time & to_compare);
    bool operator!=(const Time & to_compare);
    bool operator>(const Time & to_compare);
    bool operator<(const Time & to_compare);
    friend ostream& operator<< (ostream& stream, const Time& d);

private:

    int hh;
    int mm;
    int ss;

};


#endif //SQLENGINE_TIME_H
