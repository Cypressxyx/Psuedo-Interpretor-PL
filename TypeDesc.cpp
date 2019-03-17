#include "TypeDesc.hpp"
#include <iostream>
/* Type Descriptor Super Class */
TypeDescriptor::TypeDescriptor() {}

/* Integer Descriptor Implementation */
IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name, int value) {
	_name = name;
	_value = value;
}

IntegerTypeDescriptor::IntegerTypeDescriptor(std::string name) {
	_name = name;
}

void IntegerTypeDescriptor::print() {
	std::cout << "name:"  << _name  << std::endl;
	std::cout << "value:" << _value << std::endl;
}

std::string IntegerTypeDescriptor::name() {
	return _name;
}

int IntegerTypeDescriptor::intValue() {
	return _value;
}
	


