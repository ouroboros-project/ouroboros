#include <opwig/md/metadataobject.h>
#include <opwig/md/scope.h>
#include <string>

namespace opwig {
namespace md {

using std::string;

string MetadataObject::nested_name(const string& separator) const {
    Ptr<Scope> scope = parent_;
    string nestedname = name_;
    while (scope) {
        nestedname = scope->name() + separator + nestedname;
        scope = scope->parent();
    }
    return nestedname;
}

} // namespace md
} // namespace opwig

