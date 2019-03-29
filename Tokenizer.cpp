//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"
#include <stack>

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.

    char c;
    std::string name;
    while( inStream.get(c) && (isalnum(c) || c == '_') ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

int Tokenizer::readInteger() {
    // This function is called when it is known that
    // the first character in input is a digit.
    // The function reads and returns all remaining digits.

    int intValue = 0;
    char c;
    while( inStream.get(c) && isdigit(c) ) {
        intValue = intValue * 10 + c - '0';
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

std::string Tokenizer::readString() {
	std::string str = "";
	char c;
	while( inStream.get(c) && c != '"') {  
		str += c;
	}
	if(!inStream.good()) {
		std::cout << "Error reading string. Found EOL. Exiting...\n";
		exit(1);
	}
		
	return str;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingNewLine{true} {
	stack.push(0);
}


int parseIndent(std::ifstream &inStream, bool &parsingNewLine) {
	std::cout << "\n------------------------\n";
	std::cout << "PARSING INDENT\n";
	std::cout << "------------------------\n";
	char c;
	int indentCnt = 0;

	while( inStream.get(c) && isspace(c))
		indentCnt ++;

	inStream.putback(c);
	std::cout << "indent count is: " << indentCnt << std::endl;
	parsingNewLine = false;
	return indentCnt;
}

Token Tokenizer::getToken() {
    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }
    char c;
    Token token;
		
		if ( parsingNewLine ) 
			parseIndent(inStream, parsingNewLine);

    while( inStream.get(c) && isspace(c) ) { 
				if ( c == '\n') {
					int  indentCnt = parseIndent(inStream, parsingNewLine);
					if (indentCnt > 0 || stack.size() >= 2) {
						std::cout << indentCnt << " " << stack.top() << std::endl;
						if (indentCnt == stack.top()) {
							std::cout << "matching indentation parse\n";
							inStream.get(c); //get next char to parse
							break;
						}
						if ( indentCnt > stack.top()){
							stack.push(indentCnt);
							std::cout << stack.top() << std::endl;
							token.indent();
						}
						else {
							while(stack.size()) {
								std::cout << "checking: " << indentCnt << " " << stack.top() << std::endl;
								if ( indentCnt == stack.top() ) {
									if ( stack.top() == 0 ) {
										stack.pop();
										token.dedent();
									}
									else {
										token.dedent();
										break;
									}
									//std::cout << stack.top() << std::endl;
									break;
								} else {
									stack.pop();
								}
							}
							if (stack.size() == 0 && indentCnt > 0) {
								std::cout << "No indentation match. Exiting\n";
								exit(1);
							}
						}
						_tokens.push_back(token);
						return lastToken = token;
					}
				}
		}

    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

	 	if ( c == '#' ){
			while (1) {
				while( c != '\n' ) {
					inStream.get(c);
				}
				inStream.get(c);
				if(c != '#') 
					break;
			}
		} 

    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {
        token.eol() = true;
				parsingNewLine = true;
    } else if( isdigit(c) ) { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber( readInteger() );
    } 
		else if ( c == '>') {
			char temp = c;
			inStream.get(c);
			if ( c == '=' ) 
				token.relOp(">=");
			else {
				inStream.putback(c);
				c = temp;
				std::string relOp(1,c);
				token.relOp(relOp);
			}
		}
		else if ( c == '<') {
			char temp = c;
			inStream.get(c);
			if ( c == '=' ) 
				token.relOp("<=");
			else {
				inStream.putback(c);
				c = temp;
				std::string relOp(1,c);
				token.relOp(relOp);
			}
		}
		else if ( c == '!') {
				char temp = c;
				inStream.get(c);
				if ( c == '=') 
					token.relOp("!=");
				
				else {
					c = temp;
					inStream.putback(c);
				}
		} else if( c == '=' ) {
				char temp = c;
				inStream.get(c);
				if ( c == '=') 
					token.relOp("==");
				
				else {
					inStream.putback(c);
					c = temp;
        	token.symbol(c);
				}
		}
    else if( c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
        token.symbol(c);
    else if( c == ';' || c == ':' || c == ',' || c == '_' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if( c == '{' || c == '}')
        token.symbol(c);
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    } 
		else if ( c == '"' ) {
			token.setString( readString() );
		}
		else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
