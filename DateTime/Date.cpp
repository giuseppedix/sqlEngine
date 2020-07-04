#include "Date.h"
#include "../SqlEngine/SqlEngine.h"

Date::Date() {
    time_t t = time(0);
    tm* now = localtime(&t);
    dd = now->tm_mday;
    mm = now->tm_mon + 1;
    yy = now->tm_year + 1900;
}

Date::Date(const Date &to_copy) {
    *this = to_copy;
}

Date::Date(int d, int m, int y) {
    dd = d;
    mm = m;
    yy = y;
}

int Date::day() const {
    return dd;
}

int Date::month() const {
    return mm;
}

int Date::year() const {
    return yy;
}
int Date::yearsFrom(Date d) const {
    return yy - d.yy;
}

string Date::str() const {
    ostringstream stream;

    stream 	<< setfill('0') << setw(2) << dd << "/"
              << setfill('0') << setw(2) << mm << "/"
              << setfill('0') << setw(4) << yy;
    return stream.str();
}

void Date::operator=(const Date &to_assign) {
    dd = to_assign.dd;
    mm = to_assign.mm;
    yy = to_assign.yy;
}

bool Date::operator==(const Date &to_compare) {
    return ((dd == to_compare.dd) &&
            (mm == to_compare.mm) &&
            (yy == to_compare.yy));
}

bool Date::operator!=(const Date &to_compare) {
    return !(*this == to_compare);
}

Date Date::operator+(const Date &to_add) {
    Date ret(to_add);
    ret.dd += dd;
    ret.mm += mm;
    ret.yy += yy;
    return ret;
}
ostream& operator<< (std::ostream& stream, const Date& d) {
    stream << d.str();
    return  stream;
}

bool Date::operator<(const Date &to_compare) {
    bool ret = false;
    if(yy < to_compare.yy){
        ret = true;
    } else if (yy > to_compare.yy){
        ret = false;
    } else if (yy == to_compare.yy){
        if (mm < to_compare.mm){
            ret = true;
        } else if (mm > to_compare.mm){
            ret = false;
        } else if (mm == to_compare.mm){
            ret = dd < to_compare.dd;
        }
    }
    return ret;
}

bool Date::operator>(const Date &to_compare) {
    bool ret = false;
    if(yy > to_compare.yy){
        ret = true;
    } else if (yy < to_compare.yy){
        ret = false;
    } else if (yy == to_compare.yy){
        if (mm > to_compare.mm){
            ret = true;
        } else if (mm < to_compare.mm){
            ret = false;
        } else if (mm == to_compare.mm){
            ret = dd > to_compare.dd;
        }
    }
    return ret;
}

Date::Date(string data_str) {

    vector<string> timeSplitted = SqlEngine::splitValueByDelimiter(data_str, "/");
    dd = stoi(timeSplitted[0]);
    mm = stoi(timeSplitted[1]);
    yy = stoi(timeSplitted[2]);

    try{
        if(dd > 31 || dd < 0){
            throw invalid_argument("ERROR: Invalid day");
        }
        if(mm > 12 || mm < 0){
            throw invalid_argument("ERROR: Invalid month");
        }


    } catch (invalid_argument &exc) {
        cerr << exc.what() << endl;
    }
}
