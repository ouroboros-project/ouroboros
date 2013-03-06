// Generated by Bisonc++ V4.01.00 on Tue, 05 Mar 2013 14:20:09 -0300

#ifndef MDParser_h_included
#define MDParser_h_included

// $insert baseclass
#include <opwig/mdparser/mdparserbase.h>

#include <opwig/mdparser/mdscanner.h>

// $insert namespace-open
namespace opwig {

#undef MDParser
class MDParser: public MDParserBase {

  public:
    int parse();

  private:
    MDScanner d_scanner;

    void error(char const *msg);    // called on (syntax) errors
    int lex();                      // returns the next token from the
                                    // lexical scanner. 
    void print();                   // use, e.g., d_token, d_loc

    // support functions for parse():
    void executeAction(int ruleNr);
    void errorRecovery();
    int lookup(bool recovery);
    void nextToken();
    void print__();
};

// $insert namespace-close
}

#endif
