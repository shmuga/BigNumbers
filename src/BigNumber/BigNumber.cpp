#include "BigNumber.h"
#include <string>
#include <cstdlib>
#include <cstdio>

BigNumber::BigNumber(string numString) {
    for (int i=(int)numString.length(); i>0; i-=9) {
        if (i < 9) {
            this->num.push_back(atoi(numString.substr(0, i).c_str()));
        }
        else {
            this->num.push_back(atoi(numString.substr(i - 9, 9).c_str()));
        }
    }
}

string BigNumber::print() {
    string res;
    char* tmp = new char[9];
    sprintf (tmp, "%d", this->num.empty() ? 0 : this->num.back());
    for (int i=(int)this->num.size()-2; i>=0; --i) {
        char* tmp = new char[9];
        sprintf (tmp, "%09d", this->num[i]);
        res += tmp;
    }
    return tmp + res;
}