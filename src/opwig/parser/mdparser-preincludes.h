
#ifndef OPWIG_MDPARSER_PREINCLUDES_H_
#define OPWIG_MDPARSER_PREINCLUDES_H_

#include <opwig/md/namespace.h>
#include <opwig/md/ptr.h>

#include <memory>
#include <list>
#include <functional>

namespace opwig {

typedef std::function<bool (md::Ptr<md::Namespace>)>  NamespaceAction;
typedef std::list<NamespaceAction>                NamespaceActionList;

typedef std::list<std::string>                    StringList;

} // namespace opwig

#endif // OPWIG_MDPARSER_PREINCLUDES_H_

