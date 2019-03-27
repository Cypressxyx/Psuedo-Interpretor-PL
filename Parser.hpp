
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ArithExpr.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>

template <typename T>
class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements<T> *statements();

    AssignmentStatement<T> *assignStatement();
    PrintStatement<T>      *printStatement();
    ForStatement<T>        *forStatement();

    ExprNode<T> *relExpr();
    ExprNode<T> *relTerm();
    ExprNode<T> *relPrimary();
    ExprNode<T> *expr();
    ExprNode<T> *term();
    ExprNode<T> *primary();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

