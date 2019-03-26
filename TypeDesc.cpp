#include "TypeDesc.hpp"
#include <iostream>
/* Type Descriptor Super Class */
TypeDescriptor::TypeDescriptor() {}

/* Integer Descriptor Implementation */
IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name, int value) {
	_name = name;
	_val = value;
}

IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name) {
	_name = name;
}

void IntegerTypeDescriptor::print() {
	std::cout << "name:"  << _name  << std::endl;
	std::cout << "value:" << _val << std::endl;
}

std::string IntegerTypeDescriptor::name() {
	return _name;
}

int IntegerTypeDescriptor::value() {
	return _val;
}

/* String Descriptor Implementation */
StringTypeDescriptor::StringTypeDescriptor(std::string name): _name{name} {}

StringTypeDescriptor::StringTypeDescriptor(std::string name, std::string val): _name{name}, _val{val} {}
	
void StringTypeDescriptor::print() {
	std::cout << "name:" << _name << std::endl;
	std::cout << "value:" << _val << std::endl;
}

std::string StringTypeDescriptor::name() {
	return _name;
}

std::string StringTypeDescriptor::value() {
	return _val;
}


