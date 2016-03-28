#include "BigNumber.h"
#include <string>

BigNumber::BigNumber(string numString) {
    this->sign = 0;
    if (numString[0] == '-') {
        this->sign = 1;
    }
    this->num = new int[numString.length() - this->sign];
    this->length = numString.length() - this->sign;
    for (int i = 0; i < (int) (numString.length() - this->sign); i++) {
        this->num[i] = (int)(numString[i + this->sign] - '0');
    }
}

string BigNumber::print() {
    string ret = "";
    for (int i = 0; i < this->length; i++) {
        ret += std::to_string(this->num[i]);
    }
    return (sign) ? '-' + ret : ret;
}