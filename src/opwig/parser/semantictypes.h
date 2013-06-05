
#ifndef OPWIG_MDPARSER_SEMANTICTYPES_H_
#define OPWIG_MDPARSER_SEMANTICTYPES_H_

#include <opwig/md/namespace.h>
#include <opwig/md/function.h>
#include <opwig/md/ptr.h>
#include <opwig/md/semanticerror.h>
#include <opwig/md/nestednamespecifier.h>

#include <opwig/parser/basespecifier.h>
#include <opwig/parser/declarator.h>

#include <memory>
#include <list>
#include <vector>
#include <functional>
#include <utility>

namespace opwig {

typedef std::function<bool (md::Ptr<md::Scope>)>        ScopeAction;
typedef std::list<ScopeAction>                          ScopeActionList;

/*TypeAction is just like a NamespaceAction, adding a type/class to a namespace, 
  but instead of returning a bool it returns a string with the type name.*/
typedef std::function<std::string (md::Ptr<md::Scope>)> TypeAction;
typedef std::list<TypeAction>                           TypeActionList;

typedef std::vector<std::string>                          StringList;

typedef std::list<parser::BaseSpecifier>                BaseSpecifierList;

namespace parser {

using DeclaratorList = std::list<Declarator>;

typedef std::pair<md::Ptr<md::Class>, md::NestedNameSpecifier>  ClassHeadPair;
typedef std::pair<std::string, md::NestedNameSpecifier>          EnumHeadPair;

} // namespace parser

} // namespace opwig

#endif // OPWIG_MDPARSER_SEMANTICTYPES_H_

