
#ifndef OPWIG_MDPARSER_PREINCLUDES_H_
#define OPWIG_MDPARSER_PREINCLUDES_H_

#include <opwig/md/namespace.h>
#include <opwig/md/ptr.h>

#include <opwig/parser/basespecifier.h>

#include <memory>
#include <list>
#include <functional>

namespace opwig {

typedef std::function<bool (md::Ptr<md::Namespace>)>  NamespaceAction;
typedef std::list<NamespaceAction>                NamespaceActionList;

/*TypeAction is just like a NamespaceAction, adding a type/class to a namespace, 
  but instead of returning a bool it returns a string with the type name.*/
typedef std::function<std::string (md::Ptr<md::Namespace>)> TypeAction;

typedef std::list<std::string>                    StringList;

typedef std::list<parser::BaseSpecifier>          BaseSpecifierList;

} // namespace opwig

#endif // OPWIG_MDPARSER_PREINCLUDES_H_

