#include <cxxtest/TestSuite.h>

#include "readInput.h"
#include "scanner.h"
#include <stdio.h>
#include <iostream>
using namespace std ;

class ScannerTestSuite : public CxxTest::TestSuite 
{
public:
    /* A Scanner object is created in the test_setup_code method.
       This is a test, beginning with "test_", so that it is executed
       by the testing framework.  The scanner is used in some tests of
       the method "scan".
     */
    Scanner *s ;
    void test_setup_code ( ) {
        s = new Scanner() ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.


    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_TokenMaker_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */



    // Tests for "scan"
    // --------------------------------------------------

    /* Below are some helper functions and sample tests for testing the 
       "scan" method on Scanner.
    */

    // Test that a list of tokens has no lexicalError tokens.
    bool noLexicalErrors (Token *tks) {
        Token *currentToken = tks ;
        while (currentToken != NULL) {
            if (currentToken->terminal == lexicalError) {
                printf("priblem: %s\n",currentToken->lexeme.c_str());
				return false ;
            }
            else {
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }

    /* A quick, but inaccurate, test for scanning files.  It only
       checks that no lexical errors occurred, not that the right
       tokens were returned. 
    */
    void scanFileNoLexicalErrors ( const char* filename ) {
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (noLexicalErrors(tks));
    }


    /* This function checks that the terminal fields in the list of
       tokens matches a list of terminals.
    */
    bool sameTerminals (Token *tks, int numTerms, tokenType *ts) {
        Token *currentToken = tks ;
        int termIndex = 0 ;
        while (currentToken != NULL && termIndex < numTerms ) {
            if (currentToken->terminal != ts[termIndex]) {
            printf("%i: %i should be %i\n",termIndex,currentToken->terminal , ts[termIndex]);
			fflush(stdout);
                return false ;
            }
            else {
                ++ termIndex ;
                currentToken = currentToken->next ;
            }
        }
        return true ;
    }


    // Tests for components and functions used by "scan"
    // --------------------------------------------------

    /* You may need to write several tests to adequately test the
       functions that are called from "scan".  Once you are confident
       that the components used by "scan" work properly, then you can
       run tests on the "scan" method itself.
    */

    // Here you should place tests for these other functions.



    void test_terminal_intKwd ( ) {
        Token *t = s->scan (" Int ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT ( t->terminal==intKwd) ;
    }
    
    void test_terminal_floatKwd ( ) {
        Token *t = s->scan (" Float ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT (t->terminal==floatKwd) ;
    }

   void test_terminal_boolKwd ( ) {          //new test
        Token *t = s->scan (" Bool ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT (t->terminal==boolKwd) ;
    }

    void test_terminal_trueKwd ( ) {          //new test
        Token *t = s->scan (" True ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT (t->terminal==trueKwd) ;
    }

    void test_terminal_falseKwd ( ) {          //new test
        Token *t = s->scan (" False ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT (t->terminal==falseKwd) ;
    }


    void test_terminal_stringKwd ( ) {
        Token *t = s->scan (" Str ") ;
        TS_ASSERT (t != NULL) ;
        TS_ASSERT (t->terminal==stringKwd) ;
    }

    void test_terminal_matrixKwd ( ) {
        Token *t = s->scan (" Matrix ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { matrixKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_letKwd ( ) {
        Token *t = s->scan (" let ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { letKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_inKwd ( ) {
        Token *t = s->scan (" in ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { inKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_endKwd ( ) {
        Token *t = s->scan (" end ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { endKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_ifKwd ( ) {
        Token *t = s->scan (" if ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { ifKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_thenKwd ( ) {
        Token *t = s->scan (" then ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { thenKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_elseKwd ( ) {
        Token *t = s->scan (" else ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { elseKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_forKwd ( ) {
        Token *t = s->scan (" for ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { forKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_whileKwd ( ) {
        Token *t = s->scan (" while ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { whileKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_printKwd ( ) {
        Token *t = s->scan (" print ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { printKwd, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_intConst ( ) {
        Token *t = s->scan (" 12 ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { intConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_floatConst ( ) {
        Token *t = s->scan (" 12.34 ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { floatConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
        void test_terminal_stringConst ( ) {
        Token *t = s->scan (" \"abc\" ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { stringConst, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_variableName ( ) {
        Token *t = s->scan (" abc ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { variableName, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_leftParen ( ) {
        Token *t = s->scan (" ( ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { leftParen, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_rightParen ( ) {
        Token *t = s->scan (" ) ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { rightParen, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_leftCurly ( ) {
        Token *t = s->scan (" { ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { leftCurly, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_rightCurly ( ) {
        Token *t = s->scan (" } ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { rightCurly, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_leftSquare ( ) {
        Token *t = s->scan (" [ ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { leftSquare, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_rightSquare ( ) {
        Token *t = s->scan (" ] ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { rightSquare, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_comma ( ) {
        Token *t = s->scan (" , ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { comma, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_semiColon ( ) {
        Token *t = s->scan (" ; ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { semiColon, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_colon ( ) {
        Token *t = s->scan (" : ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { colon, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_assign ( ) {
        Token *t = s->scan (" = ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { assign, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_plusSign ( ) {
        Token *t = s->scan (" + ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { plusSign, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_star ( ) {
        Token *t = s->scan (" * ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { star, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_dash ( ) {
        Token *t = s->scan (" - ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { dash, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_forwardSlash ( ) {
        Token *t = s->scan (" / ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { forwardSlash, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_lessThan ( ) {
        Token *t = s->scan (" < ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { lessThan, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_lessThanEqual ( ) {
        Token *t = s->scan (" <= ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { lessThanEqual, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_greaterThan ( ) {
        Token *t = s->scan (" > ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { greaterThan, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_greaterThanEqual ( ) {
        Token *t = s->scan (" >= ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { greaterThanEqual, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_equalsEquals ( ) {
        Token *t = s->scan (" == ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { equalsEquals, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_notEquals ( ) {
        Token *t = s->scan (" != ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { notEquals, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }
    
    void test_terminal_andOp ( ) {
        Token *t = s->scan (" && ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { andOp, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_orOp ( ) {
        Token *t = s->scan (" || ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { orOp, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }

    void test_terminal_notOp ( ) {
        Token *t = s->scan (" ! ") ;
        TS_ASSERT (t != NULL) ;
        tokenType ts[] = { notOp, endOfFile } ;
        TS_ASSERT ( sameTerminals ( t, 2, ts ) ) ;
    }


	

    /* Below is one of the tests for these components in the project
       solution created by your instructor..  It uses a helper
       function function called "tokenMaker_tester", which you have
       not been given.  You are expected to design your own components
       for "scan" and your own mechanisms for easily testing them.

        void xtest_terminal_leftCurly ( ) {
            tokenMaker_tester ("{ ", "^\\{", leftCurly, "{" ) ;
        }

        Note that this test is here named "xtest_Token..." The leading
        "x" is so that cxxTest doesn't scan the line above and think
        it is an actual test that isn't commented out.  cxxTest is
        very simple and doesn't even process block comments.
    */


    /* You must have at least one separate test case for each terminal
       symbol.  Thus, you need a test that will pass or fail based
       solely on the regular expression (and its corresponding code)
       for each terminal symbol.

       This requires a test case for each element of the enumerated
       type tokenType.  This may look something like the sample test
       shown in the comment above.
    */


    // Tests for "scan"
    // --------------------------------------------------


    /* Below are the provided test files that your code should also
       pass.

       You may initially want to rename these tests to "xtest_..." so
       that the CxxTest framework does not see it as a test and won't
       run it as one.  This way you can focus on the tests that you'll
       write above for the individual terminal types first. Then focus
       on these tests.

    */

    // The "endOfFile" token is always the last one in the list of tokens.
    void test_scan_empty ( ) {
        Token *tks = s->scan ("  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    void test_scan_empty_comment ( ) {
        Token *tks = s->scan (" /* a comment */ ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }

    // When a lexical error occurs, the scanner creates a token with a 
    // single-character lexeme and lexicalError as the terminal.
   void test_scan_lexicalErrors ( ) {
        Token *tks = s->scan ("$&1  ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "$");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, lexicalError) ;
        TS_ASSERT_EQUALS (tks->lexeme, "&");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, intConst) ;
        TS_ASSERT_EQUALS (tks->lexeme, "1");
        tks = tks->next ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT_EQUALS (tks->terminal, endOfFile) ;
        TS_ASSERT (tks->next == NULL) ;
    }


    // A test for scanning numbers and a variable.
    void test_scan_nums_vars ( ) {
        Token *tks = s->scan (" 123 x 12.34 ") ;
        TS_ASSERT (tks != NULL) ;
        TS_ASSERT (tks->terminal==intConst ) ;
        TS_ASSERT (tks->lexeme=="123" ) ;
        TS_ASSERT (tks->next->lexeme=="x" ) ;
        TS_ASSERT (tks->next->terminal==variableName) ;
        TS_ASSERT (tks->next->next->lexeme=="12.34" ) ;
        TS_ASSERT (tks->next->next->terminal==floatConst) ;
    }



    /* This test checks that the scanner returns a list of tokens with
       the correct terminal fields.  It doesn't check that the lexeme
       are correct.
     */
    
    void test_scan_bad_syntax_good_tokens ( ) {
       const char *filename = "../samples/bad_syntax_good_tokens.dsl" ;
        char *text =  readInputFromFile ( filename )  ;
        TS_ASSERT ( text ) ;
        Token *tks = s->scan ( text ) ;
        TS_ASSERT (tks != NULL) ;
        tokenType ts[] = { 
            intConst, intConst, intConst, intConst, 

            stringConst, stringConst, stringConst,

            floatConst, floatConst, floatConst,

                   matrixKwd,



            semiColon, comma, colon,
            leftCurly, leftParen, rightCurly, rightParen,

            plusSign, star, dash, forwardSlash, 

            equalsEquals, lessThanEqual, 
            greaterThanEqual, notEquals, 
            assign,

            variableName, variableName, variableName, variableName, 
            variableName, variableName, variableName,


            intKwd, floatKwd,  stringKwd,  whileKwd,

            endOfFile
       } ;
        int count = 39; 
        TS_ASSERT ( sameTerminals ( tks, count, ts ) ) ;
    }

    void test_scan_sample_forestLoss ( ) {
        scanFileNoLexicalErrors ("../samples/forest_loss_v2.dsl") ;
    }

      

} ;
