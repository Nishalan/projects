/* A small meaningless commenttt */
#ifndef SCANNER_H
#define SCANNER_H

#include <regex.h>
#include <string>

//To avoid any plagiarism issues, consuming white space and comments is (mostly) from lab 2 wordcount.cpp	

class Token ;

/* This enumerated type is used to keep track of what kind of
   construct was matched. 
 */

enum tokenEnumType { 

	intKwd, floatKwd, boolKwd,
	trueKwd, falseKwd, stringKwd, matrixKwd,
	letKwd, inKwd, endKwd, ifKwd, thenKwd, elseKwd,
	forKwd, whileKwd, printKwd,

	// Constants
	intConst, floatConst, stringConst, 

	// Names
	variableName ,

	// Punctuation
	leftParen, rightParen, 
	leftCurly, rightCurly, 
	leftSquare, rightSquare,

	comma, semiColon, colon,

	//Operators
	assign, 
	plusSign, star, dash, forwardSlash,
	lessThan, lessThanEqual, greaterThan, greaterThanEqual,
	equalsEquals, notEquals,
	andOp, orOp, notOp,

	// Special terminal types
	endOfFile ,
	lexicalError
} ;
typedef enum tokenEnumType tokenType ;

// Below you need to write your class definitions for Token and Scanner.

class Token{
public:
    std::string lexeme;

    tokenType terminal;

    Token* next;


    Token();
    Token(std::string lex,tokenType type );
    Token(std::string lex, tokenType type, Token* next);
    ~Token() ;


private:

};

class Scanner{
public:
    regex_t** typeDatabase;//holds an array of regex_t pointers, used for matching later
    Token *scan (const char *) ;

   

    Scanner();
    ~Scanner();


private:
    void buildDataBase();
    int consumeWhiteSpaceAndComments(const char *text);
};



#endif /* SCANNER_H */
