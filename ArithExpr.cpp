//
// Created by Ali A. Kooshesh on 2/5/19.
// Modified by _ 

#include <iostream>
#include "ArithExpr.hpp"

bool Debug = false;

/*------- ExprNode ----------------*/
template <typename T>
ExprNode<T>::ExprNode(Token token): _token{token} {}

template <typename T>
Token ExprNode<T>::token() { return _token; }

/*------- InfixExprNode functions -------*/
template <typename T>
InfixExprNode<T>::InfixExprNode(Token tk) : ExprNode<T>{tk}, _left(nullptr), _right(nullptr) {}

template <typename T>
ExprNode<T> *&InfixExprNode<T>::left()  { return _left; }

template <typename T>
ExprNode<T> *&InfixExprNode<T>::right() { return _right; }

template <typename T>
T InfixExprNode<T>::evaluate(SymTab &symTab) {
    // Evaluates an infix expression using a post-order traversal of the expression tree.
    int lValue = left()->evaluate(symTab);
    int rValue = right()->evaluate(symTab);
		if ( token().isRelationalOperator() ) {

			if (Debug)
    		std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().relOp() << " " << rValue << std::endl;

			if( token().isLessOperator())
				return lValue < rValue;
			else if( token().isGreaterOperator())	
				return lValue > rValue;
			else if( token().isLessThanOrEqual())
				return lValue <= rValue;
			else if( token().isGreaterThanOrEqual())
				return lValue >= rValue;
			else if( token().isEqual())
				return lValue == rValue;
			else if( token().isNotEqual())
				return lValue != rValue;
		}
		else {
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
        std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
	}
}

template <typename T>
void InfixExprNode<T>::print() {
    _left->print();
    token().print();
    _right->print();
}

/*------- WholeNumber functions -------*/
template <typename T>
WholeNumber<T>::WholeNumber(Token token): ExprNode<T>{token} {}

template <typename T>
void WholeNumber<T>::print() {
    token().print();
}

template <typename T>
T WholeNumber<T>::evaluate(SymTab &symTab) {
		if (Debug)
    	std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    return token().getWholeNumber();
}

// Variable

template <typename T>
Variable<T>::Variable(Token token): ExprNode<T>{token} {}

template <typename T>
void Variable<T>::print() {
    token().print();
}

template <typename T>
T Variable<T>::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
		if (Debug)
    	std::cout << "Variable::evaluate: returning " << symTab.getValueFor(token().getName()) << std::endl;
    return symTab.getValueFor(token().getName());
}

// Relational Expression
template <typename T>
RelExpr<T>::RelExpr(Token token): ExprNode<T>{token} {}

template <typename T>
void RelExpr<T>::print() {
	token().print();
}

template <typename T>
T RelExpr<T>::evaluate(SymTab &symTab) {
	if ( !symTab.isDefined(token().relOp())) {
		std::cout << "Use of undefined variable, " << token().relOp() << std::endl;
		exit(1);
	}
	if (Debug)
		std::cout << "RelOp::evaluate: returning " << symTab.getValueFor(token().relOp()) << std::endl;
	return 1;
}

template class ExprNode<int>;
template class InfixExprNode<int>;
template class WholeNumber<int>;
template class RelExpr<int>;
template class Variable<int>;
