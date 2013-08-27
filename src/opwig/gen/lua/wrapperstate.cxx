
#include <opwig/gen/lua/wrapperstate.h>

#include <opwig/md/ptr.h>

namespace opwig {
namespace gen {
namespace lua {

using std::string;
using md::Ptr;

WrapperState::WrapperState (const string& the_module_name) {
    Ptr<ModuleWrap> root(new ModuleWrap);
    root->name = the_module_name;
    PushModule(root);
}

string WrapperState::StackAsString (const string& sep, size_t skip) const {
    string result;
    for (auto module : stack_) {
        if (skip > 0) {
            --skip;
            continue;
        }
        result += module->name+sep;
    }
    return result;
}

void WrapperState::PushModule (const string& module_name, bool is_class_flag) {
    Ptr<ModuleWrap> new_module(new ModuleWrap(is_class_flag));

    new_module->name = module_name;
    new_module->path = StackAsString("_");
    new_module->nesting = StackAsString("::", 1)+(is_class_flag ? "class_" : "")+module_name+"::";

    current_module()->AddChild(new_module);
    PushModule(new_module);
}

} // namespace lua
} // namespace gen
} // namespace opwig

