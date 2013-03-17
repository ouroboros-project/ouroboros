
#ifndef OPWIG_MDPARSER_PREINCLUDES_H_
#define OPWIG_MDPARSER_PREINCLUDES_H_

#include <opwig/mdnamespace.h>

#include <memory>
#include <list>
#include <functional>

namespace opwig {

typedef std::function<bool (MDNamespace::Ptr)>  NamespaceAction;
typedef std::list<NamespaceAction>              NamespaceActionList;

} // namespace opwig

#endif // OPWIG_MDPARSER_PREINCLUDES_H_

