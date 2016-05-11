//
// Created by axel on 5/7/16.
//
#include "Functions.h"

BigNumber addition(BigNumber a, BigNumber b) {

    int aLength = (int)a.num.size();
    int bLength = (int)b.num.size();
    int additionalPoint = 0;

    for (int i = 0; i < bLength; i++) {
        a.num[i] += b.num[i] + additionalPoint;
        if (a.num[i] >= 1000000000) {
            a.num[i] -= 1000000000;
            additionalPoint = 1;
        } else {
            additionalPoint = 0;
        }
    }

    if (additionalPoint == 1) {
        if (aLength == bLength) {
            a.num.push_back(additionalPoint);
        } else {
            a.num[bLength + 1] += 1;
        }
    }

    return a;

}

BigNumber sum(BigNumber a, BigNumber b) {

    int aSign = a.sign;
    int bSign = b.sign;

    a.sign = 1;
    b.sign = 1;

    if ((bool)(aSign ^ bSign)) { // different signs, using subtraction
        if (a > b) {
            a = subtraction(a, b);
            a.sign = aSign;
        } else {
            a = subtraction(b, a);
            a.sign = bSign;
        }
    } else {
        if (a.num.size() >= b.num.size()) {
            a = addition(a, b);
        } else {
            a = addition(b, a);
        }
        a.sign = aSign;
    }

    return a;

}

BigNumber subtraction(BigNumber a, BigNumber b) {

    int subtractionPoint = 0;
    int aLength = (int)a.num.size();
    int bLength = (int)b.num.size();

    for (int i = 0; i < bLength; i++) {
        a.num[i] -= b.num[i] + subtractionPoint;
        if (a.num[i] < 0) {
            a.num[i] += 1000000000;
            subtractionPoint = 1;
        } else {
            subtractionPoint = 0;
        }
    }

    if (subtractionPoint && aLength > bLength) {
        while (a.num[++bLength] <= 0 && aLength > bLength) {}
        a.num[bLength] -= 1;
    }

    while (a.num.back() == 0 && --aLength > 0) {
        a.num.pop_back();
    }

    return a;

}

BigNumber sub(BigNumber a, BigNumber b) {

    int aSign = a.sign;
    int bSign = b.sign;

    a.sign = 1;
    b.sign = 1;

    if ((bool)(aSign ^ bSign)) { // different signs, using addition

        if (a.num.size() >= b.num.size()) {
            a = addition(a, b);
        } else {
            a = addition(b, a);
        }
        a.sign = aSign;

    } else {

        if (a > b) {
            a = subtraction(a, b);
            a.sign = aSign;
        } else {
            a = subtraction(b, a);
            a.sign = (int)!aSign;
        }

    }

    return a;

}