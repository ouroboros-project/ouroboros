
#ifndef OUROBOROS_SCRIPT_VIRTUALTYPE_H_
#define OUROBOROS_SCRIPT_VIRTUALTYPE_H_

#include <opa/defs.h>

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

extern "C" {
struct swig_type_info {};
}

namespace opa {

enum class Convertibility {
    NONE, IS_BASE, INHERITS
};

using HeritageTable = std::unordered_map<std::type_index, std::vector<std::type_index>>;

Convertibility CheckConversion (const std::type_index& from, const std::type_index& to);

void ClearConversions ();

void DefineConversion (const std::type_index& from, const std::type_index& to,
                       const Convertibility& conversion);

void DefineConversion (const HeritageTable& table);

class VirtualType {

  public:

    VirtualType() : types_(10, (struct swig_type_info*)nullptr) {}
    ~VirtualType() {}

    struct swig_type_info* FromLang(LangID id) const {
        return types_[id];
    }

    void RegisterFromLang(struct swig_type_info* info, LangID id) {
        if (!types_[id]) types_[id] = info;
    }

  private:

    typedef std::vector<struct swig_type_info*> TypeFromLang;

    TypeFromLang types_;

};


template <class T>
class TypeRegistry {

  public:

    static VirtualType& type () {
        return type_;
    }

  private:

    TypeRegistry () {}

    static VirtualType type_;

};

template <class T>
VirtualType TypeRegistry<T>::type_;

} /* namespace opa */

template <class T>
static void RegisterType(T* tp) {
    (void)tp;
}

template <typename T>
T* GetNull () { return nullptr; }

#endif /* OUROBOROS_SCRIPT_VIRTUALTYPE_H_ */
