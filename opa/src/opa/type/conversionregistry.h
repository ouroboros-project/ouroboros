
#ifndef OUROBOROS_TYPE_CONVERSIONREGISTRY_H_
#define OUROBOROS_TYPE_CONVERSIONREGISTRY_H_

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace opa {
namespace type {

enum class Convertibility {
    NONE, IS_BASE, INHERITS
};

using HeritageTable = std::unordered_map<std::type_index, std::vector<std::type_index>>;

class ConversionRegistry final {

  public:

    Convertibility CheckConversion (const std::type_index& from, const std::type_index& to);

    void DefineConversion (const std::type_index& from, const std::type_index& to,
                           const Convertibility& conversion);

    void DefineConversion (const HeritageTable& table);

  private:

    template <typename T>
    using TypeRegistry = std::unordered_map<std::type_index, T>;

    TypeRegistry<TypeRegistry<Convertibility>> registry_;

};

} // namespace type
} // namespace opa

#endif // OUROBOROS_TYPE_CONVERSIONREGISTRY_H_

