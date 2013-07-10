
#ifndef OPWIG_GEN_CODEPROVIDER_H_
#define OPWIG_GEN_CODEPROVIDER_H_

#include <string>

namespace opwig {
namespace gen {

class ProxyCodeProvider {
  public:
    ProxyCodeProvider (const std::string& the_proxied_class_name)
        : proxied_class_name_(the_proxied_class_name) {}
    std::string OpenProxy () const {
        return
            "#ifndef OPWIG_GENERATED_"+proxied_class_name_+"_H_\n"
            "#define OPWIG_GENERATED_"+proxied_class_name_+"_H_\n"
            "namespace generated {\n"
            "class "+proxied_class_name_+"_Proxy : public "+proxied_class_name_+" {\n"
            "  public:\n";
    }
    std::string CloseProxy () const {
        return
            "};\n"
            "} // namespace\n"
            "#endif\n";
    }
  private:
    std::string proxied_class_name_;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CODEPROVIDER_H_

