#include <opwig/md/nestednamespecifier.h>
#include <opwig/md/scope.h>
#include <opwig/md/namespace.h>
#include <opwig/md/class.h>
#include <opwig/md/semanticerror.h>

#include <iostream>
namespace opwig {
namespace md {

void NestedNameSpecifier::AddPath(const std::string& path) {
    paths_.push_back(path);
}

Ptr<Scope> NestedNameSpecifier::FindNearestNestingScope(Ptr<Scope> initial_scope) const {
    Ptr<Scope> scope = initial_scope;
    
    if (global_) {
        std::cout << "Going to backtrack scope, initial Scope name = " << scope->name() << std::endl;
        while (scope->parent()) {
            std::cout << "Scope name = " << scope->name() << std::endl;
            scope = scope->parent();
        }
    }
    std::cout << "finish Scope name = " << scope->name() << std::endl;

    for (const std::string& it : paths_) {
        if (scope->NestedNamespace(it))
            scope = scope->NestedNamespace(it);
        else if (scope->NestedClass(it))
            scope = scope->NestedClass(it);
        else
            throw SemanticError("Invalid path '"+(it)+"' in NestedNameSpecifier("+ToString()+").", __FILE__, __LINE__);
    }
    return scope;
}

std::string NestedNameSpecifier::ToString() const {
    std::string result = "";
    if (global_) result += "::";
    
    for (const std::string& it : paths_)
        result += it + "::";
    return result + name_;
}

}
}
