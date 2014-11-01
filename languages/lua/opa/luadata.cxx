
#include <algorithm>
#include <functional>

#include <lua/opa/luadata.h>
#include <lua/opa/datagear.h>

#include <opa/exceptions.h>

namespace opa {
namespace lua {

using std::vector;

LuaData::~LuaData() {
    if (machine_) try {
        machine_->data_gear()
            .SafeCall(DataGear::DestroyID)
            .Arg(id_)
            .NoResult();
    } catch (InternalVMError& e) {}
    // We must throw out errors because throwing exceptions in destructors is BAD.
}

void* LuaData::Unwrap(const VirtualType& type, bool disown) const {
    return machine_->data_gear()
        .SafeCall(DataGear::UnwrapData)
        .Arg(id_)
        .Arg(type.FromLang(LANG(Lua)))
        .Arg(static_cast<int>(disown))
        .GetResult<UData>(nullptr);
}

template <class T>
T UnwrapPrimitive(LuaMachine* machine, const DataID id, const T default_value) {
    return machine->data_gear()
        .SafeCall(DataGear::UnwrapPrimitive<T>)
        .Arg(id)
        .GetResult(default_value);
}

const char* LuaData::UnwrapString() const {
    return UnwrapPrimitive<const char*>(machine_, id_, nullptr);
}

bool LuaData::UnwrapBoolean() const {
    return UnwrapPrimitive<bool>(machine_, id_, false);
}

int LuaData::UnwrapInteger() const {
    return UnwrapPrimitive<int>(machine_, id_, 0);
}

double LuaData::UnwrapNumber() const {
    return UnwrapPrimitive<double>(machine_, id_, 0.0);
}

template <class T>
static T UnwrapSequence(LuaMachine* machine,
                        DataID seq_id) {
    DataBuffer id_list;
    machine->data_gear()
        .SafeCall(DataGear::UnwrapList)
        .Arg(seq_id)
        .Arg(&id_list)
        .NoResult();
    T data_seq;
    DataBuffer::iterator it;
    for (it = id_list.begin(); it != id_list.end(); ++it)
        data_seq.push_back(VirtualData::Ptr(new LuaData(machine, *it)));
    return data_seq;
}

LuaData::Vector LuaData::UnwrapVector() const {
    return UnwrapSequence<Vector>(machine_, id_);
}

LuaData::List LuaData::UnwrapList() const {
    return UnwrapSequence<List>(machine_, id_);
}

LuaData::Map LuaData::UnwrapMap() const {
    DataMap id_table;
    machine_->data_gear()
        .SafeCall(DataGear::UnwrapTable)
        .Arg(id_)
        .Arg(&id_table)
        .NoResult();
    Map data_table;
    DataMap::iterator it;
    for (it = id_table.begin(); it != id_table.end(); ++it) {
        data_table.insert(std::pair<VirtualData::Ptr, VirtualData::Ptr>(
            Ptr(new LuaData(machine_, it->first)),
            Ptr(new LuaData(machine_, it->second))
        ));
    }
    return data_table;
}

void LuaData::Wrap(void* data, const VirtualType& type) {
    machine_->data_gear()
        .SafeCall(DataGear::WrapData)
        .Arg(id_)
        .Arg(data)
        .Arg(type.FromLang(LANG(Lua)))
        .NoResult();
}

template <class T>
void WrapPrimitive(LuaMachine* machine, DataID id, T value) {
    machine->data_gear()
        .SafeCall(DataGear::WrapPrimitive<T>)
        .Arg(id)
        .Arg(value)
        .NoResult();
}

void LuaData::WrapString(const char* str) {
    WrapPrimitive(machine_, id_, str);
}

void LuaData::WrapBoolean(bool boolean) {
    WrapPrimitive(machine_, id_, boolean);
}

void LuaData::WrapInteger(int number) {
    WrapPrimitive(machine_, id_, number);
}

void LuaData::WrapNumber(double number) {
    WrapPrimitive(machine_, id_, number);
}

VirtualData::Ptr LuaData::Execute(const vector<Ptr>& args) {
    std::for_each(
        args.begin(),
        args.end(),
        std::mem_fn(&VirtualData::AddToBuffer)
    );
    return machine_->OperateBuffer(id_, DataGear::Execute);
}

VirtualData::Ptr LuaData::GetAttribute(Ptr key) {
    machine_->CleanBuffer();
    key->AddToBuffer();
    return machine_->OperateBuffer(id_, DataGear::GetField);
}

VirtualData::Ptr LuaData::SetAttribute(Ptr key, Ptr value) {
    key->AddToBuffer();
    value->AddToBuffer();
    return machine_->OperateBuffer(id_, DataGear::SetField);
}

void LuaData::UnsafePopValue() {
    machine_->data_gear().SetData(id_);
}

void LuaData::UnsafePushValue() {
    machine_->data_gear().GetData(id_);
}

void LuaData::AddToBuffer() {
    machine_->AddToBuffer(id_);
}

} /* namespace lua */
} /* namespace opa */
