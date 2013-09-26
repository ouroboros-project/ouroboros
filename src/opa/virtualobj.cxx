
#include <opa/virtualobj.h>
#include <opa/exceptions.h>

#include <algorithm>

namespace opa {

using std::vector;
using std::list;

VirtualObj VirtualObj::operator() (const list<VirtualObj>& args) const {
    vector<VirtualData::Ptr> arglist;
    list<VirtualObj>::const_iterator it;
    for (it = args.begin(); it != args.end(); ++it) {
        // Wrappers of executed VObj (we) and of the VObjs passed as
        // arguments must be the same.
        if (!*it) {
            throw InvalidVMError(this->wrapper()->lang_name(), "received a invalid VObj as argument in VObj::operator().");
            return VirtualObj();
        }
        if (wrapper() != it->wrapper()) {
            puts("[vobj::op()] Argument wrapper is not the same as the called object.");
            throw InvalidVMError(this->wrapper()->lang_name(),
                  "in VObj::operator(): received argument of a different VM ("+it->wrapper()->lang_name()+").");
            return VirtualObj();
        }
        arglist.push_back(it->data_);
    }
    VirtualObj ret(data_->Execute(arglist));
    return ret;
}

VirtualObj VirtualObj::Create (const char* obj, LangWrapper* wrapper) {
    if (!wrapper) return VirtualObj();
    VirtualData::Ptr new_data = wrapper->NewData();
    new_data->WrapString(obj);
    return VirtualObj(new_data);
}


} /* namespace opa */
