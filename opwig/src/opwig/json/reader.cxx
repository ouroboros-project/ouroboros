
#include <opwig/json/reader.h>

#include <opwig/md/accessspecifier.h>
#include <opwig/md/namespace.h>
#include <opwig/md/function.h>
#include <opwig/md/class.h>
#include <opwig/md/variable.h>
#include <opwig/md/enum.h>
#include <opwig/json/exceptions.h>

#include <libjson.h>

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>

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

    auto void_type = md::Type::Create("void", false);

    std::map<std::string, md::AccessSpecifier> access_specifier_mapper = {
        { "private", md::AccessSpecifier::PRIVATE },
        { "protected", md::AccessSpecifier::PROTECTED },
        { "public", md::AccessSpecifier::PUBLIC },
    };

    std::map<std::string, Ptr<md::Type>> types;

    std::pair<Ptr<Scope>, std::string> GetScopeAndName(Ptr<Scope> initial_scope, const std::string& full_name) {
        auto result = initial_scope;
        std::size_t pos = 0, prev_pos = 0;
        while ((pos = full_name.find("::", prev_pos)) != std::string::npos) {
            auto name = full_name.substr(prev_pos, pos - prev_pos);
            Ptr<Scope> next = result->NestedClass(name);
            result = next ? next : result->NestedNamespace(name);
            if (!result) {
                throw MissingScope(name, full_name);
            }
            prev_pos = pos + 2;
        }
        return std::make_pair(result, full_name.substr(prev_pos));
    }

    Ptr<Class> FindClass(Ptr<Scope> initial_scope, const std::string& full_name) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(initial_scope, full_name);
        return scope->NestedClass(name);
    }

    void str_replace( string &s, const string &search, const string &replace ) {
        for( size_t pos = 0; ; pos += replace.length() ) {
            pos = s.find( search, pos );
            if( pos == string::npos ) break;

            s.erase( pos, search.length() );
            s.insert( pos, replace );
        }
    }

    std::string FixTypename(const std::string& json_type) {
        std::string result = json_type;
        str_replace(result, "class ", "");
        str_replace(result, "struct ", "");
        return result;
    }

    Ptr<md::Type> FindType(const std::string& json_type) {
        std::string fixed = FixTypename(json_type);
        auto& x = types[fixed];
        if (!x) {
            x = md::Type::Create(fixed, false);
        }
        return x;
    }

    Ptr<md::Function> CreateFunction(const JSONNode& data, bool is_method) {
        std::vector<md::Parameter> parameter_list;
        for (const auto& param : data["params"]) {
            parameter_list.emplace_back(FindType(param.as_string()));
        }

        auto func = md::Function::Create(data["name"].as_string(),
                                         data.find("return") != data.end()
                                            ? FindType(data["return"].as_string())
                                            : nullptr,
                                         parameter_list,
                                         is_method ? data["pure"].as_bool() : false,
                                         is_method ? data["virtual"].as_bool() : false,
                                         is_method ? data["const"].as_bool() : false);
        func->set_deleted(data["deleted"].as_bool());
        func->set_access(access_specifier_mapper.at(data["access"].as_string()));
        return func;
    }

    md::AccessSpecifier GetAccessSpecifier(const JSONNode& data) {
        // TODO: throw better exception
        return access_specifier_mapper.at(data.at("access").as_string());
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

    if (!libjson::is_valid(contents_)) {
        throw InvalidJson();
    }

    auto json_root = libjson::parse(contents_);

    if (json_root.find("headers") != json_root.end())
    for (const auto& header : json_root.at("headers")) {
        metadata_headers_.push_back(header.as_string());
    }

    if (json_root.find("namespaces") != json_root.end())
    for (const auto& ns : json_root.at("namespaces")) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, ns.as_string());

        if (scope->NestedNamespace(name)) continue;

        auto newns = md::Namespace::Create(name);
        newns->set_access(md::AccessSpecifier::PUBLIC);
        scope->AddNestedNamespace(newns);
    }

    if (json_root.find("classes") != json_root.end())
    for (const auto& c : json_root.at("classes")) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, c.at("qualified_name").as_string());

        std::list<md::BaseSpecifier> base_specifiers;
        if (c.find("base_class") != c.end())
        for (const auto& base : c.at("base_class")) {
            base_specifiers.emplace_back(base.at("name").as_string(),
                                         false,
                                         GetAccessSpecifier(base));
        }
        Ptr<Class> tc = Class::Create(name, base_specifiers);
        tc->set_access(GetAccessSpecifier(c));

        if (c.find("methods") != c.end())
        for (const auto& method : c.at("methods")) {
            auto func = CreateFunction(method, true);
            tc->AddNestedFunction(func);
        }

        scope->AddNestedClass(tc);
    }

    if (json_root.find("functions") != json_root.end())
    for (const auto& f : json_root.at("functions")) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, f.at("qualified_name").as_string());
        scope->AddNestedFunction(CreateFunction(f, false));
    }

    if (json_root.find("variables") != json_root.end())
    for (const auto& v : json_root.at("variables")) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, v.at("qualified_name").as_string());

        auto var = md::Variable::Create(name, FindType(v.at("type").as_string()));
        var->set_access(GetAccessSpecifier(v));
        scope->AddGlobalVariable(var);
    }

    if (json_root.find("enums") != json_root.end())
    for (const auto& e : json_root.at("enums")) {
        Ptr<Scope> scope;
        std::string name;
        std::tie(scope, name) = GetScopeAndName(global_, e.at("qualified_name").as_string());

        std::vector<std::string> values;
        for (const auto& val : e.at("values")) {
            values.push_back(val.as_string());
        }

        auto var = md::Enum::Create(name, "", values);
        var->set_access(GetAccessSpecifier(e));
        scope->AddNestedEnum(var);
    }

    return true;
}

} // namespace gen 
} // namespace opwig
