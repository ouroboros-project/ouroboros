#include <opwig/md/nestednamespecifier.h>
#include <opwig/md/scope.h>
#include <opwig/md/semanticerror.h>

namespace opwig {
namespace md {

void NestedNameSpecifier::AddPath(const std::string& path) {
    paths_.push_back(path);
}

Ptr<Scope> Evaluate(Ptr<Scope> initial_scope) {
    Ptr<Scope> scope = initial_scope;
    
    if (global_) {
        while (scope->parent())
            scope = scope->parent();
    }
    
    auto it = paths_.begin();
    for (; it != paths_.end(); ++it) {
        if (scope->NestedNamespace(*it))
            scope = scope->NestedNamespace(*it);
        else if (scope->NestedClass(*it))
            scope = scope->NestedClass(*it);
        else
            throw SemanticError("Invalid path '"+(*it)+"' in NestedNameSpecifier.", __FILE__, __LINE__);
    }
    return scope;
}

}
}
