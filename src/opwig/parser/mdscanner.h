// Generated by Flexc++ V1.02.00 on Wed, 06 Mar 2013 08:59:35 -0300

#ifndef MDScanner_H_INCLUDED_
#define MDScanner_H_INCLUDED_

// $insert baseclass_h
#include <opwig/parser/mdscannerbase.h>
#include <opwig/parser/mdparserbase.h>

#include <opwig/md/ptr.h>
#include <opwig/md/scope.h>

#include <stack>

// $insert namespace-open
namespace opwig
{

// $insert classHead
class MDScanner: public MDScannerBase {
  public:
    explicit MDScanner(std::istream &in = std::cin,
                       std::ostream &out = std::cout);

    MDScanner(std::string const &infile, std::string const &outfile);
    
    void PushScope(const md::Ptr<md::Scope>& the_current_scope);
    void PopScope();

    // $insert lexFunctionDecl
    int lex();
    
    void setShortDebugOutput(bool short_output) { short_debug_ = short_output; }

  private:
    //md::Ptr<md::Scope> current_scope_;
    std::stack<md::Ptr<md::Scope>> scope_stack_;

    int lex__();
    int executeAction__(size_t ruleNr);

    void print();
    void preCode();     // re-implement this function for code that must 
                        // be exec'ed before the patternmatching starts

    int peek(); //calls lex() to get the next token, then puts it back in the input queue to be re-read
                //and returns the token.
                // effectively 'peeking' at the next token, before it is actually read.
    bool in_peek_ = false;
    int last_token_ = -1;
    unsigned int parenthesis_depth_ = 0;
    bool short_debug_ = false;
    
    bool currentIsTypeName();
};

// $insert scannerConstructors
inline MDScanner::MDScanner(std::istream &in, std::ostream &out)
:
    MDScannerBase(in, out)
{}

inline MDScanner::MDScanner(std::string const &infile, std::string const &outfile)
:
    MDScannerBase(infile, outfile)
{}

// $insert inlineLexFunction
inline int MDScanner::lex()
{
    if (!in_peek_) {
        last_token_ = d_token__;
        if (short_debug_)
            std::cout << "    -------------------------------------" << std::endl;
    }
    int token = lex__();
    if (!in_peek_) {
        if (short_debug_) {
            std::cout << "    lex: token=" << d_token__ << " [" << matched() << "]" << std::endl;
            std::cout << "    -------------------------------------" << std::endl;
        }
        if (token == '(')   parenthesis_depth_++;
        else if (token == ')')  parenthesis_depth_--;
    }
    return token;
}

inline void MDScanner::preCode() 
{
    // optionally replace by your own code
}

inline void MDScanner::print() 
{
    print__();
}

inline void MDScanner::PushScope(const md::Ptr<md::Scope>& the_current_scope) {
    scope_stack_.push(the_current_scope);
}

inline void MDScanner::PopScope() {
    scope_stack_.pop();
}

inline int MDScanner::peek() {
    if (in_peek_) return -1;
    in_peek_ = true;

    std::string current_matched = matched();
    int current_token = d_token__;
    if (short_debug_)
        std::cout << "    lex Peek (current): token=" << d_token__ << " [" << matched() << "]" << std::endl;

    int token = lex();
    if (short_debug_)
        std::cout << "    lex Peek (next): token=" << d_token__ << " [" << matched() << "]" << std::endl;

    accept(0);
    setMatched(current_matched);
    d_token__ = current_token;

    in_peek_ = false;
    return token;
}

inline bool MDScanner::currentIsTypeName() {
    int nextToken = peek();
    if (last_token_ == MDParserBase::TYPE_NAME) return false;
    bool ok = (parenthesis_depth_ > 0 && (last_token_==',' || last_token_=='(' || last_token_==MDParserBase::SCOPE_OPERATOR) 
                                      && (nextToken==')' || nextToken==',' || nextToken=='*' || nextToken=='&' || nextToken==MDParserBase::IDENTIFIER) );
    ok = ok || (parenthesis_depth_ == 0 && (nextToken==MDParserBase::IDENTIFIER || nextToken=='*' || nextToken=='&'));
    return ok;
}

// $insert namespace-close
}

#endif // MDScanner_H_INCLUDED_

