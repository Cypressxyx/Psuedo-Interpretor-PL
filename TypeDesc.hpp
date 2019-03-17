
#include <string>

class TypeDescriptor {
	public:
		TypeDescriptor();
		virtual void print() = 0;
		virtual std::string name() = 0;
	private:
};

class IntegerTypeDescriptor: public TypeDescriptor {
	public:
		IntegerTypeDescriptor();
		IntegerTypeDescriptor(std::string , int );
		IntegerTypeDescriptor(std::string );
		virtual std::string name();
		virtual void print();
		int intValue();
	private:
		std::string _name;
		int _value;
};
