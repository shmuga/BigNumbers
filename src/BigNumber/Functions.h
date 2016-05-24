//
// Created by axel on 5/7/16.
//

#include "BigNumber.h"

#ifndef BIGNUMBER_FUNCTIONS_H
#define BIGNUMBER_FUNCTIONS_H

using namespace std;

BigNumber addition(BigNumber a, BigNumber b);
BigNumber sum(BigNumber a, BigNumber b);
BigNumber subtraction(BigNumber a, BigNumber b);
BigNumber sub(BigNumber a, BigNumber b);

BigNumber multiplication(BigNumber X, BigNumber Y);
BigNumber multiplication(int a, int b);
BigNumber mult(BigNumber X, BigNumber Y);

#endif //BIGNUMBER_FUNCTIONS_H
