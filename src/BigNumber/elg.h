//
// Created by Mark Orel on 6/3/16.
//

#ifndef ELG_H
#define ELG_H

#include "gmpxx.h"
#include <string>
using namespace std;

pair<string, string> generateP();
string generateG(string p, string q);
string generateX(string p);
string generateK(string p, string q);
string generateY(string p, string g, string x);

pair<string, string> ELGcrypt(string M, string p, string g, string y, string x);
string ELGdecrypt(string b, string a, string x, string p);


pair<string, string> ELGsubscribe(string M, pair<string, string> p, string g, string x);
bool ELGcheck(string M, string p, string g, string y, string r, string s);

#endif
