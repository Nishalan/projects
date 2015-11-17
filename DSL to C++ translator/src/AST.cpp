
//actual definitions for member functions in AST.h classes

#include "AST.h"
#include "scanner.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;
//Root

static int i = 1;//used for padding to keep format of cpp readable

std::string Root::unparse(){
	return varname->unparse() + "(){" + stmts->unparse() + "}";
}

std::string Root::cppCode(){
	return "#include <iostream>\n#include <math.h>\n#include \"Matrix.h\"\nusing namespace std;\n \n int " + varname->cppCode() +"(){\n" + stmts->cppCode() + "}\n";
}
//Varname
std::string Varname::unparse(){
	return lexeme;
}

std::string Varname::cppCode(){
	return lexeme;
} 

//StmtsSeq
std::string StmtsSeq::unparse(){
	return stmt->unparse() + stmts->unparse();
}

std::string StmtsSeq::cppCode(){
	return padding() + stmt->cppCode() +"\n" + stmts->cppCode();
}

//emptystatement
std::string EmptyStmts::unparse(){
	 
	return "";
}

std::string EmptyStmts::cppCode(){
	 
	return "";
}
//KwdDecl
std::string KwdDecl::unparse(){
	std::string keyword;
	if(kwd==intKwd){
		keyword="Int";
	}
	if(kwd==floatKwd){
		keyword="Float";
	}
	if(kwd==stringKwd){
		keyword="Str";
	}
	if(kwd==boolKwd){
		keyword="Bool";
	}

	return keyword + " " + varname->unparse() + ";" ;
}

std::string KwdDecl::cppCode(){
	std::string keyword;
	if(kwd==intKwd){
		keyword="int";
	}
	if(kwd==floatKwd){
		keyword="float";
	}
	if(kwd==stringKwd){
		keyword="char* ";
	}
	if(kwd==boolKwd){
		keyword="bool";
	}

	return keyword + " " + varname->cppCode() + ";" ;
}

//print								
std::string StmtPrint::unparse(){
	return "print(" + expr->unparse() + ");";
}

std::string StmtPrint::cppCode(){
	return "cout<<" + expr->cppCode() + ";";
}

//exprconst
std::string ExprConstStr::unparse(){
	return s;
}
std::string ExprConstInt::unparse(){
	return s;
}
std::string ExprConstFloat::unparse(){
	return s;
}

std::string ExprConstStr::cppCode(){
	return s;
}
std::string ExprConstInt::cppCode(){
	return s;
}
std::string ExprConstFloat::cppCode(){
	return s;
}

//exprvar
std::string ExprVar::unparse( ){
	return v->unparse();
}	

std::string ExprVar::cppCode( ){
	return v->cppCode();
}

//exprbool
std::string ExprBool::cppCode ( ){
	if(s=="True"){
	   return "true";
	}
	else{
		return "false";
	}
}

std::string ExprBool::unparse ( ){
	return s;
}
//binop
std::string ExprBinOp::unparse ( ){
	return left->unparse() + op + right->unparse();
}
std::string ExprBinOp::cppCode ( ){
	return left->cppCode() + op + right->cppCode();
}

//not op
std::string NotExpr::unparse ( ) {
	return "!"+ expr->unparse();
}
std::string NotExpr::cppCode ( ) {
	return "!"+ expr->cppCode();
}

//let in end
std::string LetExpr::unparse ( ) {

	return " let " + s->unparse() + " in " + expr->unparse() + " end ";
}
std::string LetExpr::cppCode ( ) {//
	i=i+1;
	std::string o = "({ \n" + s->cppCode() + padding(i) + "(" + expr->cppCode() + " ) ;\n" + padding(i) + "})";
	i=i-1;
	return o;
}

std::string ExprNested::unparse ( ) {	
	return "("+ expr->unparse() + ")";
}

std::string ExprNested::cppCode ( ) {	
	return "("+ expr->cppCode() + ")";
}

std::string StmtIfElse::unparse(){
	
	return "if(" + expr->unparse() + ")" + left->unparse() + "else" + right->unparse();
}
std::string StmtIfElse::cppCode(){
	i=i+1;
	std::string o = "if(" + expr->cppCode() + ")\n" + left->cppCode() + "\nelse\n" + right->cppCode();
	i=i-1;
	return o;
}

std::string StmtIf::unparse(){
	
	return "if(" + expr->unparse() + ")" + stmt->unparse();
}

std::string StmtIf::cppCode(){
	i=i+1;
	std::string o = "if(" + expr->cppCode() + ")\n" + stmt->cppCode() + "\n";
	i=i-1;
	return o;
}
std::string NestedStmt::unparse ( ){
	
	return "{"+ stmts->unparse() + "}";
}
std::string NestedStmt::cppCode ( ){
	
	return "{\n"+ stmts->cppCode() + padding(i-1) + "}";
}

std::string StmtAsgMatrix::unparse(){
	
	return varname->unparse() + "[" + expr->unparse() + "," + expro->unparse() + "]=" + exprt->unparse() + ";";
}
std::string StmtAsgMatrix::cppCode(){//
	
	return "*( " + varname->cppCode() + ".access(" + expr->cppCode() + "," + expro->cppCode() + " )) = " + exprt->cppCode() + " ;";
}
std::string StmtAsg::unparse(){

	return varname->unparse() + "=" + expr->unparse() + ";";
}

std::string StmtAsg::cppCode(){

	return varname->cppCode() + "=" + expr->cppCode() + ";";
}

std::string StmtFor::unparse(){
	
	return "for(" + v->unparse() + "=" + e->unparse() + ":" + s->unparse() + ")" + ss->unparse();
	
	
}
std::string StmtFor::cppCode(){
	i=i+1;
	std::string o =  "for(" + v->cppCode() + "=" + e->cppCode() + ";" + v->cppCode() + "<=" + s->cppCode() + ";" + v->cppCode() + " ++)"  + ss->cppCode() ;
	
	i=i-1;
	return o;
}
std::string StmtWhile::unparse(){
	
	return "while(" + e->unparse() +  ")" + s->unparse();
}
std::string StmtWhile::cppCode(){
	i=i+1;
	std::string o =  "while(" + e->cppCode() +  ")\n" + s->cppCode();
	i=i-1;
	return o;
}

std::string Stmtsemic::unparse(){
	
	return ";";
}
std::string Stmtsemic::cppCode(){
	
	return ";";
}

std::string ExprMatrixRef::unparse(){
	
	return v->unparse() + "[" + left->unparse() + "," + right->unparse()+"]";
}

std::string ExprMatrixRef::cppCode(){//
	
	return "*( " + v->cppCode() + ".access(" + left->cppCode() + "," + right->cppCode() + " ))";
}
std::string ExprNestedCall::unparse(){
	
	return v->unparse() + "(" + expr->unparse() + ")";
}
std::string ExprNestedCall::cppCode(){

	if(v->cppCode()=="Matrix"||v->cppCode()=="access"||v->cppCode()=="readMatrix"){
	return "Matrix::" + v->cppCode() + "(" + expr->cppCode() + ")";
	}
	else if(v->cppCode()=="numRows"||v->cppCode()=="numCols"){
	return expr->cppCode() + "." + v->cppCode() + "()";
	}
	else{
	return v->cppCode() + "(" + expr->cppCode() + ")";
	}
}
std::string IfExpr::unparse(){

	return "if " + e->unparse() + " then " + left->unparse()+" else "+right->unparse();
}

std::string IfExpr::cppCode(){//

	return "(" + e->cppCode() + ") ? (" + left->cppCode() + ") : (" + right->cppCode()  + ")";
}
std::string DeclMatrixOne::unparse(){
	
	return " Matrix "+v->unparse() + " [ " + e->unparse() + " , " + ee->unparse() + " ] " + vv->unparse()+ "," + vvv->unparse() + "=" + eee->unparse()+ ";";
}

std::string DeclMatrixOne::cppCode(){//
	
	return "Matrix " + v->cppCode() + "( " + e->cppCode() + " , " + ee->cppCode() + "); \n	for( int " + vv->cppCode() + " = 0;" + vv->cppCode()+ " < " + e->cppCode() + " ; " + vv->cppCode() + " ++ ) { \n		for(int " + vvv->cppCode() + " = 0; " + vvv->cppCode() + " < " + ee->cppCode() + " ; " + vvv->cppCode() + " ++ ){\n			*(" + v->cppCode() + ".access(" + vv->cppCode() + " , " + vvv->cppCode() + " )) = " + eee->cppCode() + ";\n		}\n	}";
}

std::string DeclMatrixTwo::unparse(){
	
	return "Matrix "+v->unparse() + " = " + e->unparse() + ";";
}
std::string DeclMatrixTwo::cppCode(){//
	
	return "Matrix "+v->cppCode() + " ( " + e->cppCode() + ");";
}


std::string padding(){
int c = i;
std::string indent = "";
while(c>0){
indent = indent + "	";
c=c-1;
}
return indent;
}

std::string padding(int i){
int c = i;
std::string indent = "";
while(c>0){
indent = indent + "	";
c=c-1;
}
return indent;
}





