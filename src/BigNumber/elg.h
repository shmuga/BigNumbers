//
// Created by Mark Orel on 6/3/16.
//

#ifndef ELG_H
#define ELG_H

#include "gmp.h"
#include <string>
using namespace std;

pair<string, string> generateP();
string generateG(string p, string q);
string generateX(string p);
string generateY(string p, string g, string x);

pair<string, string> ELGcrypt(int M, string p, string g, string y, string x);
#endif
