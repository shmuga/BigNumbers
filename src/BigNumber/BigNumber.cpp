#include "BigNumber.h"
#include <string>
#include <cstdlib>
#include <cstdio>

BigNumber::BigNumber(string numString) {
    this->setNum(numString);
}

BigNumber::BigNumber() {
    this->num.push_back(0);
}

BigNumber::BigNumber(int number) {
    this->setNum(to_string(number));
}

BigNumber::BigNumber(const char* number) {
    this->setNum((string) number);
}

string BigNumber::print() {
    const string minus = "-";
    string res;
    char* tmp = new char[9];
    sprintf (tmp, "%d", this->num.empty() ? 0 : this->num.back());
    for (int i=(int)this->num.size()-2; i>=0; --i) {
        char* tmp = new char[9];
        sprintf (tmp, "%09d", this->num[i]);
        res += tmp;
    }

    return (this->sign == -1) ? minus + tmp + res : tmp + res;
}

void BigNumber::setNum(string numString) {
    this->sign = 1;
    if (numString[0] == '-') {
        this->sign = -1;
    }
    for (long i = (int)numString.length(); i>0; i-=9) {
        if (i < 9) {
            this->num.push_back(abs(atoi(numString.substr(0, i).c_str())));
        }
        else {
            this->num.push_back(abs(atoi(numString.substr(i - 9, 9).c_str())));
        }
    }
}

bool operator<(const BigNumber &left, const BigNumber &right) {
    if (left.num.size() > right.num.size()) {
        return false;
    }
    if (left.num.size() < right.num.size()) {
        return true;
    }
    for (long i = left.num.size() - 1; i >= 0; i--) {
        if (left.sign * left.num[i] < right.sign * right.num[i]) return true;
    }

    return false;
}

bool operator<(const int &left, const BigNumber &right) { return BigNumber(left) < right;}
bool operator<(const BigNumber &left, const int &right) { return left < BigNumber(right);}

bool operator>(const BigNumber &left, const BigNumber &right) {  return right < left; }
bool operator>(const int &left, const BigNumber &right) {  return right < BigNumber(left); }
bool operator>(const BigNumber &left, const int &right) {  return BigNumber(right) < left; }

bool operator<=(const BigNumber &left, const BigNumber &right) {  return !(left > right); }
bool operator<=(const int &left, const BigNumber &right) {  return !(BigNumber(left) > right); }
bool operator<=(const BigNumber &left, const int &right) {  return !(left > BigNumber(right)); }

bool operator>=(const BigNumber &left, const BigNumber &right) {  return !(left < right); }
bool operator>=(const int &left, const BigNumber &right) {  return !(BigNumber(left) < right); }
bool operator>=(const BigNumber &left, const int &right) {  return !(left < BigNumber(right)); }


bool operator==(const BigNumber &left, const BigNumber &right) {
    if (left.num.size() > right.num.size() || left.num.size() < right.num.size()) {
        return false;
    }
    for (long i = left.num.size() - 1; i >= 0; i--) {
        if (left.sign * left.num[i] != right.sign * right.num[i]) return false;
    }

    return true;
}
bool operator==(const int &left, const BigNumber &right) { return BigNumber(left) == right;}
bool operator==(const BigNumber &left, const int &right) { return left == BigNumber(right);}


bool operator!=(const BigNumber &left, const BigNumber &right) { return !(left == right);}
bool operator!=(const int &left, const BigNumber &right) { return !(BigNumber(left) == right);}
bool operator!=(const BigNumber &left, const int &right) { return !(left == BigNumber(right));}