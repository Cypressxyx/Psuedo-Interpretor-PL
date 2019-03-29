/*
 Created by Ali A. Kooshesh on 1/30/19.
 Modified by 
*/

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {
public:
    Token();

    bool &eof()      { return _eof; }
    bool &eol()      { return _eol; }
    bool eof() const { return _eof; }
    bool eol() const { return _eol; }

    bool isOpenParen()    const { return _symbol == '('; }
    bool isCloseParen()   const { return _symbol == ')'; }
		bool isOpenBracket()  const { return _symbol == '{'; }
    bool isCloseBracket() const { return _symbol == '}'; }

    void symbol(char c) { _symbol = c; }
    char symbol() { return _symbol; }
	
		void relOp(std::string relOp) { _relOp = relOp; }
		void indent() { _indent = true; }
		void dedent() { _dedent = true; }

		std::string relOp() { return _relOp; }

    bool isSemiColon() const { return _symbol == ';'; }
    bool isColon() const { return _symbol == ':'; }
    bool isComma()     const { return _symbol == ','; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }

		bool isLessOperator() const       { return _relOp == "<"  ;  }
		bool isGreaterOperator() const    { return _relOp == ">"  ;  }
		bool isLessThanOrEqual() const    { return _relOp == "<=" ;  }
		bool isGreaterThanOrEqual() const { return _relOp == ">=" ;  }
		bool isEqual() const              { return _relOp == "==" ;  }
		bool isNotEqual() const           { return _relOp == "!=" ;  }

		bool isRelationalOperator() const {
			return	isLessOperator()       ||
							isGreaterOperator()    ||
							isLessThanOrEqual()    ||
							isGreaterThanOrEqual() ||
							isEqual()              ||
							isNotEqual();
							
		}

    bool isName() const                   { return _name.length() > 0; }
		bool isStr()  const										{ return _str.length() > 0;  }
		bool isIndent() const									{ return _indent; }
		bool isDedent() const									{ return _dedent; }
		bool isKeyword() const								{ return _name == "print" || _name == "for"; }
    std::string getName() const           { return _name; }
    std::string getStr() const            { return _str; }
    void setName(std::string n) { _name = n; }
    void setString(std::string n)  { _str  = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

    void print() const;

private:
    char _symbol;
		bool _indent;
		bool _dedent;
    bool _eof, _eol;
    bool _isWholeNumber;
    int _wholeNumber;
		std::string _str;
    std::string _name;
		std::string _relOp;
};

#endif //EXPRINTER_TOKEN_HPP
