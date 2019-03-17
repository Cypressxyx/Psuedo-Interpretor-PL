//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
bool debug = true;

void SymTab::setValueFor(std::string vName, int value) {
    // Define a variable by setting its initial value.
		if (debug)
    	std::cout << vName << " <- " << value << std::endl;
    symTab[vName] = value;
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

//TypeDescriptor SymTab::getValueFor(std::string vName) {
int SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
		if (debug)
    	std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;

    return symTab.find(vName)->second;
}

