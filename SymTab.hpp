/*
 Created by Ali A. Kooshesh on 2/5/19.
*/

#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include "TypeDesc.hpp"
#include <string>
#include <map>

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

template <typename T>
class SymTab {
public:
    void setValueFor(std::string vName, int value);
    void setValueFor(std::string vName, std::string value);
    bool isDefined(std::string vName);
    //T getValueFor(std::string vName);
		TypeDesc *getValueFor(std::string vName);

private:
    std::map<std::string, TypeDesc *> symTab;
    //std::map<std::string, T> symTab;
};

#endif //EXPRINTER_SYMTAB_HPP
