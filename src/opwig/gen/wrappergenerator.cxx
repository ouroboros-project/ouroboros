#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/scope.h>
#include <fstream>
#include <iostream>

namespace opwig {
namespace gen {

using std::cout;
using std::ofstream;
using std::ios_base;
using std::endl;
using md::Ptr;

void WrapperGenerator::Generate (const std::string& module_name, const Ptr<const md::Scope>& root,
                                 const Ptr<WrapperSpecification>& spec) {
    spec->set_module_name(module_name);
    
    ofstream wrap_file;
    wrap_file.open(output_dir_+"/"+spec->wrapper_name()+"_"+module_name+"_wrap."+wrap_file_extension_,
                   ios_base::out);
    
    wrap_file << spec->FileHeader() << endl << endl;
    
    for (auto entry : root->IterateFunctions()) {
        wrap_file << spec->WrapFunction(entry.second) << endl;
    }
    for (auto entry : root->IterateVariables()) {
        wrap_file << spec->WrapVariable(entry.second) << endl;
    }
    for (auto entry : root->IterateEnums()) {
        wrap_file << spec->WrapEnum(entry.second) << endl;
    }
    for (auto entry : root->IterateClasses()) {
        wrap_file << spec->WrapClass(entry.second) << endl;
    }
    for (auto entry : root->IterateNamespaces()) {
        wrap_file << spec->WrapNamespace(entry.second) << endl;
    }
    
    wrap_file << spec->FinishFile() << endl;
    wrap_file << generateBootstrap(spec) << endl;
    wrap_file.close();
}

std::string WrapperGenerator::generateBootstrap(const md::Ptr<WrapperSpecification>& spec) const {
    return 
        // Bootstrap implementation
        "namespace {\n\n"
        // Bootstrap class
        "class Bootstrap final {\n"
        "  public:\n"
        "    Bootstrap ();\n"
        "};\n\n"
        // Bootstrap object
        "Bootstrap entry_point;\n\n"
        "Bootstrap::Bootstrap () {\n"
        "    cout << \"Bootstrapping "+spec->wrapper_name()+" module \\\"\" << "+spec->module_name()+" << \"\\\"\" << endl;\n"
        "    "+spec->wrapper_name()+"Wrapper *wrapper = dynamic_cast<"+spec->wrapper_name()+"Wrapper*>(\n"
        "        SCRIPT_MANAGER()->GetWrapper(\""+spec->wrapper_name()+"\")\n"
        "    );\n"
        "    if (wrapper == NULL) {\n"
        "        wrapper = new "+spec->wrapper_name()+"Wrapper;\n"
        "        SCRIPT_MANAGER()->Register(wrapper);\n"
        "    }\n"
        "    wrapper->RegisterModule("
                "Module<"+spec->LoadFuncSignature()+">(\""+spec->module_name()+"\", "+spec->LoadFuncName()+")"
             ");\n"
        "}\n\n"
        "} // unnamed namespace\n"
}

} //end namespace gen
} //end namespace opwig
