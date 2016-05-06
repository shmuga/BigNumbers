//
// Created by axel on 5/7/16.
//
#include "Functions.h"

BigNumber addition(BigNumber a, BigNumber b) {

    int aLength = (int)a.num.size();
    int bLength = (int)b.num.size();
    int diff = aLength - bLength;
    int length = 0;

    if (diff > 0) {
        length = aLength;
        for (int i = 0; i < diff; i++) {
            b.num.push_back(0);
        }
    } else {
        length = bLength;
        diff *= -1;
        for (int i = 0; i < diff; i++) {
            a.num.push_back(0);
        }
    }

    BigNumber result("");

    int sum = 0;
    int additionalPoint = 0;
    for (int i = 0; i < length; i++) {
        sum = a.num[i] + b.num[i] + additionalPoint;
        result.num.push_back(sum % 1000000000);
        additionalPoint = sum / 1000000000;
    }
    if (additionalPoint > 0) {
        result.num.push_back(1);
    }

    return result;

}