
#ifndef OPWIG_MDPARSER_UTILITIES_H_
#define OPWIG_MDPARSER_UTILITIES_H_

#include <opwig/parser/semantictypes.h>

/*
Defines stand-alone helper functions to facilitate and reduce semantic code on the grammar files.
*/

namespace opwig {
namespace parser {

///Gets a type action and returns a ScopeAction that executes it for the given scope.
ScopeAction AddTypeToScope( const TypeAction& type_action );

///Gets a type action and a list of related declarators and returns a ScopeAction that adds them (Variables, Functions, etc)
///to the given scope.
ScopeAction JoinDeclarations( const TypeAction& type_action, const std::shared_ptr<parser::DeclaratorList>& init_list );

} // namespace parser
} // namespace opwig

#endif // OPWIG_MDPARSER_UTILITIES_H_

