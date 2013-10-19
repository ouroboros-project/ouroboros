
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
        if (!*it)
            throw InvalidVMError(this->machine()->lang_name(), "received a invalid VObj as argument in VObj::operator().");
        if (machine() != it->machine())
            throw InvalidVMError(this->machine()->lang_name(),
                  "in VObj::operator(): received argument of a different VM ("+it->machine()->lang_name()+").");
        arglist.push_back(it->data_);
    }
    VirtualObj ret(data_->Execute(arglist));
    return ret;
}

VirtualObj VirtualObj::Create (const char* obj, VirtualMachine* vm) {
    if (!vm) return VirtualObj();
    VirtualData::Ptr new_data = vm->NewData();
    new_data->WrapString(obj);
    return VirtualObj(new_data);
}


} /* namespace opa */
