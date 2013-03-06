
#ifndef OUROBOROS_SCRIPT_LUA_LUADATA_H_
#define OUROBOROS_SCRIPT_LUA_LUADATA_H_

#include <opa/type.h>
#include <opa/virtualdata.h>

#include <languages/lua/defs.h>
#include <languages/lua/luawrapper.h>

namespace ouroboros {
namespace lua {

class LuaData : public VirtualData {

  public:

    LuaData(LuaWrapper* wrapper, DataID id) :
        wrapper_(wrapper),
        id_(id) {}

    ~LuaData();

    void* Unwrap(const VirtualType& type, bool disown) const;
    const char* UnwrapString() const;
    bool UnwrapBoolean() const;
    int UnwrapInteger() const;
    double UnwrapNumber() const;
    Vector UnwrapVector() const;
    List UnwrapList() const;
    Map UnwrapMap() const;

    void Wrap(void* data, const VirtualType& type);
    void WrapString(const char* str);
    void WrapBoolean(bool boolean);
    void WrapInteger(int number);
    void WrapNumber(double number);

    LangWrapper* wrapper () const { return wrapper_; }

    Ptr Execute(const std::vector<Ptr>& args);

    Ptr GetAttribute(Ptr key);

    Ptr SetAttribute(Ptr key, Ptr value);
    
    void* unsafe_data() const {
        return const_cast<void*>(static_cast<const void*>(this));
    }

    DataID id () const { return id_; }

    /// Unsafely sets this LuaData objects data.
    /** WARNING: Uses the lua value on the top of the stack.
     */
    void UnsafePopValue();

    void UnsafePushValue();

    void AddToBuffer();
    void RemoveFromBuffer() {}

  private:

    LuaWrapper* wrapper_;
    DataID      id_;

};

} /* namespace lua */
} /* namespace ouroboros */

#endif /* OUROBOROS_SCRIPT_LUA_LUADATA_H_ */
