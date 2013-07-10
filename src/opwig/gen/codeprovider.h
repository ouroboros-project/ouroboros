
#ifndef OPWIG_GEN_CODEPROVIDER_H_
#define OPWIG_GEN_CODEPROVIDER_H_

#include <string>

namespace opwig {
namespace gen {

class CodeProvider {
  public:
    std::string OpenHeader (const std::string& the_name) const {
        return
            "#ifndef OPWIG_GENERATED_"+the_name+"_H_\n"+
            "#define OPWIG_GENERATED_"+the_name+"_H_\n";
    }
    std::string CloseHeader () const {
        return
            "#endif\n";
    }
    std::string OpenNamespace (const std::string& the_name) const {
        return
            "namespace "+the_name+" {\n";
    }
    std::string CloseNamespace () const {
        return
            "} // namespace\n";
    }
    std::string ProxyClass (const std::string& the_parent) const {
        return
            "class "+the_parent+"_Proxy : public "+the_parent+" {};\n";
    }
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CODEPROVIDER_H_

