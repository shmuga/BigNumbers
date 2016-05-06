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
    BigNumber(string);
    string print();
};

#endif