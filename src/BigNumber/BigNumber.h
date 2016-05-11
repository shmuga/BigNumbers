#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <vector>
#include <string>
    using namespace std;

class BigNumber {
public:
    vector<int> num;
    int sign;
    int base;

public:
    BigNumber();
    BigNumber(const char*);
    BigNumber(int);
    BigNumber(string);

    void setNum(string);
    friend bool operator<(const BigNumber &, const BigNumber &);
    friend bool operator<(const int &, const BigNumber &);
    friend bool operator<(const BigNumber &, const int &);

    friend bool operator>(const BigNumber &, const BigNumber &);
    friend bool operator>(const int &, const BigNumber &);
    friend bool operator>(const BigNumber &, const int&);

    friend bool operator<=(const BigNumber &, const BigNumber &);
    friend bool operator<=(const int &, const BigNumber &);
    friend bool operator<=(const BigNumber &, const int&);

    friend bool operator>=(const BigNumber &, const BigNumber &);
    friend bool operator>=(const int &, const BigNumber &);
    friend bool operator>=(const BigNumber &, const int &);

    friend bool operator==(const BigNumber &, const BigNumber &);
    friend bool operator==(const int &, const BigNumber &);
    friend bool operator==(const BigNumber &, const int &);

    friend bool operator!=(const BigNumber &, const BigNumber &);
    friend bool operator!=(const int &, const BigNumber &);
    friend bool operator!=(const BigNumber &, const int &);

    string print();
};

bool operator<(const BigNumber &, const BigNumber &);
bool operator<(const int&, const BigNumber &);
bool operator<(const BigNumber &, const int&);

bool operator>(const BigNumber &, const BigNumber &);
bool operator>(const int&, const BigNumber &);
bool operator>(const BigNumber &, const int &);

bool operator<=(const BigNumber &, const BigNumber &);
bool operator<=(const int &, const BigNumber &);
bool operator<=(const BigNumber &, const int &);

bool operator>=(const BigNumber &, const BigNumber &);
bool operator>=(const int &, const BigNumber &);
bool operator>=(const BigNumber &, const int &);

bool operator==(const BigNumber &, const BigNumber &);
bool operator==(const int &, const BigNumber &);
bool operator==(const BigNumber &, const int &);

bool operator!=(const BigNumber &, const BigNumber &);
bool operator!=(const int &, const BigNumber &);
bool operator!=(const BigNumber &, const int &);

#endif