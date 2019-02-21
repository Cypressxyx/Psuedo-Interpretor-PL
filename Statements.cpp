//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
    for (auto s: _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}
void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

//Print Statement

PrintStatement::PrintStatement() : _Variable(""), value(nullptr) {}
PrintStatement::PrintStatement(std::string Var, ExprNode *tok): _Variable(Var), value(tok) {}

ExprNode *&PrintStatement::valueVariable() {
	return value;
}

void PrintStatement::evaluate(SymTab &symTab){
  //int val = valueVariable()->evaluate(symTab);
  int val = symTab.getValueFor(_Variable);
	std::cout << val << std::endl;
	return ;
}

void PrintStatement::print() {
	std::cout << "print " << _Variable << std::endl;
}


//For Statement
ForStatement::ForStatement() : assignStmt{nullptr}, assignStmtTwo{nullptr}, stmts{nullptr}, exprNode{nullptr} {}
ForStatement::ForStatement(AssignmentStatement *_assignStmt, ExprNode *_exprNode, AssignmentStatement *_assignStmtTwo, Statements *_stmts) {
	assignStmt = _assignStmt;
	exprNode   = _exprNode; assignStmtTwo = _assignStmtTwo;
	stmts = _stmts;
}

AssignmentStatement *&ForStatement::getAssignStmtTwo() {
	return assignStmtTwo;
}

AssignmentStatement *&ForStatement::getAssignStmt() {
	return assignStmt;
}
ExprNode *&ForStatement::getRelExpr() {
	return exprNode;
}

void ForStatement::evaluate(SymTab &symTab) {
	assignStmt->evaluate(symTab);
	while(exprNode->evaluate(symTab)) {
		assignStmtTwo->evaluate(symTab);
		stmts->evaluate(symTab);
	}
};

void ForStatement::print() {
	std::cout << "for ( "; 
	assignStmt->print(); 
	std::cout << "; " ;
	exprNode->print();
	std::cout << "; " ;
	assignStmtTwo->print();
	std::cout << ") {\n";
	stmts->print();
	std::cout << "}\n"; 
}

