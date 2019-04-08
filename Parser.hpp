
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

 
class Parser {
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    Statements *statements();

    AssignmentStatement *assignStatement();
    PrintStatement      *printStatement();
    ForStatement        *forStatement();
    IfStmt              *ifStmt();

    ExprNode *boolExpr();
    ExprNode *andExpr();
    ExprNode *orExpr();
    ExprNode *notExpr();
    ExprNode *relExpr();
    ExprNode *relTerm();
    ExprNode *relPrimary();
    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();
    ExprNode *factor();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

