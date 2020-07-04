#ifndef SQLENGINE_DATE_H
#define SQLENGINE_DATE_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

class Date {
public:
    // Dafault Constructor, set the date to the actual date
    Date();
// Constructor, pass the date in the form of dd,mm,yyyy
// Note that the program crashes if the date is not valid...
    Date(int d, int m, int y);
// Copy Constructor
    Date(const Date & to_copy);
// Getters
    int year() const;
    int month() const;
    int day() const;
// return the year from the date d, if d it not passed, it returns the year from now.
// Usefull to compute the age of a person, where this is the person’s birth date
    int yearsFrom(Date d = Date()) const;
// return the date as a string in the form dd/mm/yyyy
// Useful to print on the screen
    string str() const;
// overload of the << operator for the Data class.
// the fried keyword is mandatory to define the overload
    void operator= (const Date & to_assign);
    Date operator+(const Date & to_add);
    bool operator==(const Date & to_compare);
    bool operator!=(const Date & to_compare);
    bool operator>(const Date & to_compare);
    bool operator<(const Date & to_compare);

    friend ostream& operator<< (ostream& stream, const Date& d);
private:
    int yy;
    int mm;
    int dd;
    //int _actual_kind;

};


#endif //SQLENGINE_DATE_H
