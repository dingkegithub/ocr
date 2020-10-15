//
// Created by dk on 12/14/19.
//

#ifndef _BASE64_H
#define _BASE64_H

#include <stdio.h>
#include <iostream>

using namespace std;

class Base64 {

public:
    static string encode(const char* data, int dsize);

    static string decode(const char* data, int dsize, int &osize);
};

#endif
