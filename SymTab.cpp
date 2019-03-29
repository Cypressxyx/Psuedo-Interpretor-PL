/*
 Created by Ali A. Kooshesh on 2/5/19.
 Modified by:
*/

#include <iostream>
#include "SymTab.hpp"
bool debug = false;

// Define a variable by setting its initial value to an int
 
void SymTab::setValueFor(std::string vName, int val) {
		if (debug)
    	std::cout << vName << " <- " << val << std::endl;
		NumDesc *desc = new NumDesc(TypeDesc::INTEGER);
		desc->value.intVal = val;
    symTab[vName] = desc;
}

 
void SymTab::setValueFor(std::string vName, std::string val) {
	if (debug)
		std::cout << vName << "<- " << val << std::endl;
	StrDesc *desc = new StrDesc(TypeDesc::STRING, val);
	symTab[vName] = desc;
}

 
bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

 
TypeDesc *SymTab::getValueFor(std::string vName) {
		//int SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
		if (debug) //temporary edit cause i dont want the address to print fix later
    	std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;

    return symTab.find(vName)->second;
}
