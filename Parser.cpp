
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include <vector>
#include <iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions
//template class Parser<float>;
//template class Parser<std::string>;

template <typename T>
void Parser<T>::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

template <typename T>
Statements *Parser<T>::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.
    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while (tok.isName()) {
				if (tok.isKeyword()) { //use catch instead
					if (tok.getName() == "print") {
						PrintStatement<T> *printStmt = printStatement();
						stmts->addStatement(printStmt);
						tok = tokenizer.getToken();
					} else {
						ForStatement<T> *forStmt = forStatement();
						stmts->addStatement(forStmt);
						tok = tokenizer.getToken();
					}
				}
				else {
        	tokenizer.ungetToken();
        	AssignmentStatement<T> *assignStmt = assignStatement();
        	stmts->addStatement(assignStmt);
        	tok = tokenizer.getToken();
				}
    }
    tokenizer.ungetToken();
    return stmts;
}

template <typename T>
ForStatement<T> *Parser<T>::forStatement() {
	Token tok = tokenizer.getToken();
	if (!tok.isOpenParen())
		die("Parser::forStatement","Expected a ( token, instead got", tok);
	
	AssignmentStatement<T> *assignStmt = assignStatement();
	tok = tokenizer.getToken();	
	if(!tok.isSemiColon())
		die("Parser::forStatement","Expected a ; token, instead got", tok);
	
	ExprNode<T> *relExprNode = relExpr();
	tok = tokenizer.getToken();	
	if(!tok.isSemiColon())
		die("Parser::forStatement","Expected a ; token, instead got", tok);

	AssignmentStatement<T> *assignStmtTwo = assignStatement();
	tok = tokenizer.getToken();	
	if (!tok.isCloseParen())
		die("Parser::forStatement","Expected a ) token, instead got", tok);

	tok = tokenizer.getToken();	
	if (!tok.isOpenBracket())
		die("Parser::forStatement","Expected a { token, instead got", tok);

	Statements *stmts = statements();

	tok = tokenizer.getToken();	
	if (!tok.isCloseBracket())
		die("Parser::forStatement","Expected a } token, instead got", tok);
		
	return new ForStatement<T>(assignStmt, relExprNode, assignStmtTwo, stmts);
}

template <typename T>
PrintStatement<T> *Parser<T>::printStatement() {
	Token tok = tokenizer.getToken();
	if (!tok.isName())
		die("Parser::assignStatement","Expeceted a name token, instead got", tok);
	ExprNode<T> *value = new Variable<T>(tok);
	return new PrintStatement<T>(tok.getName(), value);
}	

template <typename T>
AssignmentStatement<T> *Parser<T>::assignStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    //ExprNode *rightHandSideExpr = expr();
    ExprNode<T> *rightHandSideExpr = relExpr();
    //Token tok = tokenizer.getToken();
    //if (!tok.isSemiColon())
        //die("Parser::assignStatement", "Expected a semicolon, instead got", tok);

    return new AssignmentStatement<T>(varName.getName(), rightHandSideExpr);
}

template <typename T>
ExprNode<T> *Parser<T>::relExpr() {
	/* This Function parses the grammar rules
			<relExpr> -> { (==,!=) <relTerm> }
	*/
	ExprNode<T> *left = relTerm();
	Token tok = tokenizer.getToken();
	while (tok.isEqual() || tok.isNotEqual() ) {
		InfixExprNode<T> *p = new InfixExprNode<T>(tok);
		p->left() = left;
		p->right() = relTerm();
		left = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return left;
}
template <typename T>
ExprNode<T> *Parser<T>::relTerm() {
	ExprNode<T> *left = relPrimary();
	Token tok = tokenizer.getToken();
	while(tok.isGreaterOperator() || tok.isGreaterThanOrEqual() || tok.isLessOperator() || tok.isLessThanOrEqual()  ) {
		InfixExprNode<T> *p = new InfixExprNode<T>(tok);
		p->left() = left;
		p->right() = relPrimary();
		left = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return left;
}
template <typename T>
ExprNode<T> *Parser<T>::relPrimary() {
	return expr();
}

template <typename T>
ExprNode<T> *Parser<T>::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode<T> *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode<T> *p = new InfixExprNode<T>(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


template <typename T>
ExprNode<T> *Parser<T>::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode<T> *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode<T> *p = new InfixExprNode<T>(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

template <typename T>
ExprNode<T> *Parser<T>::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber<T>(tok);

    else if( tok.isName() )
        return new Variable<T>(tok);

		else if( tok.isRelationalOperator() )
				return new RelExpr<T>(tok);

    else if (tok.isOpenParen()) {
        ExprNode<T> *p = expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
template class Parser<int>;
