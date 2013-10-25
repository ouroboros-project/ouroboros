
#include <opa/type.h>

#include <unordered_map>

namespace opa {

using std::unordered_map;
using std::type_index;

namespace {

unordered_map<type_index, unordered_map<type_index, Convertibility>> REGISTRY;

}

Convertibility CheckConversion (const type_index& from, const type_index& to) {
    auto& row = REGISTRY[from];
    auto  it = row.find(to);
    if (it == row.end()) return Convertibility::NONE;
    return it->second;
}

void ClearConversions () {
    REGISTRY.clear();
}

void DefineConversion (const type_index& from, const type_index& to,
                       const Convertibility& conversion) {
    REGISTRY[from][to] = conversion;
}

void DefineConversion (const HeritageTable& table) {
    for (auto& heritage : table)
        for (auto& parent : heritage.second) {
            DefineConversion(heritage.first, parent, Convertibility::INHERITS);
            DefineConversion(parent, heritage.first, Convertibility::IS_BASE); 
        }
}

} // namespace opa

