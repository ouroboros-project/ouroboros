#include <opwig/md/variable.h>
#include <opwig/md/class.h>
#include <opwig/md/function.h>
#include <opwig/md/namespace.h>
#include <opwig/md/accessspecifier.h>
#include <opwig/md/semanticerror.h>
#include <opwig/parser/mdparser.h>
#include <opwig/parser/declarator.h>

#include <gtest/gtest.h>

#include <vector>

#include <string>
#include <istream>
#include <sstream>
#include <list>
#include <algorithm>
#include <exception>

using std::vector;
using std::string;
using std::istringstream;
using std::list;

using opwig::md::Ptr;
using opwig::md::Variable;
using opwig::md::ParameterList;
using opwig::md::Function;
using opwig::md::Scope;
using opwig::md::Namespace;
using opwig::md::Class;
using opwig::md::AccessSpecifier;
using opwig::parser::BaseSpecifier;
using opwig::parser::Declarator;
using opwig::MDParser;


#include <opwig/declarator.cc>
#include <opwig/class.cc>
#include <opwig/classmembers.cc>
#include <opwig/function.cc>
#include <opwig/namespace.cc>
#include <opwig/variable.cc>
