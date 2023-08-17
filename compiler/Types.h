#ifndef TYPES_H
#define TYPES_H

#include<iostream>
using namespace std;

    // Définition du type "Type"
    typedef enum {_INT, _CHAR, _DOUBLE, _VOID} Type;
    // Définition du type "TypeAssembleur"
    typedef enum {x86, arm, msp430} TypeAssembleur;

    Type stringToType(const string& str);
    TypeAssembleur charToTypeAssembleur(const string& str);

#endif // TYPES_H