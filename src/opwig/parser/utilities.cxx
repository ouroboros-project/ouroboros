#include <opwig/parser/utilities.h>
#include <opwig/md/enum.h>
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
using md::Enum;
using md::NestedNameSpecifier;

ScopeAction AddTypeToScope( const TypeAction& type_action ) {
    ScopeAction action = [type_action] (Ptr<Scope> current_scope) -> bool {
        type_action(current_scope);
        return true;
    };
    return action;
}

ScopeAction JoinDeclarations (const TypeAction& type_action,
                              const std::shared_ptr<parser::DeclaratorList>& init_list ) {
    ScopeAction action = [type_action, init_list] (Ptr<Scope> current_scope) -> bool {
        for (auto declarator: *init_list) {
            DeclSpecifier spec = type_action(current_scope);
            std::string type = spec.type().ToString();
            NestedNameSpecifier nestedname = declarator.nested_name();
            Ptr<Scope> targetScope = nestedname.FindNearestNestingScope(current_scope);
            if (declarator.has_parameters()) {
                std::string sig = Function::GetSignatureFor(
                    nestedname.name(),
                    declarator.parameters()
                );
                Ptr<Function> func = targetScope->NestedFunction(sig);
                if (!func) {
                    func = Function::Create(
                        nestedname.name(),
                        type,
                        declarator.parameters(),
                        declarator.is_pure(),
                        spec.is_virtual()
                    );
                    if (!targetScope->AddNestedFunction(func)) {
                        throw SemanticError(
                            "Failed to add Function '"+func->name()+"' to Scope", __FILE__, __LINE__
                        );
                    }
                }
                else if (func->is_declared()) {
                    throw SemanticError(
                        "Duplicate function declaration for '"+func->id()+"' in scope "+
                        targetScope->name(), __FILE__, __LINE__
                    );
                }
                else if (func->return_type() != type) {
                    throw SemanticError(
                        "Erroneous function declaration for '"+func->id()+"' in scope "+
                        targetScope->name()+" [return type mismatch]", __FILE__, __LINE__
                    );
                }
                func->set_declared();
            } 
            else {
                Ptr<Variable> var =
                    Variable::Create(nestedname.name(), type+declarator.pointer_type());
                if (!targetScope->AddGlobalVariable(var)) {
                    throw SemanticError(
                        "Failed to add Variable '"+var->name()+"' to Scope", __FILE__, __LINE__
                    );
                }
            }
        }
        return true;
    };
    return action;
}

TypeAction AddClassToScope( Ptr<Class> classObj, const NestedNameSpecifier& nestedname) {
    TypeAction action = [classObj, nestedname] (md::Ptr<md::Scope> current_scope) {
        Ptr<Scope> targetScope = nestedname.FindNearestNestingScope(current_scope);
        if (!targetScope)
            throw SemanticError("Invalid NestedNameSpecifier("+nestedname.ToString()+")", __FILE__, __LINE__);
        if (targetScope->AddNestedClass(classObj))
            return DeclSpecifier(false, nestedname);
        throw SemanticError("Non-anonymous class cannot have empty name!", __FILE__, __LINE__);
    };
    return action;
}

ScopeAction AddFunctionToScope( const TypeAction& type_action, const parser::Declarator& declarator, bool is_default, bool is_delete) {
    ScopeAction action = [type_action, declarator, is_default, is_delete] (Ptr<Scope> current_scope) -> bool {
        DeclSpecifier spec = type_action(current_scope);
        std::string type = spec.type().ToString();
        NestedNameSpecifier nestedname = declarator.nested_name();
        Ptr<Scope> targetScope = nestedname.FindNearestNestingScope(current_scope);
        if (!declarator.has_parameters()) {
            throw SemanticError("Invalid FunctionDefinition for '"+nestedname.name()+"' - no parameters clause.", __FILE__, __LINE__);
        }
        std::string sig = Function::GetSignatureFor(nestedname.name(), declarator.parameters());
        Ptr<Function> func = targetScope->NestedFunction(sig);
        if (!func) {
            func = Function::Create(nestedname.name(), type, declarator.parameters(), declarator.is_pure(), spec.is_virtual());
            if (!targetScope->AddNestedFunction(func)) {
                throw SemanticError("Failed to define Function '"+func->id()+"' in Scope", __FILE__, __LINE__);
            }
        }
        else if (func->is_defined()) {
            throw SemanticError("Duplicate function definition for '"+func->id()+"' in scope "+targetScope->name(), __FILE__, __LINE__);
        }
        else if (func->return_type() != type) {
            throw SemanticError("Erroneous function definition for '"+func->id()+"' in scope "+targetScope->name()+" [return type mismatch]", __FILE__, __LINE__);
        }
        func->set_defined();
        func->set_default(is_default);
        func->set_deleted(is_delete);
        return true;
    };
    return action;
}

TypeAction AddEnumToScope( const std::string& base, const StringList& values, const md::NestedNameSpecifier& nestedname) {
    TypeAction action = [base, values, nestedname] (md::Ptr<md::Scope> current_scope) {
        Ptr<Scope> targetScope = nestedname.FindNearestNestingScope(current_scope);
        if (!targetScope)
            throw SemanticError("Invalid NestedNameSpecifier("+nestedname.ToString()+")", __FILE__, __LINE__);
        Ptr<Enum> en = Enum::Create(nestedname.name(), base, values);
        if (targetScope->AddNestedEnum(en))
            return DeclSpecifier(false, nestedname);
        throw SemanticError("Error adding enum '"+en->name()+"' to scope '"+targetScope->name()+"'!", __FILE__, __LINE__);
    };
    return action;
}

} // namespace parser
} // namespace opwig

