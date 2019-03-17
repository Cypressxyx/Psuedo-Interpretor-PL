//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ArithExpr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();
    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
};


// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

template <typename T>
class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode<T> *rhsExpr);

    std::string &lhsVariable();
    ExprNode<T> *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode<T> *_rhsExpression;
};

// PrintStatement represents the notion of value being printed to stdin
template <typename T>
class PrintStatement: public Statement {
public:
	PrintStatement();
	PrintStatement(std::string var, ExprNode<T> *);
  ExprNode<T> *&valueVariable();
	virtual void evaluate(SymTab &symTab);
	virtual void print();
private:
	std::string _Variable;
	ExprNode<T> *value;
};

// For Statement represents the notion of a loop
template <typename T>
class ForStatement: public Statement {
public:
	ForStatement();
	ForStatement(AssignmentStatement<T> *, ExprNode<T> *, AssignmentStatement<T> *, Statements *);
  AssignmentStatement<T> *&getAssignStmt();
  AssignmentStatement<T> *&getAssignStmtTwo();
  ExprNode<T> *&getRelExpr();
  Statements *&getSmts();
	//ForStatement(std::string var, exprNode *);
	virtual void evaluate(SymTab &symTab);
	virtual void print();
private:
	AssignmentStatement<T> *assignStmt;
	AssignmentStatement<T> *assignStmtTwo;
	Statements *stmts;
	ExprNode<T> *exprNode;

};

#endif //EXPRINTER_STATEMENTS_HPP
