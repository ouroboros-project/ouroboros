// Generated by Bisonc++ V4.01.00 on Tue, 05 Mar 2013 14:20:09 -0300

#ifndef MDParser_h_included
#define MDParser_h_included

// $insert baseclass
#include <opwig/parser/mdparserbase.h>

#include <opwig/parser/mdscanner.h>
#include <opwig/md/namespace.h>

#include <iostream>
#include <string>

// $insert namespace-open
namespace opwig {

#undef MDParser
class MDParser: public MDParserBase {

  public:
    MDParser () :
      d_scanner(std::cin, std::cerr),
      global_namespace_(md::Namespace::Create()) {}
    explicit MDParser (std::istream& in) :
      d_scanner(in, std::cerr),
      global_namespace_(md::Namespace::Create()) {}

    int parse();

    md::Ptr<const md::Namespace> global_namespace () const;

  private:
    MDScanner               d_scanner;
    md::Ptr<md::Namespace>  global_namespace_;

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
    void exceptionHandler__(std::exception const &exc);
};

inline md::Ptr<const md::Namespace> MDParser::global_namespace () const {
  return global_namespace_;
}

// $insert namespace-close
}

#endif
