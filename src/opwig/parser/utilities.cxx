#include <opwig/parser/utilities.h>

namespace opwig {
namespace parser {

ScopeAction AddTypeToScope( const TypeAction& type_action ) {
    ScopeAction action = [type_action] (md::Ptr<md::Scope> current_scope) -> bool {
        type_action(current_scope);
        return true;
    };
    return action;
}

ScopeAction JoinDeclarations( const TypeAction& type_action, const std::shared_ptr<parser::DeclaratorList>& init_list ) {
    ScopeAction action = [type_action, init_list] (md::Ptr<md::Scope> current_scope) -> bool {
        for (auto declarator: *init_list) {
            std::string type = type_action(current_scope);
            if (declarator.has_parameters()) {
                md::Ptr<md::Function> func = md::Function::Create(declarator.name(), type, declarator.parameters(), declarator.is_pure());
                if (!current_scope->AddNestedFunction(func)) {
                    throw md::SemanticError("Failed to add Function '"+func->name()+"' to Scope");
                }
            } 
            else {
                md::Ptr<md::Variable> var = md::Variable::Create(declarator.name(), type);
                if (!current_scope->AddGlobalVariable(var)) {
                    throw md::SemanticError("Failed to add Variable '"+var->name()+"' to Scope");
                }
            }
        }
        return true;
    };
    return action;
}

} // namespace parser
} // namespace opwig

