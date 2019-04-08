//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include"Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _indent{false}, _dedent{false}, _boolExpr{""} {}

void Token::print() const {
    if( eol() ) std::cout <<"EOL";
    else if( eof() )                    std::cout <<"EOF";
    else if( isOpenParen() )  std::cout <<"(";
    else if( isCloseParen() )  std::cout <<")";
    else if( isOpenBracket() )  std::cout <<"{";
    else if( isCloseBracket() )  std::cout <<"}";
    else if( isAssignmentOperator() )  std::cout <<"=";
    else if( isSemiColon() )  std::cout <<";";
    else if( isComma() )  std::cout <<",";
    else if( isColon() )  std::cout <<":";
    else if( isMultiplicationOperator() )  std::cout <<"*";
    else if( isAdditionOperator() )  std::cout <<"+";
    else if( isSubtractionOperator() )  std::cout <<"-";
    else if( isModuloOperator() )  std::cout <<"%";
    else if( isDivisionOperator() )  std::cout <<"/";
		else if ( isAndExpr()) std::cout << "and";
		else if ( isNotExpr()) std::cout << "not";
		else if ( isOrExpr()) std::cout << "or";
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isLessOperator() ) std::cout <<"<";
    else if( isGreaterOperator() ) std::cout <<">";
    else if( isLessThanOrEqual() ) std::cout <<"<=";
    else if( isGreaterThanOrEqual() ) std::cout <<">=";
    else if( isEqual() ) std::cout <<"==";
    else if( isNotEqual() ) std::cout <<"!=";
		else if ( isStr() ) std :: cout << getStr();
		else if ( isIndent() ) std::cout <<"Indent Token";
		else if ( isDedent() ) std::cout <<"Dedent Token";
    else std::cout <<"Uninitialized token.\n";
}
