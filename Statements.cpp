/*
   Created by Ali A. Kooshesh on 2/5/19.
*/

#include "Statements.hpp"
bool _Debug = false;

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

AssignmentStatement::AssignmentStatement(std::string lhsVar): _lhsVariable{lhsVar} {}
 
void AssignmentStatement::evaluate(SymTab &symTab) {
		if (_Debug) 
			std::cout << "evaluating\n";

		if (rhsExpression()->token().isName()) { //handle variable assignmenst
			TypeDesc *desc = symTab.getValueFor(rhsExpression()->token().getName()); 
			if (desc->type() == TypeDesc::STRING) {
    		auto rhs = rhsExpression()->strEval(symTab);
				symTab.setValueFor(lhsVariable(), rhs);
			} else {	
    		auto rhs = rhsExpression()->evaluate(symTab);
				symTab.setValueFor(lhsVariable(), rhs);
			}
		}
		else if (rhsExpression()->token().isStr()) {  //handle string assignments
    	auto rhs = rhsExpression()->strEval(symTab);
			symTab.setValueFor(lhsVariable(), rhs);
		}
		else if (rhsExpression()->token().isWholeNumber()) { //handle whole numbers
			auto rhs = rhsExpression()->evaluate(symTab);
			symTab.setValueFor(lhsVariable(), rhs);
		}
		else { //handle infix expr assignment	 
			InfixExprNode *node = dynamic_cast<InfixExprNode *>(rhsExpression());
			ExprNode *left = node->left();
			if ( left->token().isStr()) {
				auto rhs = rhsExpression()->strEval(symTab);
    		symTab.setValueFor(lhsVariable(), rhs);
			} 
			else if ( left->token().isWholeNumber() ) {
				auto rhs = rhsExpression()->evaluate(symTab);
    		symTab.setValueFor(lhsVariable(), rhs);
			}
			else {
				TypeDesc *desc = symTab.getValueFor(left->token().getName());
				if (desc->type() == TypeDesc::STRING && !rhsExpression()->token().isRelationalOperator()) { 
					auto rhs = rhsExpression()->strEval(symTab);
    			symTab.setValueFor(lhsVariable(), rhs);
				} else {
    			auto rhs = rhsExpression()->evaluate(symTab);
    			symTab.setValueFor(lhsVariable(), rhs);
				}
			}
		}
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

 
void PrintStatement::print() {
	std::cout << "print " << _Variable << std::endl;
}


//For Statement
 
ForStatement::ForStatement() : _seq{nullptr}, _stmts{nullptr} {}

 
ForStatement::ForStatement(ForSequence *seq, Statements *stmts) {
	_seq   = seq;
	_stmts = stmts;
}

 
void ForStatement::evaluate(SymTab &symTab) {
	_seq->initIters(symTab);
	//std::cout << _seq->evaluate(symTab) << std::endl;
	while(_seq->evaluate(symTab)) {
		_stmts->evaluate(symTab);
	}
};

 
void ForStatement::print() {
	std::cout << "for i in range():"; 
	//assignStmt->print(); 
	_stmts->print();
}
