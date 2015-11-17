/* A recursive descent parser with operator precedence.

   Author: Eric Van Wyk
   Modified: Robert Edge        

   This algorithm is based on the work of Douglas Crockford in "Top
   Down Operator Precedence", a chapter in the book "Beautiful Code".
   Mr. Crockford describes in his chapter how his work is based on an
   algorithm described Vaughan Pratt in "Top Down Operator
   Precedence", presented at the ACM Symposium on Principles of
   Programming Languages.

   Douglas Crockford's chapter is available at 
    http://javascript.crockford.com/tdop/tdop.html

   Vaughan Pratt's paper is available at 
    http://portal.acm.org/citation.cfm?id=512931

   These are both quite interesting works and worth reading if you
   find the problem of parsing to be an interesting one.

   Last modified: Oct 13, 2013.

*/

#include "parser.h"
#include "scanner.h"
#include "extToken.h"
#include <stdio.h>
#include <assert.h>
using namespace std ;

Parser::~Parser() {
    if (s) delete s ;

    ExtToken *extTokenToDelete ;
    currToken = tokens ;
    while (currToken) {
        extTokenToDelete = currToken ;
        currToken = currToken->next ;
        delete extTokenToDelete ;
    }

    Token *tokenToDelete ;
    Token *currScannerToken = stokens ;
    while (currScannerToken) {
        tokenToDelete = currScannerToken ;
        currScannerToken = currScannerToken->next ;
        delete tokenToDelete ;
    }

}


/*! This is the constructor for the parser, it sets up the members that we need to parse the program. In this case, it really may as well be the default constructor with an empty body */
Parser::Parser ( ) { 
    currToken = NULL; prevToken = NULL ; tokens = NULL; 
    s = NULL; stokens = NULL; 
}

/*! Parser obtains the necessary data, by setting up the scanner, extending the token list, etc. */
ParseResult Parser::parse (const char *text) {
    assert (text != NULL) ;
    ParseResult pr ;
    try {
        s = new Scanner() ;
        stokens = s->scan (text) ;        
        tokens = extendTokenList ( this, stokens ) ;    
        assert (tokens != NULL) ;
        currToken = tokens ;
        pr = parseProgram( ) ;
    }
    catch (string errMsg) {
        pr.ok = false ;
        pr.errors = errMsg ;
        pr.ast = NULL ;
    }
    return pr ;
}

/* 
 * parse methods for non-terminal symbols
 * --------------------------------------
 */

/*! parseProgram parses the root of the program, i.e 
main(){statements}, separating the main method from statements inside.
Program ::= varName '(' ')' '{' Stmts '}' 
*/ 
ParseResult Parser::parseProgram () {
    ParseResult pr ;
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    match(leftParen) ;
    match(rightParen) ;
    match(leftCurly);
    ParseResult prStmts = parseStmts() ;
    match(rightCurly);
    match(endOfFile) ;
    
    Varname *nm = new Varname(name);
    pr.ast = new Root(nm,(Stmts*) prStmts.ast);
    
    return pr ;
}


/*! MatrixDecl identical purpose of parseDecl, handles special matrix syntax.*/
ParseResult Parser::parseMatrixDecl () {
    ParseResult pr ;
    match(matrixKwd);
    match(variableName) ;
    string name( prevToken->lexeme ) ;
    Varname *nm = new Varname(name);
    /*! Decl ::= 'Matrix' varName '[' Expr ',' Expr ']' varName ',' varName  '=' Expr ';' */
    if(attemptMatch(leftSquare)){
        ParseResult i = parseExpr(0);
        match(comma);
        ParseResult j = parseExpr(0);
        match(rightSquare);
        ParseResult lvar = parseVariableName();
        match(comma);
        ParseResult rvar = parseVariableName();
        match(assign);
       ParseResult res = parseExpr(0);
		pr.ast = new DeclMatrixOne(nm,(Expr*) i.ast,(Expr*) j.ast,(Varname*) lvar.ast,(Varname*) rvar.ast,(Expr*) res.ast);
    }
    /*! Decl ::= 'Matrix' varName '=' Expr ';' */
    else if(attemptMatch(assign)){
        ParseResult e = parseExpr(0);
        pr.ast = new DeclMatrixTwo(nm,(Expr*) e.ast);
    }
    else{
        throw ( (string) "Bad Syntax of Matrix Decl in in parseMatrixDecl" ) ;
    }   

    match(semiColon) ;

    return pr ;
}
/*! This function parse standardDecl parses the nonmatrix declarations
Decl ::= integerKwd varName | floatKwd varName | stringKwd varName */
ParseResult Parser::parseStandardDecl(){
    ParseResult pr ;
    tokenType t;
    //ParseResult prType = parseType() ;
	
    if ( attemptMatch(intKwd) ) {
        /*! Type ::= intKwd */
      
        t=intKwd;
    } 
    else if ( attemptMatch(floatKwd) ) {
        /*! Type ::= floatKwd */
        t=floatKwd;
    }
    else if ( attemptMatch(stringKwd) ) {
        /*! Type ::= stringKwd */
        t=stringKwd;
    }
    else if ( attemptMatch(boolKwd) ) {
        /*! Type ::= boolKwd */
        t=boolKwd;
    }
    
    match(variableName) ;
    Varname* name = new Varname( prevToken->lexeme ) ; 
    pr.ast = new KwdDecl(t, name);
    match(semiColon) ;
    return pr ;
}

/*! parseDecl is the general declaration, figures out if it should use standard or matrix decl parsing */
ParseResult Parser::parseDecl () {
    ParseResult pr ;
    /*! Decl :: Matrix variableName ....*/
    if(nextIs(matrixKwd)){
        pr =parseMatrixDecl();
    } 
    /*! Decl ::= Type variableName semiColon */
    else{
        pr=parseStandardDecl();
    }
    return pr ;
}



// Stmts
ParseResult Parser::parseStmts () {
    ParseResult pr ;
    if ( ! nextIs(rightCurly) && !nextIs(inKwd)  ) {
        // Stmts ::= Stmt Stmts
        ParseResult prStmt = parseStmt() ;
        ParseResult prStmts = parseStmts() ;
        pr.ast = new StmtsSeq((Stmt*) prStmt.ast, (Stmts*) prStmts.ast);
    }
    else {
        // Stmts ::= 
        // nothing to match.
        pr.ast = new EmptyStmts();
    }
    return pr ;
}


/*! Stmt general case is handled here and handed off to each specific parser function base on type */                                  
ParseResult Parser::parseStmt () {
    ParseResult pr ;

    /*! Stmt ::= Decl */
    if(nextIs(intKwd)||nextIs(floatKwd)||nextIs(matrixKwd)||nextIs(stringKwd)||nextIs(boolKwd)){
        pr=parseDecl();
    }
    /*Stmt ::= '{' Stmts '}'*/
    else if (attemptMatch(leftCurly)){
        ParseResult ps = parseStmts() ; 
	pr.ast=new NestedStmt((Stmts*) ps.ast);
        match(rightCurly);
    }   
    /*! The grammar rules
    Stmt ::= 'if' '(' Expr ')' Stmt 
    Stmt ::= 'if' '(' Expr ')' Stmt 'else' Stmt
    are handled here */
    else if (attemptMatch(ifKwd)){
        match(leftParen);
        ParseResult l = parseExpr(0);
        match(rightParen);
        ParseResult m = parseStmt();
        
        if(attemptMatch(elseKwd)){
            ParseResult r = parseStmt();
			pr.ast = new StmtIfElse((Expr*) l.ast, (Stmt*) m.ast, (Stmt*) r.ast);

        }
		else{
			pr.ast = new StmtIf((Expr*) l.ast, (Stmt*) m.ast);
		}

    }
    /*! Stmt ::= varName '=' Expr ';'  | varName '[' Expr ',' Expr ']' '=' Expr ';' */
    else if  ( attemptMatch (variableName) ) {
	string name( prevToken->lexeme ) ;
	Varname *nm = new Varname(name);
        if (attemptMatch ( leftSquare ) ) {
              ParseResult x = parseExpr(0);
              match ( comma ) ;
              ParseResult y = parseExpr (0) ;
              match  ( rightSquare ) ;
	      match(assign);
              ParseResult d = parseExpr(0);
              match(semiColon);
	      pr.ast=new StmtAsgMatrix(nm, (Expr*) x.ast, (Expr*) y.ast, (Expr*) d.ast);
        }
	else{
              match(assign);
              ParseResult e = parseExpr(0);
              match(semiColon);
	      pr.ast = new StmtAsg(nm, (Expr*) e.ast);
	}

    }
    /*! Stmt ::= 'print' '(' Expr ')' ';' */
    else if ( attemptMatch (printKwd) ) {
        match (leftParen) ;
        ParseResult prpnt = parseExpr(0);
        match (rightParen) ;
        match (semiColon) ;
        pr.ast=new StmtPrint((Expr*) prpnt.ast);
    }
    /*! Stmt ::= 'for' '(' varName '=' Expr ':' Expr ')' Stmt */
    else if ( attemptMatch (forKwd) ) {
        match (leftParen) ;
        match (variableName) ;

	string name( prevToken->lexeme ) ;
	Varname *nm = new Varname(name);

        match (assign) ;
        ParseResult i = parseExpr (0) ;
        match (colon) ;
        ParseResult j = parseExpr (0) ;
        match (rightParen) ;
        ParseResult t = parseStmt () ;
	pr.ast = new StmtFor(nm, (Expr*) i.ast, (Expr*) j.ast, (Stmt*) t.ast);
    }
    /*! Stmt ::= 'while' '(' Expr ')' Stmt */
    else if (attemptMatch(whileKwd)) {
        match(leftParen);
        ParseResult e = parseExpr(0);
        match(rightParen);
        ParseResult s = parseStmt();
	pr.ast=new StmtWhile((Expr*) e.ast, (Stmt*) s.ast);
    }
    /*! Stmt ::= ';  this is a statement that is only a single semicolon. It is not very useful, but still is valid syntax*/
    else if ( attemptMatch (semiColon) ) {
        // parsed a skip
	pr.ast = new Stmtsemic();
    }
    else{
        throw ( makeErrorMsg ( currToken->terminal ) + " while parsing a statement" ) ;
    }
    /*! Stmt ::= variableName assign Expr semiColon */
    return pr ;
}



// Expr
ParseResult Parser::parseExpr (int rbp) {
    /*! Examines current token, without consuming it, to call its
       associated parse methods.  The ExtToken objects have 'nud' and
       'led' methods that are dispatchers that call the appropriate
       parse methods.*/
    ParseResult left = currToken->nud() ;
   
    while (rbp < currToken->lbp() ) {
        left = currToken->led(left) ;
    }
    return left ;
}


/*!
 * parse methods for Expr productions
 * ----------------------------------
 
 These functions parse various types of expressions, such as constants, and language constructs that can evaluate to constants, and variablenames
 */

 /*! Expr ::= trueKwd */
 ParseResult Parser::parseTrueKwd ( ) {
     ParseResult pr ;
     match ( trueKwd ) ;
     string name( prevToken->lexeme );
     pr.ast= new ExprBool(name) ;
 
     return pr ;
 }

 /*! Expr ::= falseKwd */
 ParseResult Parser::parseFalseKwd ( ) {
     ParseResult pr ;
     match ( falseKwd ) ;
     string name( prevToken->lexeme );
     pr.ast= new ExprBool(name) ;

     return pr ;
 }

/*! Expr ::= intConst */
ParseResult Parser::parseIntConst ( ) {
    ParseResult pr ;
    match ( intConst ) ;
    string name( prevToken->lexeme );
    pr.ast= new ExprConstInt(name) ;

    return pr ;
}

/*! Expr ::= floatConst */
ParseResult Parser::parseFloatConst ( ) {
    ParseResult pr ;
    match ( floatConst ) ;
    string name( prevToken->lexeme );
    pr.ast= new ExprConstFloat(name) ;

    return pr ;
}

/*! Expr ::= stringConst */
ParseResult Parser::parseStringConst ( ) {
    ParseResult pr ;
    match ( stringConst ) ;
    string name( prevToken->lexeme );
    pr.ast= new ExprConstStr(name) ;

    return pr ;
}

/*! Expr ::= variableName ..... */
ParseResult Parser::parseVariableName ( ) {
    ParseResult pr ;
    match ( variableName ) ;
    string name( prevToken->lexeme ) ;
    Varname *nm = new Varname(name);
    if(attemptMatch(leftSquare)){
        ParseResult x = parseExpr(0);
        match(comma);
        ParseResult y = parseExpr(0);
        match(rightSquare);
		pr.ast = new ExprMatrixRef(nm, (Expr*) x.ast, (Expr*) y.ast);
    }
    /*! This grammar defines how a nested function call fits into the AST
    !Expr ::= varableName '(' Expr ')' */  
    else if(attemptMatch(leftParen)){
        ParseResult c = parseExpr(0);
        match(rightParen);
	pr.ast = new ExprNestedCall(nm, (Expr*) c.ast);
    }
    /*Expr := variableName */
    else{
       pr.ast = new ExprVar(nm);

    }
    return pr ;
}


/*! Expr ::= leftParen Expr rightParen */
ParseResult Parser::parseNestedExpr ( ) {
    ParseResult pr ;
    match ( leftParen ) ;
    ParseResult r = parseExpr(0) ; 
    match(rightParen) ;
    pr.ast = new ExprNested((Expr*) r.ast);
    return pr ;
}

/*! Expr ::= 'if' Expr 'then' Expr 'else' Expr  */
ParseResult Parser::parseIfExpr(){  
     ParseResult pr ; 
    
    match(ifKwd);
    ParseResult tst = parseExpr(0);
    match(thenKwd);
    ParseResult res = parseExpr(0);
    match(elseKwd);
    ParseResult alt = parseExpr(0);
    pr.ast = new IfExpr((Expr*) tst.ast,(Expr*) res.ast,(Expr*) alt.ast);

    return pr;
}


/*! Expr ::= 'let' Stmts 'in' Expr 'end' */
ParseResult Parser::parseLetExpr(){
   ParseResult pr ;
   match(letKwd);
   ParseResult l = parseStmts();
   match(inKwd);
   ParseResult r = parseExpr(0);
   match(endKwd);
   pr.ast = new LetExpr((Stmts*) l.ast, (Expr*) r.ast);

   return pr;
}

/*! Expr ::= '!' Expr */
ParseResult Parser::parseNotExpr () {
    ParseResult pr ;
    match ( notOp ) ;
    ParseResult p = parseExpr( 0 ); 
    pr.ast = new NotExpr((Expr*) p.ast);

    return pr ;

}
/*! Expr ::= Expr plusSign Expr */
ParseResult Parser::parseAddition ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( plusSign ) ;
    string name( prevToken->lexeme ) ;
    ParseResult p = parseExpr( prevToken->lbp() ); 
    pr.ast = new ExprBinOp((Expr*)prLeft.ast,name, (Expr*)p.ast);

    return pr ;
}

/*! Expr ::= Expr star Expr */
ParseResult Parser::parseMultiplication ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( star ) ;
    string name( prevToken->lexeme ) ;
    ParseResult p = parseExpr( prevToken->lbp() ); 
    pr.ast = new ExprBinOp((Expr*)prLeft.ast,name, (Expr*)p.ast);

    return pr ;
}

/*! Expr ::= Expr dash Expr */
ParseResult Parser::parseSubtraction ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( dash ) ;
    string name( prevToken->lexeme ) ;
    ParseResult p = parseExpr( prevToken->lbp() ); 
    pr.ast = new ExprBinOp((Expr*)prLeft.ast,name, (Expr*)p.ast);

    return pr ;
}

/*! Expr ::= Expr forwardSlash Expr */
ParseResult Parser::parseDivision ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;
    match ( forwardSlash ) ;
    string name( prevToken->lexeme ) ;
    ParseResult p = parseExpr( prevToken->lbp() );
    pr.ast = new ExprBinOp((Expr*)prLeft.ast,name, (Expr*)p.ast);

    return pr ;
}

/*! The boolean ops :

// Expr ::= Expr equalEquals Expr
// Expr ::= Expr lessThanEquals Expr
// Expr ::= Expr greaterThanEquals Expr
// Expr ::= Expr notEquals Expr
// Expr ::= Expr leftAngle Expr
// Expr ::= Expr rightAngle Expr

can all be implemented with one method because the operator can simply operate on both boolean expression on the right or left. With the add, mul, etc ops, we had to separate cases of the binary op, one case was Str, one was Int, etc.

*/

/* Notice that for relational operators we use just one parse
   function.  This shows another possible means for implementing
   expressions, as opposed to the method used for arithmetic
   expressions in which each operation has its own parse method.  It
   will depend on what we do in iteration 3 in building an abstract
   syntax tree to decide which method is better.
 */
 
/*! The following functions are a group of helper functions used by the parser to navigate throught the list of Tokens. */
ParseResult Parser::parseRelationalExpr ( ParseResult prLeft ) {
    // parser has already matched left expression 
    ParseResult pr ;

    nextToken( ) ;
    // just advance token, since examining it in parseExpr caused
    // this method being called.
	string op( prevToken->lexeme ) ;
    ParseResult prRight = parseExpr( prevToken->lbp() ); 
    pr.ast = new ExprBinOp((Expr*) prLeft.ast, op, (Expr*) prRight.ast);
    return pr ;
}


// Helper function used by the parser.

void Parser::match (tokenType tt) {
    if (! attemptMatch(tt)) {
        throw ( makeErrorMsgExpected ( tt ) ) ;
    }
}

bool Parser::attemptMatch (tokenType tt) {
    if (currToken->terminal == tt) { 
        nextToken() ;
        return true ;
    }
    return false ;
}

bool Parser::nextIs (tokenType tt) {
    return currToken->terminal == tt ;
}

void Parser::nextToken () {
    if ( currToken == NULL ) 
        throw ( string("Internal Error: should not call nextToken in unitialized state"));
    else 
    if (currToken->terminal == endOfFile && currToken->next == NULL) {
        prevToken = currToken ;
    } else if (currToken->terminal != endOfFile && currToken->next == NULL) {
        throw ( makeErrorMsg ( "Error: tokens end with endOfFile" ) ) ;
    } else {
        prevToken = currToken ;
        currToken = currToken->next ;
    }
}

string Parser::terminalDescription ( tokenType terminal ) {
    Token *dummyToken = new Token ("",terminal, NULL) ;
    ExtToken *dummyExtToken = extendToken (this, dummyToken) ;
    string s = dummyExtToken->description() ;
    delete dummyToken ;
    delete dummyExtToken ;
    return s ;
}

string Parser::makeErrorMsgExpected ( tokenType terminal ) {
    string s = (string) "Expected " + terminalDescription (terminal) +
        " but found " + currToken->description() ;
    return s ;
}

string Parser::makeErrorMsg ( tokenType terminal ) {
    string s = "Unexpected symbol " + terminalDescription (terminal) ;
    return s ;
}

string Parser::makeErrorMsg ( const char *msg ) {
    return msg ;
}

