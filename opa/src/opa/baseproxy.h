#ifndef OUROBOROS_MODULE_PROXY_CACHE_H_
#define OUROBOROS_MODULE_PROXY_CACHE_H_

#include <map>
#include <opa/virtualobj.h>

namespace opa {

template <class T>
class BaseProxy {
public:
    BaseProxy(const opa::VirtualObj& proxy) : proxy_(proxy) {}
    ~BaseProxy() {
        BaseProxy::table_.erase(proxy_.unsafe_data());
    }

    opa::VirtualObj get_proxy_vobj() const { return proxy_; }
    

    static void Set(void* key, T* object) { BaseProxy::table_[key] = object; }

    static bool Check(const opa::VirtualObj& proxy) {
        return BaseProxy::table_.count(proxy.unsafe_data()) > 0;
    }

    static T* Get(const opa::VirtualObj& proxy) {
        void* key = proxy.unsafe_data();
        T* obj;
        if (BaseProxy::table_.count(key) == 0) {
            obj = new T(proxy);
            Set(key, obj);
        }
        else {
            obj = BaseProxy::table_[key]; 
        }
        return obj;
    }

    static T* Get(opa::VirtualData* proxy) {
        void* key = proxy->unsafe_data();
        T* obj;
        if (BaseProxy::table_.count(key) == 0) {
            obj = new T(VirtualObj(VirtualData::Ptr(proxy)));
            Set(key, obj);
        }
        else {
            obj = BaseProxy::table_[key];
        }
        return obj;
    }

protected:
    opa::VirtualObj proxy_;

private:
    static std::map<void*, T*> table_;
};

template <class T>
std::map<void*, T*> BaseProxy<T>::table_;

}
#endif
