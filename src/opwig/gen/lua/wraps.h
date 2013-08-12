
#ifndef OPWIG_GEN_LUA_WRAPS_H_
#define OPWIG_GEN_LUA_WRAPS_H_

#include <opwig/md/ptr.h>

#include <string>
#include <list>

namespace opwig {
namespace gen {
namespace lua {

struct DataWrap {
    std::string name;
    std::string nesting;
};

struct Module {
    std::string                 name, path;
    std::list<DataWrap>         functions;
    std::list<DataWrap>         getters;
    std::list<DataWrap>         setters;
    std::list<md::Ptr<Module>>  children;
    md::WeakPtr<Module>         parent;
    bool                        open;
};

} // namespace lua
} // namespace gen
} // namespace opwig

#endif

