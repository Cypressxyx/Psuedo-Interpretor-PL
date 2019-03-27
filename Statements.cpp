/*
   Created by Ali A. Kooshesh on 2/5/19.
*/

#include "Statements.hpp"

// Statement
template <typename T>
Statement<T>::Statement() {}

// Statements
template <typename T>
Statements<T>::Statements() {}

template <typename T>
void Statements<T>::addStatement(Statement<T> *statement) { _statements.push_back(statement); }

template <typename T>
void Statements<T>::print() {
    for (auto s: _statements)
        s->print();
}

template <typename T>
void Statements<T>::evaluate(SymTab<T> &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement
template <typename T>
AssignmentStatement<T>::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

template <typename T>
AssignmentStatement<T>::AssignmentStatement(std::string lhsVar, ExprNode<T> *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

template <typename T>
void AssignmentStatement<T>::evaluate(SymTab<T> &symTab) {
		std::cout << "evaluating\n";
		if (rhsExpression()->token().isStr()) {
			auto value = rhsExpression()->token().getStr();
			symTab.setValueFor(lhsVariable(), value);
		}
		else {
    	auto rhs = rhsExpression()->evaluate(symTab);
    	symTab.setValueFor(lhsVariable(), rhs);
		}
}

template <typename T>
std::string &AssignmentStatement<T>::lhsVariable() {
    return _lhsVariable;
}

template <typename T>
ExprNode<T> *&AssignmentStatement<T>::rhsExpression() {
    return _rhsExpression;
}

template <typename T>
void AssignmentStatement<T>::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

//Print Statement

template <typename T>
PrintStatement<T>::PrintStatement() : _Variable(""), value(nullptr) {}

template <typename T>
PrintStatement<T>::PrintStatement(std::string Var, ExprNode<T> *tok): _Variable(Var), value(tok) {}

template <typename T>
ExprNode<T> *&PrintStatement<T>::valueVariable() {
	return value;
}

template <typename T>
void PrintStatement<T>::evaluate(SymTab<T> &symTab){
	//do the if desc->type is num when adding string support
	TypeDesc *desc    = symTab.getValueFor(_Variable);
	if ( desc->type() == TypeDesc::INTEGER) {
		NumDesc  *numDesc = dynamic_cast<NumDesc *>(desc);
		int val = numDesc->value.intVal;
		std::cout << val << std::endl;
	}
	else if (desc->type() == TypeDesc::STRING) {
		StrDesc  *sDesc = dynamic_cast<StrDesc *>(desc);
		std::string val = sDesc->strVal();
		std::cout << val << std::endl;
	}
	return ;
}

template <typename T>
void PrintStatement<T>::print() {
	std::cout << "print " << _Variable << std::endl;
}


//For Statement
template <typename T>
ForStatement<T>::ForStatement() : assignStmt{nullptr}, assignStmtTwo{nullptr}, stmts{nullptr}, exprNode{nullptr} {}

template <typename T>
ForStatement<T>::ForStatement(AssignmentStatement<T> *_assignStmt, ExprNode<T> *_exprNode, AssignmentStatement<T> *_assignStmtTwo, Statements<T> *_stmts) {
	assignStmt = _assignStmt;
	exprNode   = _exprNode; assignStmtTwo = _assignStmtTwo;
	stmts      = _stmts;
}

template <typename T>
AssignmentStatement<T> *&ForStatement<T>::getAssignStmtTwo() {
	return assignStmtTwo;
}

template <typename T>
AssignmentStatement<T> *&ForStatement<T>::getAssignStmt() {
	return assignStmt;
}

template <typename T>
ExprNode<T> *&ForStatement<T>::getRelExpr() {
	return exprNode;
}

template <typename T>
void ForStatement<T>::evaluate(SymTab<T> &symTab) {
	assignStmt->evaluate(symTab);
	while(exprNode->evaluate(symTab)) {
		assignStmtTwo->evaluate(symTab);
		stmts->evaluate(symTab);
	}
};

template <typename T>
void ForStatement<T>::print() {
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

template class ForStatement<int>;
template class AssignmentStatement<int>;
template class PrintStatement<int>;
template class Statements<int>;
template class Statement<int>;
