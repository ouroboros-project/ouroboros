
#include <opwig/json/reader.h>

#include <libjson.h>

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

using std::list;
using std::string;
using std::ifstream;
using opwig::md::Ptr;
using opwig::md::Namespace;
using opwig::md::Scope;
using opwig::md::Class;

namespace opwig {
namespace json {

    namespace {
        std::pair<Ptr<Scope>, std::string> GetScopeAndName(Ptr<Scope> initial_scope, const std::string& full_name) {
            auto result = initial_scope;
            std::size_t pos = 0, prev_pos = 0;
            while ((pos = full_name.find("::", prev_pos)) != std::string::npos) {
                auto name = full_name.substr(prev_pos, pos - prev_pos);
                Ptr<Scope> next = result->NestedClass(name);
                result = next ? next : result->NestedNamespace(name);
                prev_pos = pos + 2;
            }
            return std::make_pair(result, full_name.substr(prev_pos));
        }
    }

Reader::Reader(std::istream& in, md::Ptr<md::Namespace> global)
    : global_(global)
{
    in.seekg(0, std::ios::end);
    contents_.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents_[0], contents_.size());
}

Reader::Reader(const std::string& s, md::Ptr<md::Namespace> global)
    : contents_(s)
    , global_(global) {}

bool Reader::parse() {

    auto json_root = libjson::parse(contents_);

    for (const auto& ns : json_root["namespaces"]) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, ns.as_string());
        scope->AddNestedNamespace(md::Namespace::Create(name));
    }

    return false;
}

} // namespace gen 
} // namespace opwig
