
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
    root->open = false;
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

string WrapperState::PushModule (const string& module_name) {
    bool open = current_module()->open;
    if (open) current_module()->open = false;
    Ptr<ModuleWrap> new_module(new ModuleWrap);

    new_module->name = module_name;
    new_module->open = false;
    new_module->parent = current_module();
    new_module->path = StackAsString("_");

    current_module()->children.push_back(new_module);
    PushModule(new_module);
    return
        string(open ? "} // namespace generated\n\n" : "")+
        "namespace "+new_module->name+" {\n";
}

} // namespace lua
} // namespace gen
} // namespace opwig

