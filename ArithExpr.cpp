/*
 Created by Ali A. Kooshesh on 2/5/19.
 Modified by _ 
*/

#include <iostream>
#include <string>
#include "ArithExpr.hpp"
#include "TypeDesc.hpp"

bool Debug = false;

/*------- ExprNode ----------------*/
ExprNode::ExprNode(Token token): _token{token} {}
Token ExprNode::token() { return _token; }

/*------- InfixExprNode functions -------*/
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}
ExprNode *&InfixExprNode::left()  { return _left; }
ExprNode *&InfixExprNode::right() { return _right; }

std::string InfixExprNode::strEval(SymTab &symTab) {	
	std::string lValue = left()->strEval(symTab);
	std::string rValue = right()->strEval(symTab);
	if( token().isAdditionOperator() )
 		return lValue + rValue;
	std::cout << "invalid operation on strings";
	exit(1);
	return "";
}

template <typename T>
int evalRelOp(T lValue, T rValue, Token token) {
	if( token.isLessOperator()) 
		return lValue < rValue;
	else if( token.isGreaterOperator())	
		return lValue > rValue;
	else if( token.isLessThanOrEqual())
		return lValue <= rValue;
	else if( token.isGreaterThanOrEqual())
		return lValue >= rValue;
	else if( token.isEqual())
		return lValue == rValue;
	else if( token.isNotEqual())
		return lValue != rValue;
	std::cout <<" invalid relop.exiting\n";	
	exit(1);
}

// Evaluates an infix expression using a post-order traversal of the expression tree.
int InfixExprNode::evaluate(SymTab &symTab) {
		if ( Debug ) 
			std::cout << "evaluating an infix expr\n";

		if ( token().isBoolExpr()) {
			if (token().isNotExpr()) {
				int val = left()->evaluate(symTab);
				return not val;
			}
			int lValue = left()->evaluate(symTab);
			int rValue = right()->evaluate(symTab);
    	if( token().isAndExpr() )
				return lValue && rValue;
			else if ( token().isOrExpr())
				return lValue || rValue;
			else if ( token().isNotExpr())
				return not lValue;
		}

		if ( token().isRelationalOperator() ) {	
			TypeDesc *desc;
			if(!symTab.isDefined(left()->token().getName())) {
				if(left()->token().isStr())
					desc = new TypeDesc(TypeDesc::STRING);
				else
					desc = new TypeDesc(TypeDesc::INTEGER);
			}
			else {
				desc = symTab.getValueFor(left()->token().getName()); 
			}
			if ( desc->type() == TypeDesc::STRING) {
				std::string lValue = left()->strEval(symTab);
				std::string rValue = right()->strEval(symTab);
				return evalRelOp(lValue, rValue, token());
			} else {
				int lValue = left()->evaluate(symTab);
				int rValue = right()->evaluate(symTab);
				return evalRelOp(lValue, rValue, token());
			}
		}

		else {
			int lValue = left()->evaluate(symTab);
			int rValue = right()->evaluate(symTab);
			if (Debug)
    		std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
    	if( token().isAdditionOperator() )
      	return lValue + rValue;
			else if(token().isSubtractionOperator())
     		return lValue - rValue;
			else if(token().isMultiplicationOperator())
     		return lValue * rValue;
			else if(token().isDivisionOperator())
     		return lValue / rValue; // division by zero?
			else if( token().isModuloOperator() )
     		return lValue % rValue;
		 	else {
     		std::cout << "InfixExprNode::evaluate: Invalid Operation";
      	token().print();
        exit(1);
    	}
		}
}
 
void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

/*------- WholeNumber functions -------*/
WholeNumber::WholeNumber(Token token): ExprNode{token} {}

void WholeNumber::print() {
    token().print();
}

std::string WholeNumber::strEval(SymTab &symTab) {	
	return "";
}
 
int WholeNumber::evaluate(SymTab &symTab) {
		if (Debug)
    	std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

/* Variable */
Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

template <typename T>
auto evalHelper(TypeDesc *_desc, SymTab &symTab) {
	T *tDesc = dynamic_cast<T *>(_desc);
	auto value = tDesc->getVal();	
	return value;
}

std::string Variable::strEval(SymTab &symTab) {	
	if( ! symTab.isDefined(token().getName())) {
 		std::cout << "Use of undefined variable, " << token().getName() << std::endl;
 		exit(1);
	}
	TypeDesc *desc = symTab.getValueFor(token().getName());
	std::string value = evalHelper<StrDesc>(desc, symTab);
	if (Debug)
		std::cout << "Variable::evaluate: returning " << value << std::endl;
	return value;
}
 
int Variable::evaluate(SymTab &symTab) {
	if( ! symTab.isDefined(token().getName())) {
		std::cout << "Use of undefined variable, " << token().getName() << std::endl;
		exit(1);
	}
	TypeDesc *desc = symTab.getValueFor(token().getName());
	int value = evalHelper<NumDesc>(desc, symTab);
	if (Debug)
		std::cout << "Variable::evaluate: returning " << value << std::endl;
	return value;
}
/* Boolean Expression */
BoolExpr::BoolExpr(Token token): ExprNode{token} {}

void BoolExpr::print() {
	token().print();
}

std::string BoolExpr::strEval(SymTab &symTab) {	return ""; }

int BoolExpr::evaluate(SymTab &symTab) {
	return 1;
}

/* Relational Expression */
RelExpr::RelExpr(Token token): ExprNode{token} {}

void RelExpr::print() {
	token().print();
}

std::string RelExpr::strEval(SymTab &symTab) {	return ""; }

int RelExpr::evaluate(SymTab &symTab) {
	std::cout << "evluating relexpr\n";
	if ( !symTab.isDefined(token().relOp())) {
		std::cout << "Use of undefined variable, " << token().relOp() << std::endl;
		exit(1);
	}
	//if (Debug)
		//std::cout << "RelOp::evaluate: returning " << symTab.getValueFor(token().relOp()) << std::endl;
	return 1;
}

/* String node */
Str::Str(Token _tok): ExprNode{_tok} {}

void Str::print() {
	token().print();
}

int Str::evaluate(SymTab &symTab) {
	return 1;
}	

std::string Str::strEval(SymTab &symTab) {
	return token().getStr();
}

/* Sequence node for For loops */
ForSequence::ForSequence(Token _tok): ExprNode{_tok}, _iters{nullptr} {}

void ForSequence::print() {
	for (auto i: *_iters)
		i->print();
}

void ForSequence::setIters(std::vector <ExprNode *> *iters) {
	 _iters = iters;
} 

void ForSequence::initIters(SymTab &symTab){
	ExprNode *inc;
	ExprNode *end;
	ExprNode *begin;
	switch(_iters->size()) {
		case 1: //only end was specified
			startVal = 0;
			step     = 1;
			end      = _iters[0][0];
			endVal   = end->evaluate(symTab);
			break;
		case 2: // begin and end specified
	    begin    = _iters[0][0];
			end      = _iters[0][1];
			step     = 1;
			startVal = begin->evaluate(symTab);
			endVal   = end->evaluate(symTab);
			break;
		case 3: //begin,end,step specified
	    begin    = _iters[0][0];
			end      = _iters[0][1];
			inc      = _iters[0][2];
			startVal = begin->evaluate(symTab);
			endVal   = end->evaluate(symTab);
			step     = inc->evaluate(symTab);
			break;
	}
	symTab.setValueFor(token().getName(), startVal);
}

int ForSequence::evaluate(SymTab &symTab) {
  int res;
  if (endVal < 0){
	  res = startVal > endVal;
	}
	else
	  res = startVal < endVal;
	//res ? next(symTab) : symTab.setValueFor(token().getName(), startVal - 1);
	return res;
}

void ForSequence::next(SymTab &symTab) {
	int val = startVal;
	startVal+=step;
	symTab.setValueFor(token().getName(), val);
}

std::string ForSequence::strEval(SymTab &symTab) {return "";}
