
#ifndef OPWIG_GEN_CODEPROVIDER_H_
#define OPWIG_GEN_CODEPROVIDER_H_

#include <opwig/md/function.h>
#include <opwig/md/ptr.h>

#include <string>

namespace opwig {
namespace gen {

class ProxyCodeProvider {
  public:
    ProxyCodeProvider (const std::string& the_proxied_class_name,
                       const std::string& the_header_path = "")
        : proxied_class_name_(the_proxied_class_name), header_path_(the_header_path) {}
    std::string OpenProxy () const {
        return
            "#ifndef OPWIG_GENERATED_"+proxied_class_name_+"_H_\n"
            "#define OPWIG_GENERATED_"+proxied_class_name_+"_H_\n"
            "#include \""+header_path_+"\"\n"
            "#include <opa/baseproxy.h>\n"
            "#include <opa/virtualobj.h>\n"
            "namespace generated {\n"
            "class "+proxy_name()+";\n"
            "class "+proxy_name()+" :"
              " public "+proxied_class_name_+","
              " public opa::BaseProxy<"+proxy_name()+"> {\n"
            "  public:\n"
            "    "+proxy_name()+" (const opa::VirtualObj& the_proxy)\n"
            "        : opa::BaseProxy<"+proxy_name()+">(the_proxy) {}\n";
    }
    std::string CloseProxy () const {
        return
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
    std::string header_path_;
    std::string proxy_name () const {
      return proxied_class_name_+"Proxy";
    }
};

} // namespace gen
} // namespace opwig

#endif // OPWIG_GEN_CODEPROVIDER_H_

