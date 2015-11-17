/*!
	\mainpage AST Used to Parse FCAL Language and Covert it to C++
	
	\section Introduction
		In this AST.h file, we define the context free grammar used to parse the FCAL language. We use inheritance and virtual classes to define productions in the grammar, and concrete subclasses of the virtual classes to define the terminals.
		
	\section How to Use
		Navigate to the currently working directory (the one that this file is stored in) in the terminal, then simply type "make run-tests". This will compile the translator and execute the cxxtests defined in regex_tests.h, scanner_tests.h, parser_tests.h, and AST_tests.h. Once Iteration 4 is complete, there will be a way to execute the program on a specific file, to be inputted by the user.







*/
#ifndef Node_H
#define Node_H

#include <string>
#include <iostream> 
#include <stdio.h>
#include "scanner.h"
/*! These classes are declared here so that they can be fully declared anywhere later and the order doesn't matter*/
class Node ;

class Root ;
class Stmts ;
class Stmt ;
class Expr ;
class Decl ;
class Varname ;

/*! Node is the primary class from which all other classes inherit. Note that it is pure virtual, and thus cannot be instantiated*/
std::string padding();
std::string padding(int i);
class Node {
	public:
		virtual std::string unparse ( )=0;
		virtual std::string cppCode ( )=0;
		~Node() { };
} ;

/*! Root puts the first line in the program, "main(){stmts}" into the syntax tree */
class Root : public Node {
public:
/*! This constructor takes the main function name, and the statements in the FCAL program as its param
	\param *v the varname in the first function of the program, i.e main
	\param *s the sequence of stmts objects which compose the FCAL program */
    Root(Varname *v, Stmts *s) : varname(v), stmts(s) { } ;
/*! We have an unparse string that returns the FCAL string after being parsed, this unparse string is implemented in almost every class*/
    std::string unparse ();
    std::string cppCode ();
private:
    Varname *varname ;
    Stmts *stmts ;
    Root() : varname(NULL), stmts(NULL) { } ;
    Root(const Root &) {};
} ;

// Stmts

class Stmts : public Node {
public:
   virtual std::string unparse()=0;
   virtual std::string cppCode ( )=0;
} ;

//Expr
class Expr : public Node {
public:
   virtual std::string unparse()=0;
   virtual std::string cppCode ( )=0;
} ;

//Stmt (diff from Stmts)
class Stmt : public Node {
public:
virtual std::string unparse()=0;
virtual std::string cppCode ( )=0;
} ;

// Decl
class Decl : public Stmt {
} ;

//empty statements
/*! This class is for empty states for when it reaches the end of the Stmts sequence */
class EmptyStmts : public Stmts {
public:
    EmptyStmts ( ) { } ;
    std::string unparse ();
    std::string cppCode ();
private:
    EmptyStmts(const EmptyStmts &) {} ;
} ;

//sequential statement
/*! This class allows a sequence of many Stmt bundled into a Stmts tree(or branch) */
class StmtsSeq : public Stmts {
public:
    StmtsSeq ( Stmt *_stmt, Stmts *_stmts) : stmt(_stmt), stmts(_stmts) { } ;
    std::string unparse ( );
    std::string cppCode ();
private:
    Stmt *stmt ;
    Stmts *stmts ;
    StmtsSeq () : stmt(NULL), stmts(NULL) { } ;
    StmtsSeq (const StmtsSeq &) {} ;
} ;


// Varname
/*! This class is for a variable name expression */
class Varname : public Expr {
public:
    Varname (std::string _lexeme ) : lexeme(_lexeme) { } ;
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    std::string lexeme ;
    Varname ( ) : lexeme( (std::string) "") { } ;
    Varname (const Varname &) { } ;
} ;

//stddeclaration, like Int q;
/*! This class is for a keyword declaration, like Int q or anything else
    \We use this for all 4 keyword because they are essentially all similarly implemented (string as the keyword)*/
class KwdDecl : public Decl {
public:
    KwdDecl( tokenType _kwd, Varname *_varname) :
    kwd(_kwd), varname(_varname) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    tokenType kwd;
    Varname *varname;
    KwdDecl() :varname(NULL) { } ;
    KwdDecl( const KwdDecl &) { } ;
};

//print
/*! This class is for the print terminal. Note, it does not actually print anything,*/
class StmtPrint: public Stmt {
public:
    StmtPrint (Expr *_expr) : expr(_expr) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Expr *expr ;
    StmtPrint () : expr(NULL) { } ;
    StmtPrint (const StmtPrint &) { } ;

};

//expr str int float
/*! The following three classes individually store either a integer constant, float constant, or string constant
    \We implemented it like this so that is would be easier implement a value() function on binary op later on*/
class ExprConstStr: public Expr {
public:
    ExprConstStr ( std::string _s) : s(_s) { } ;
    std::string unparse ( );
    std::string cppCode ();
private:
    std::string s;
    ExprConstStr () : s(NULL) { };
    ExprConstStr (const ExprConstStr &) { } ;
};
class ExprConstInt: public Expr {
public:
    ExprConstInt ( std::string _s) : s(_s) { } ;
    std::string unparse ( );
    std::string cppCode ();
private:
    std::string s;
    ExprConstInt () : s(NULL) { };
    ExprConstInt (const ExprConstInt &) { } ;
};
class ExprConstFloat: public Expr {
public:
    ExprConstFloat ( std::string _s) : s(_s) { } ;
    std::string unparse ( );
    std::string cppCode ();
private:
    std::string s;
    ExprConstFloat () : s(NULL) { };
    ExprConstFloat (const ExprConstFloat &) { } ;class DeclMatrixOne: public Decl { //# 20 from grammar.txt
public:
/*! The Matrix declaration takes in 6 parameters which define an entry in the matrix
	\param *_v the name of the matrix
	\param Expr *_e an index bound
	\param Expr *_ee another index bound
	\param Varname *_vv index
	\param Varname *_vvv another index
*/
    DeclMatrixOne( Varname *_v, Expr *_e, Expr *_ee, Varname *_vv, Varname *_vvv, Expr *_eee):
    v(_v), e(_e), ee(_ee), vv(_vv), vvv(_vvv), eee(_eee) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Varname *v;
    Expr *e;
    Expr *ee;
    Varname *vv;
    Varname *vvv;
    Expr *eee;
    DeclMatrixOne(): v(NULL), e(NULL), ee(NULL), vv(NULL), vvv(NULL), eee(NULL) { };
    DeclMatrixOne( const DeclMatrixOne &) { };
};
};

//exprvar
/*! This class is for a variable name expression*/
class ExprVar: public Expr {
public:
    ExprVar ( Varname *_v) : v(_v) { };
    std::string unparse( );
    std::string cppCode ();
private:
    Varname *v;
    ExprVar() : v(NULL) { } ;
    ExprVar(const ExprVar &) { } ;
};

//exprbool
/*! This class is for a boolean expression*/
class ExprBool: public Expr {
public:
    ExprBool (std::string _s) : s(_s) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    std::string s;
    ExprBool () : s(NULL) { };
    ExprBool (const ExprBool &) { } ;
};

//binop
/*! This class is for binary operation expressions. These include math operators as well since they all have similar implementation*/
class ExprBinOp : public Expr { // from num 27-38 from grammar.txt
public:
    ExprBinOp ( Expr *_left, std::string _op, Expr *_right)
    : left(_left), op(_op), right(_right) { } ;
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Expr *left ;
    std::string op ;
    Expr *right ;
    ExprBinOp() : left(NULL), op(NULL), right(NULL) { } ;
    ExprBinOp(const ExprBinOp &) { } ;
} ;

//not op
/*! This class is for a 'not' expression */
class NotExpr : public Expr {
public:
    NotExpr( Expr *_expr) : expr(_expr) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Expr *expr ;
    NotExpr () : expr(NULL) { };
    NotExpr (const NotExpr &) { };
};
 //let                             
/*! This class is for a let expression*/
class LetExpr : public Expr {
public:
    LetExpr( Stmts *_s, Expr *_expr) :
    s(_s), expr(_expr) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Stmts *s ;
    Expr *expr ;
    LetExpr () : s(NULL), expr(NULL) { };
    LetExpr (const LetExpr &) { };
};

class ExprNested : public Expr {
public:
    ExprNested(Expr *_expr) :
    expr(_expr) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Expr *expr ;
    ExprNested () : expr(NULL) { };
    ExprNested (const ExprNested &) { };
};

class StmtIfElse: public Stmt {
public:
    StmtIfElse (Expr *_expr, Stmt *_left, Stmt *_right) : expr(_expr), left(_left), right(_right) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Expr *expr ;
    Stmt *left ;
    Stmt *right ;
    StmtIfElse () : expr(NULL), left(NULL), right(NULL) { } ;
    StmtIfElse (const StmtIfElse &) { } ;
};

class StmtIf: public Stmt {
public:
    StmtIf (Expr *_expr, Stmt *_stmt) : expr(_expr), stmt(_stmt) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Expr *expr ;
    Stmt *stmt ;
    StmtIf () : expr(NULL), stmt(NULL) { } ;
    StmtIf (const StmtIf &) { } ;

};

class NestedStmt: public Stmt {
public:
    NestedStmt (Stmts *_stmts) :  stmts(_stmts) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Stmts *stmts ;
    NestedStmt () : stmts(NULL) { } ;
    NestedStmt (const NestedStmt &) { } ;

};

class StmtAsgMatrix: public Stmt {
public:
    StmtAsgMatrix (Varname *_varname, Expr *_expr, Expr *_expro, Expr *_exprt) :
    varname(_varname), expr(_expr), expro(_expro), exprt(_exprt) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Varname *varname ;
    Expr *expr;
    Expr *expro;
    Expr *exprt;
    StmtAsgMatrix () : varname(NULL), expr(NULL), expro(NULL), exprt(NULL) { };
    StmtAsgMatrix (const StmtAsgMatrix &) { } ;
};

class StmtAsg: public Stmt {
public:
    StmtAsg (Varname *_varname, Expr *_expr) : varname(_varname), expr(_expr) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Varname *varname ;
    Expr *expr;
    StmtAsg () : varname(NULL), expr(NULL) { };
    StmtAsg (const StmtAsg &) { } ;
};

class StmtFor: public Stmt {
public:
    StmtFor (Varname *_v, Expr *_e, Expr *_s, Stmt *_ss) : v(_v), e(_e), s(_s), ss(_ss) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Varname *v ;
    Expr *e ;
    Expr *s ;
    Stmt *ss ;
    StmtFor () : v(NULL), e(NULL), s(NULL), ss(NULL) { } ;
    StmtFor (const StmtFor &) { } ;

};

class StmtWhile: public Stmt {
public:
    StmtWhile (Expr *_e, Stmt *_s) : e(_e), s(_s) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Expr *e ;
    Stmt *s ;
    StmtWhile () : e(NULL), s(NULL) { } ;
    StmtWhile (const StmtWhile &) { } ;

};

class Stmtsemic : public Stmt {
public:
    Stmtsemic ( ) { } ;
    std::string unparse ();
    std::string cppCode ();
private:
    Stmtsemic(const Stmtsemic &) {} ;
} ;

class ExprMatrixRef : public Expr {
public:
    ExprMatrixRef( Varname *_v, Expr *_left, Expr *_right ) :
    v(_v), left(_left), right(_right) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Varname *v ;
    Expr *left ;
    Expr *right ;
    ExprMatrixRef () : v(NULL), left(NULL), right(NULL) { };
    ExprMatrixRef (const ExprMatrixRef &) { };
};

class ExprNestedCall : public Expr {
public:
    ExprNestedCall( Varname *_v, Expr *_expr) :
    v(_v), expr(_expr) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Varname *v ;
    Expr *expr ;
    ExprNestedCall () : v(NULL), expr(NULL) { };
    ExprNestedCall (const ExprNestedCall &) { };
};

class IfExpr : public Expr {
public:
    IfExpr( Expr *_e, Expr *_left, Expr *_right ) :
    e(_e), left(_left), right(_right) { };
    std::string unparse ( ) ;
    std::string cppCode ();
private:
    Expr *e ;
    Expr *left ;
    Expr *right ;
    IfExpr () : e(NULL), left(NULL), right(NULL) { };
    IfExpr (const IfExpr &) { };
};

class DeclMatrixOne: public Decl { //# 20 from grammar.txt
public:
/*! The Matrix declaration takes in 6 parameters which define an entry in the matrix
	\param *_v the name of the matrix
	\param Expr *_e an index bound
	\param Expr *_ee another index bound
	\param Varname *_vv index
	\param Varname *_vvv another index
*/
    DeclMatrixOne( Varname *_v, Expr *_e, Expr *_ee, Varname *_vv, Varname *_vvv, Expr *_eee):
    v(_v), e(_e), ee(_ee), vv(_vv), vvv(_vvv), eee(_eee) { };
    std::string unparse ( );
    std::string cppCode ();
private:
    Varname *v;
    Expr *e;
    Expr *ee;
    Varname *vv;
    Varname *vvv;
    Expr *eee;
    DeclMatrixOne(): v(NULL), e(NULL), ee(NULL), vv(NULL), vvv(NULL), eee(NULL) { };
    DeclMatrixOne( const DeclMatrixOne &) { };
};
class DeclMatrixTwo: public Decl { //# 21 from grammar.txt
public:
    DeclMatrixTwo( Varname *_v, Expr *_e) : v(_v), e(_e) { } ;
    std::string unparse( );
    std::string cppCode ();
private:
    Varname *v;
    Expr *e;
    DeclMatrixTwo(): v(NULL), e(NULL) { };
    DeclMatrixTwo(const DeclMatrixTwo &) { };
};


#endif

