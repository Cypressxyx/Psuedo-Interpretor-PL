#ifndef TYPEDESC_HPP
#define TYPEDESC_HPP

#include <string>

struct TypeDesc {
	enum types {
		INTEGER,
		STRING
	};	
	TypeDesc(types type);
	types &type();
	virtual ~TypeDesc() {}
	private:
		types _type;
};

struct NumDesc: public TypeDesc {
	NumDesc(types type);	
	union {
		int intVal;
	} value;
	int getVal() { return value.intVal;}
};

struct StrDesc: public TypeDesc {
	StrDesc(types type, std::string val);
	std::string strVal();
	std::string getVal() { return _val;}
	
	private:
		std::string _val;
};
#endif
