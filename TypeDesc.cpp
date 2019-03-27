#include "TypeDesc.hpp"
#include <iostream>

/* Type Descriptor Super Class */
TypeDesc::TypeDesc(types type): _type{type} {}

//returns the type int or string
TypeDesc::types &TypeDesc::type() { 
	return _type; 
}

/* Type Descriptor for Integers*/
NumDesc::NumDesc(types type): TypeDesc{type} {}

/* Type Descriptor for Strings*/
StrDesc::StrDesc(types type, std::string val): TypeDesc{type}, _val{val} {}

std::string StrDesc::strVal() {
	return _val;
}
