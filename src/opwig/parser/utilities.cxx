#include <opwig/parser/utilities.h>

#include <opwig/md/variable.h>

#include <iostream>

namespace opwig {
namespace parser {

using md::SemanticError;
using md::Ptr;
using md::Scope;
using md::Class;
using md::Function;
using md::Variable;
using md::NestedNameSpecifier;

ScopeAction AddTypeToScope( const TypeAction& type_action ) {
    ScopeAction action = [type_action] (Ptr<Scope> current_scope) -> bool {
        type_action(current_scope);
        return true;
    };
    return action;
}

ScopeAction JoinDeclarations( const TypeAction& type_action, const std::shared_ptr<parser::DeclaratorList>& init_list ) {
    ScopeAction action = [type_action, init_list] (Ptr<Scope> current_scope) -> bool {
        for (auto declarator: *init_list) {
            std::string type = type_action(current_scope);
            NestedNameSpecifier nestedName = declarator.nested_name();
            Ptr<Scope> targetScope = nestedName.FindNearestNestingScope(current_scope);
            if (declarator.has_parameters()) {
                Ptr<Function> func = Function::Create(nestedName.name(), type, declarator.parameters(), declarator.is_pure());
                if (!targetScope->AddNestedFunction(func)) {
                    throw SemanticError("Failed to add Function '"+func->name()+"' to Scope", __FILE__, __LINE__);
                }
            } 
            else {
                Ptr<Variable> var = Variable::Create(nestedName.name(), type);
                if (!targetScope->AddGlobalVariable(var)) {
                    throw SemanticError("Failed to add Variable '"+var->name()+"' to Scope", __FILE__, __LINE__);
                }
            }
        }
        return true;
    };
    return action;
}

TypeAction AddClassToScope( Ptr<Class> classObj, const NestedNameSpecifier& nestedName) {
    TypeAction action = [classObj, nestedName] (md::Ptr<md::Scope> current_scope) -> std::string {
        Ptr<Scope> targetScope = nestedName.FindNearestNestingScope(current_scope);
        if (!targetScope)
            throw SemanticError("Invalid NestedNameSpecifier("+nestedName.ToString()+")", __FILE__, __LINE__);
        if (targetScope->AddNestedClass(classObj->name(), classObj))
            return classObj->name();
        throw SemanticError("Non-anonymous class cannot have empty name!", __FILE__, __LINE__);
    };
    return action;
}

} // namespace parser
} // namespace opwig

