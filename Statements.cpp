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
			std::cout << "Evaluating an assign Statement\n";
		Token rhsTok = rhsExpression()->token();

		if (rhsTok.isName()) { //handle variable assignmenst
			TypeDesc *desc = symTab.getValueFor(rhsTok.getName()); 
			if (desc->type() == TypeDesc::STRING) {
    		auto rhs = rhsExpression()->strEval(symTab);
				symTab.setValueFor(lhsVariable(), rhs);
			} else {	
    		auto rhs = rhsExpression()->evaluate(symTab);
				symTab.setValueFor(lhsVariable(), rhs);
			}
		}

		else if (rhsTok.isStr()) {  //handle string assignments
    	auto rhs = rhsExpression()->strEval(symTab);
			symTab.setValueFor(lhsVariable(), rhs);
		}
		else if (rhsTok.isWholeNumber()) { //handle whole numbers
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
			else { //variable
				if(left->token().isRelationalOperator()) {
					auto rhs = rhsExpression()->evaluate(symTab);
    			symTab.setValueFor(lhsVariable(), rhs);
				} else{
					
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
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

 
ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

 
void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
		bool t = _rhsExpression == nullptr;
   // _rhsExpression->print();
    std::cout << std::endl;
}

//Print Statement
//PrintStatement::PrintStatement() : _Variable(""), value(nullptr) {}
//PrintStatement::PrintStatement(ExprNode *tok) : value{tok} {}
PrintStatement::PrintStatement(std::vector<ExprNode *> *nodes) : _nodes{nodes} {}
//PrintStatement::PrintStatement(std::string Var, ExprNode *tok): _Variable(Var), value(tok) {}
//ExprNode *&PrintStatement::valueVariable() {
	//return value;
//}

template <typename T>
void printDesc(TypeDesc *desc) {
	T *_desc  = dynamic_cast<T *>(desc);
	std::cout << _desc->getVal() ; 	
}

void PrintStatement::evaluate(SymTab &symTab){
	for(int i = 0 ; i < _nodes->size() ; i++) {
		ExprNode *value = _nodes[0][i];
		if (value->token().isName()) {
			TypeDesc *desc = symTab.getValueFor(value->token().getName());
			desc->type() == TypeDesc::INTEGER ? printDesc<NumDesc>(desc) : printDesc<StrDesc>(desc);
		}
		else if (value->token().isArithmeticOperator()) {
			InfixExprNode *node = dynamic_cast<InfixExprNode *>(value);
			if(node->left()->token().isStr()) {
				std::cout << value->strEval(symTab);
			}
			else if(node->left()->token().isWholeNumber()) {
				std::cout << value->evaluate(symTab);
			}
			else { //variable
				TypeDesc *desc = symTab.getValueFor(node->left()->token().getName());
				if(desc->type() == TypeDesc::INTEGER)
					std::cout << value->evaluate(symTab);
				if(desc->type() == TypeDesc::STRING)
					std::cout << value->strEval(symTab);
			}
		}
		else if (value->token().isStr()) {
			std::cout << value->strEval(symTab);
		}
		else 		
			std::cout << value->evaluate(symTab);
		std::cout << " ";
		}
	std::cout << "" << std::endl;
}
 
void PrintStatement::print() {
	//std::cout << "print " << _Variable << std::endl;
}

/*For Statement */
ForStatement::ForStatement() : _seq{nullptr}, _stmts{nullptr} {}
 
ForStatement::ForStatement(ForSequence *seq, Statements *stmts) {
	_seq   = seq;
	_stmts = stmts;
}
 
void ForStatement::evaluate(SymTab &symTab) {
	_seq->initIters(symTab);
	while(_seq->evaluate(symTab)) {
		_seq->next(symTab);
		_stmts->evaluate(symTab);
	}
}
 
void ForStatement::print() {
	std::cout << "for i in range():\n"; 
	_stmts->print();
}

/* If Statement */
IfStmt::IfStmt() : _stmts{nullptr} {}
IfStmt::IfStmt(std::vector<Statements *>*stmts, std::vector<ExprNode *>*relExprs) : _relNodes{relExprs}, _stmts{stmts} {}

void IfStmt::evaluate(SymTab &symTab) {
	int size = (_relNodes[0]).size();
	int numStmts = _stmts->size();
	int idx = 0;
	int res;
	while(size--) {
		res = _relNodes[0][idx]->evaluate(symTab);
		if (res) {
			_stmts[0][idx]->evaluate(symTab);	
			break;
		}
		idx++;
	}
	if (size < 0 && idx != numStmts) { //no evaluating has been done yet
		_stmts[0][numStmts - 1]->evaluate(symTab);
	}
	return ;
}

void IfStmt::print() {
	return;
}

