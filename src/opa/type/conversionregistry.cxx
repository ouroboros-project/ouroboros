
#include <opa/type/conversionregistry.h>

namespace opa {
namespace type {

using std::unordered_map;
using std::type_index;

Convertibility ConversionRegistry::CheckConversion (const type_index& from, const type_index& to) {
    auto& row = registry_[from];
    auto  it = row.find(to);
    if (it == row.end()) return Convertibility::NONE;
    return it->second;
}

void ConversionRegistry::DefineConversion (const type_index& from, const type_index& to,
                       const Convertibility& conversion) {
    registry_[from][to] = conversion;
}

void ConversionRegistry::DefineConversion (const HeritageTable& table) {
    for (auto& heritage : table)
        for (auto& parent : heritage.second) {
            DefineConversion(heritage.first, parent, Convertibility::INHERITS);
            DefineConversion(parent, heritage.first, Convertibility::IS_BASE); 
        }
}

} // namespace type
} // namespace opa

