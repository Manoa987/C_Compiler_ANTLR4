#include "Types.h"
using namespace std;

Type stringToType(const string& str) {
  if (str == "int") {
    return _INT;
  } else if (str == "char") {
    return _INT;
  } else if (str == "double") {
    return _DOUBLE;
  } else if (str == "void") {
    return _VOID;
  } else {
    // Gérer le cas où la chaîne de caractères n'est pas une valeur valide
    throw std::runtime_error("Type inconnu");
  }
}

TypeAssembleur charToTypeAssembleur(const string& str) {
  if (str == "x86") {
    return x86;
  } else if (str == "arm") {
    return arm;
  } else if (str == "msp430") {
    return msp430;
  } else {
    // Gérer le cas où la chaîne de caractères n'est pas une valeur valide
    throw std::runtime_error("Type inconnu");
  }
}