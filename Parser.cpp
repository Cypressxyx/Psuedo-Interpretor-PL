
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

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.
    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    while (tok.isName()) {
				if (tok.isKeyword()) { //use catch instead
					if (tok.getName() == "print") {
						PrintStatement *printStmt = printStatement();
						stmts->addStatement(printStmt);
						tok = tokenizer.getToken();
					} else {
						ForStatement *forStmt = forStatement();
						stmts->addStatement(forStmt);
						tok = tokenizer.getToken();
					}
				}
				else {
        	tokenizer.ungetToken();
        	AssignmentStatement *assignStmt = assignStatement();
        	stmts->addStatement(assignStmt);
        	tok = tokenizer.getToken();
				}
    }
    tokenizer.ungetToken();
    return stmts;
}

ForStatement *Parser::forStatement() {
	Token tok = tokenizer.getToken();
	if (!tok.isOpenParen())
		die("Parser::forStatement","Expected a ( token, instead got", tok);
	
	AssignmentStatement *assignStmt = assignStatement();
	tok = tokenizer.getToken();	
	if(!tok.isSemiColon())
		die("Parser::forStatement","Expected a ; token, instead got", tok);
	
	ExprNode *relExprNode = relExpr();
	tok = tokenizer.getToken();	
	if(!tok.isSemiColon())
		die("Parser::forStatement","Expected a ; token, instead got", tok);

	AssignmentStatement *assignStmtTwo = assignStatement();
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
		
	return new ForStatement(assignStmt, relExprNode, assignStmtTwo, stmts);
}

PrintStatement *Parser::printStatement() {
	Token tok = tokenizer.getToken();
	if (!tok.isName())
		die("Parser::assignStatement","Expeceted a name token, instead got", tok);
	ExprNode *value = new Variable(tok);
	return new PrintStatement(tok.getName(), value);
}	

AssignmentStatement *Parser::assignStatement() {
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    //ExprNode *rightHandSideExpr = expr();
    ExprNode *rightHandSideExpr = relExpr();
    //Token tok = tokenizer.getToken();
    //if (!tok.isSemiColon())
        //die("Parser::assignStatement", "Expected a semicolon, instead got", tok);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ExprNode *Parser::relExpr() {
	/* This Function parses the grammar rules
			<relExpr> -> { (==,!=) <relTerm> }
	*/
	ExprNode *left = relTerm();
	Token tok = tokenizer.getToken();
	while (tok.isEqual() || tok.isNotEqual() ) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = left;
		p->right() = relTerm();
		left = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return left;
}

ExprNode *Parser::relTerm() {
	ExprNode *left = relPrimary();
	Token tok = tokenizer.getToken();
	while(tok.isGreaterOperator() || tok.isGreaterThanOrEqual() || tok.isLessOperator() || tok.isLessThanOrEqual()  ) {
		InfixExprNode *p = new InfixExprNode(tok);
		p->left() = left;
		p->right() = relPrimary();
		left = p;
		tok = tokenizer.getToken();
	}
	tokenizer.ungetToken();
	return left;
}

ExprNode *Parser::relPrimary() {
	return expr();
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
		else if( tok.isRelationalOperator() )
				return new RelExpr(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}
