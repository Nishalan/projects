#include "scanner.h"
#include "regex.h"
#include <string>
#include <stdlib.h>
#include <stdio.h> 
#include <iostream> 



//regex_t** Scanner::typeDatabase;

Token::Token(){
}

Scanner::Scanner(){
Scanner::buildDataBase();
}

Scanner::~Scanner(void){
}

Token::~Token(void){
}

Token* Scanner::scan (const char *text){

int numMatchedChars;
int maxchars;
int topi;
Token* current = new Token();
Token* root = current;
text+=consumeWhiteSpaceAndComments(text);

while(*text!='\0'){

		maxchars=0;
		topi=-1;
		for(int i=intKwd; i<endOfFile; i++){
			if(maxchars<(numMatchedChars=matchRegex(typeDatabase[i], text))){	
				maxchars=numMatchedChars;
				topi=i;
			}
		}
			if(maxchars>0){
				//std::cout<<"matched lexeme"<< std::string(text,maxchars)<<"\n";
				(*current).lexeme=std::string(text,maxchars);
				(*current).terminal= static_cast<tokenType>(topi);
				(*current).next=new Token();
				current=(*current).next;
				text=text+maxchars;
				text+=consumeWhiteSpaceAndComments(text);


			}
			else{//maxchars==0, then its a lexical error
				(*current).lexeme=std::string(text,1);
				(*current).terminal= lexicalError;
				(*current).next=new Token();
				current=(*current).next;
				text=text+1;
				text+=consumeWhiteSpaceAndComments(text);
			}

}



(*current).terminal=endOfFile;
//(*current).lexeme="endOfFile";
current->next=NULL;
return root;
}

void Scanner::buildDataBase(){
// malloc typeDatabase, this will be an array of pointers to regexes, there are 46 types of tokens, but we wont need all of them in regexes. this array will be easily extensible

typeDatabase=(regex_t**)malloc(43*(sizeof(regex_t*)));

typeDatabase[intKwd]=makeRegex("^Int");//intkwd
typeDatabase[floatKwd]=makeRegex("^Float");//floatkwd
typeDatabase[boolKwd]=makeRegex("^Bool");//boolkwd
typeDatabase[trueKwd]=makeRegex("^True");//truekwd
typeDatabase[falseKwd]=makeRegex("^False");//falsekwd
typeDatabase[stringKwd]=makeRegex("^Str");//stringkwd
typeDatabase[matrixKwd]=makeRegex("^Matrix");//matrixkwd
typeDatabase[letKwd]=makeRegex("^let");//letkwd
typeDatabase[inKwd]=makeRegex("^in");//inkwd
typeDatabase[endKwd]=makeRegex("^end");//endkwd
typeDatabase[ifKwd]=makeRegex("^if");//ifkwd
typeDatabase[thenKwd]=makeRegex("^then");//thenkwd
typeDatabase[elseKwd]=makeRegex("^else");//elsekwd
typeDatabase[forKwd]=makeRegex("^for");//forkwd
typeDatabase[whileKwd]=makeRegex("^while");//whilekwd
typeDatabase[printKwd]=makeRegex("^print");//printkwd
typeDatabase[intConst]=makeRegex("^[0-9]+");//intconst
typeDatabase[floatConst]=makeRegex("(^[0-9]+\\.[0-9]+)");
//|(^[0-9]*\\.[0-9]+)");//floatconst
typeDatabase[stringConst]=makeRegex("^\"[^\"]*\"");//stringconst
typeDatabase[variableName]=makeRegex("^[a-zA-Z_]+[0-9_]*");//variableName
typeDatabase[leftParen]=makeRegex("^\\(");//leftParen
typeDatabase[rightParen]=makeRegex("^\\)");//rightParen
typeDatabase[leftCurly]=makeRegex("^\\{");//leftCurly
typeDatabase[rightCurly]=makeRegex("^\\}");//rightCurly
typeDatabase[leftSquare]=makeRegex("^\\[");//leftsquare
typeDatabase[rightSquare]=makeRegex("^\\]");//rightsquare
typeDatabase[comma]=makeRegex("^,");//comma
typeDatabase[semiColon]=makeRegex("^;");//semicolon
typeDatabase[colon]=makeRegex("^:");//colon
typeDatabase[assign]=makeRegex("^=");//assign
typeDatabase[plusSign]=makeRegex("^\\+");//plusSign
typeDatabase[star]=makeRegex("^\\*");//star
typeDatabase[dash]=makeRegex("^\\-");//dash
typeDatabase[forwardSlash]=makeRegex("^/");//forwardslash
typeDatabase[lessThan]=makeRegex("^<");//lessThan
typeDatabase[lessThanEqual]=makeRegex("^<=");//lessThanEqual
typeDatabase[greaterThan]=makeRegex("^>");//greaterThan
typeDatabase[greaterThanEqual]=makeRegex("^>=");//greaterThanEqual
typeDatabase[equalsEquals]=makeRegex("^==");//equalsEquals
typeDatabase[notEquals]=makeRegex("^!=");//notEquals
typeDatabase[andOp]=makeRegex("^&&");//andOp
typeDatabase[orOp]=makeRegex("^\\|\\|");//orOp
typeDatabase[notOp]=makeRegex("^!");//notOp
}

int Scanner::consumeWhiteSpaceAndComments (const char *text) {
    regex_t* whiteSpace=makeRegex("^[\n\t\r ]+") ;
    regex_t* blockComment=makeRegex("^/\\*([^\\*]|\\*+[^\\*/])*\\*+/");
    regex_t* lineComment=makeRegex("^//[^\n]*\n");
    int numMatchedChars = 0 ;
    int totalNumMatchedChars = 0 ;
    int stillConsumingWhiteSpace ;

    do {
        stillConsumingWhiteSpace = 0 ;  // exit loop if not reset by a match

        // Try to match white space
        numMatchedChars = matchRegex (whiteSpace, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
             stillConsumingWhiteSpace = 1 ;
        }

        // Try to match block comments
        numMatchedChars = matchRegex (blockComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }

        // Try to match single-line comments
        numMatchedChars = matchRegex (lineComment, text) ;
        totalNumMatchedChars += numMatchedChars ;
        if (numMatchedChars > 0) {
            text = text + numMatchedChars ;
            stillConsumingWhiteSpace = 1 ;
        }
    }
    while ( stillConsumingWhiteSpace ) ;    

    return totalNumMatchedChars ;
}

Token::Token(std::string lex, tokenType type){
	lexeme=lex;
	terminal=type;
}
Token::Token(std::string lex, tokenType type,  Token* tok){
	lexeme=lex;
	terminal=type;
	next=tok;
}
