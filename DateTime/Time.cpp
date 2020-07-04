#include "Time.h"
#include "../SqlEngine/SqlEngine.h"


Time::Time(int h, int m, int s){
    hh = h;
    mm = m;
    ss = s;
}

void Time::setTime(int h, int m, int s){
    hh = h;
    mm = m;
    ss = s;
}

void Time::printTime(){
    cout << setw(2) << setfill('0') << hh << ":"
         << setw(2) << setfill('0') << mm << ":"
         << setw(2) << setfill('0') << ss << "\n";
}

void Time::operator=(const Time &to_assign) {
    ss = to_assign.ss;
    mm = to_assign.mm;
    hh = to_assign.hh;

}

Time Time::operator+(const Time &to_add) {
    Time ret(to_add);
    ret.ss += ss;
    ret.mm += mm;
    ret.hh += hh;
    return ret;
}

bool Time::operator==(const Time &to_compare) {
    return ((ss == to_compare.ss) &&
            (mm == to_compare.mm) &&
            (hh == to_compare.hh));
}
string Time::str() const {
    ostringstream stream;

    stream 	<< setfill('0') << setw(2) << hh << ":"
              << setfill('0') << setw(2) << mm << ":"
              << setfill('0') << setw(2) << ss;
    return stream.str();
}
ostream& operator<< (std::ostream& stream, const Time& t) {
    stream << t.str();
    return  stream;
}
bool Time::operator!=(const Time &to_compare) {
    return !(*this == to_compare);
}

bool Time::operator>(const Time &to_compare) {
    bool ret = false;
    if(hh > to_compare.hh){
        ret = true;
    } else if (hh < to_compare.hh){
        ret = false;
    } else if (hh == to_compare.hh){
        if (mm > to_compare.mm){
            ret = true;
        } else if (mm < to_compare.mm){
            ret = false;
        } else if (mm == to_compare.mm){
            ret = ss > to_compare.ss;
        }
    }
    return ret;
}

bool Time::operator<(const Time &to_compare) {
    bool ret = false;
    if(hh < to_compare.hh){
        ret = true;
    } else if (hh > to_compare.hh){
        ret = false;
    } else if (hh == to_compare.hh){
        if (mm < to_compare.mm){
            ret = true;
        } else if (mm > to_compare.mm){
            ret = false;
        } else if (mm == to_compare.mm){
            ret = ss < to_compare.ss;
        }
    }
    return ret;
}

Time::Time(string time_str) {
    vector<string> timeSplitted = SqlEngine::splitValueByDelimiter(time_str, ":");
    hh = stoi(timeSplitted[0]);
    mm = stoi(timeSplitted[1]);
    ss = stoi(timeSplitted[2]);

    try{
        if(hh > 23 || hh < 0){
            throw out_of_range("ERROR: Invalid hh");
        }
        if(mm > 59 || mm < 0){
            throw out_of_range("ERROR: Invalid mm");
        }
        if(ss > 59 || ss < 0){
            throw out_of_range("ERROR: Invalid ss");
        }

    } catch (out_of_range &exc) {
        cerr << exc.what() << endl;
    }

}
