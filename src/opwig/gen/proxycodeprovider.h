
#ifndef OPWIG_GEN_CODEPROVIDER_H_
#define OPWIG_GEN_CODEPROVIDER_H_

#include <opwig/md/function.h>
#include <opwig/md/ptr.h>

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
            "#include <opa/baseproxy.h>\n"
            "namespace generated {\n"
            "class "+proxied_class_name_+"_Proxy : public "+proxied_class_name_+","
                                                 " public opa::BaseProxy {\n"
            "  public:\n"
            "    "+proxied_class_name_+"_Proxy (const VirtualObj& the_proxy)\n"
            "        : "+proxied_class_name_+"(), proxy_(the_proxy) {}\n";
    }
    std::string CloseProxy () const {
        return
            "  private:\n"
            "    VirtualObj proxy_;\n"
            "};\n"
            "} // namespace\n"
            "#endif\n";
    }
    std::string ProxyMethod (const md::Ptr<md::Function>& method) {
        return
            "    void "+method->name()+" () {\n"
            "        (proxy_ | \""+method->name()+"\") ();\n"
            "    }\n";
    }
  private:
    std::string proxied_class_name_;
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CODEPROVIDER_H_

