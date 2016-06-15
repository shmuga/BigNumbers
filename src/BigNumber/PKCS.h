//
// Created by Mark Orel on 6/16/16.
//

#ifndef PKCS_H
#define PKCS_H

#include "elg.h"

class PublicKey {
private:
    string g;
    string p;
    string y;
public:
    PublicKey(string g, string p, string y) {
        this->p = p;
        this->g = g;
        this->y = y;

    }

    string getP() {
        return this->p;
    }

    string getG() {
        return this->g;
    }

    string getY() {
        return  this->y;
    }
};

class PrivateKey {
private:
    string x;
public:
    PrivateKey(string x) {
        this->x = x;
    }

    string getX() {
        return this->x;
    }

};

class InitCrypto {
private:
    InitCrypto() {}

public:
    static pair<PublicKey, PrivateKey> init() {
        pair<string, string> p = generateP();

        PrivateKey prKey = PrivateKey(generateX(p.first));

        string g = generateG(p.first, p.second);

        PublicKey pKey = PublicKey(
                g,
                p.first,
                generateY(p.first, g, prKey.getX())
        );

        return make_pair(pKey, prKey);
    }
};

class Encrypt {
private:
    Encrypt(){}
public:
    static pair<string, string> start(string msg, PublicKey pub, PrivateKey priv) {
        return ELGcrypt(msg, pub.getP(), pub.getG(), pub.getY(), priv.getX());
    }
};


class Decrypt {
private:
    Decrypt() {}
public:
    static string start(pair<string, string> C, PublicKey pub, PrivateKey priv) {
        return ELGdecrypt(C.second, C.first, priv.getX(), pub.getP());
    }
};


#endif
