#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <string>
    using namespace std;

class BigNumber {
public:
    int *num;
    int length;
    int sign;

public:
    BigNumber(string);
    string print();
};

#endif