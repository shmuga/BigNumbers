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
            a.num[bLength] += 1;
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
        while (a.num[bLength] <= 0 && aLength > bLength) {
            bLength++;
        }
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

BigNumber multiplication(BigNumber X, BigNumber Y) {
//    X = a * 10 ^ (9 * T)  + b
//    Y = c * 10 ^ (9 * T)  + d
//    REZ = a * c * 10 ^ 2 * (9 * T) + ((a + b) * (c + d) - a * c - b * d) * 10 ^ (9 * T) + b * d

    int aLength = (int)X.num.size();
    if (aLength <= 1) { // multiplication of two integers
        return multiplication(X.num[0], Y.num[0]);
    }

//    ac = a * c
    BigNumber a;
    a.num[0] = X.num[aLength - 1];
    BigNumber c;
    c.num[0] = Y.num[aLength - 1];
    BigNumber ac = multiplication(a, c);

//    bd = b * d
    X.num.pop_back(); // b
    Y.num.pop_back(); // d
    BigNumber bd = multiplication(X, Y);

//    xy = x * y = (a + b) * (c + d)
    BigNumber x = sum(a, X); // a + b
    BigNumber y = sum(c, Y); // c + d
    int xyLengthDiff = (int)(x.num.size() - y.num.size());
    if (xyLengthDiff > 0) {
        for (int i = 0; i < xyLengthDiff; i++) {
            y.num.push_back(0);
        }
    } else {
        xyLengthDiff *= -1;
        for (int i = 0; i < xyLengthDiff; i++) {
            x.num.push_back(0);
        }
    }
    BigNumber xy = multiplication(x, y);

//    T
    unsigned long T = (unsigned long)aLength - 1;

//    z = xy - ac - bd
    BigNumber z = sub(xy, ac);
    z = sub(z, bd);

//    z = z * 10 ^ (9 * T)
    z.num.insert(z.num.begin(), T, 0);

//    ac = ac * 10 ^ 2 * (9 * T)
    ac.num.insert(ac.num.begin(), 2 * T, 0);

//    X = ac + z + bd
    X = sum(ac, z);
    X = sum(X, bd);

    return X;
}

BigNumber multiplication(int a, int b) {
    BigNumber x(to_string((long long)a * (long long)b));
    return x;
}

BigNumber mult(BigNumber X, BigNumber Y) {

    int lengthDiff = (int)(X.num.size() - Y.num.size());
    if (lengthDiff > 0) {
        for (int i = 0; i < lengthDiff; i++) {
            Y.num.push_back(0);
        }
    } else {
        lengthDiff *= -1;
        for (int i = 0; i < lengthDiff; i++) {
            X.num.push_back(0);
        }
    }

    X = multiplication(X, Y);
    X.sign = ((bool)(X.sign ^ Y.sign)) ? -1 : 1;
    return X;

}