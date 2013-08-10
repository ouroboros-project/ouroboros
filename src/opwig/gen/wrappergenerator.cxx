#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/scope.h>
#include <opwig/md/namespace.h>
#include <fstream>
#include <iostream>

namespace opwig {
namespace gen {

using std::cout;
using std::ofstream;
using std::ios_base;
using std::endl;
using std::string;
using md::Ptr;

void WrapperGenerator::Generate (const string& module_name, const Ptr<const md::Scope>& root,
                                 const Ptr<WrapperSpecification>& spec) {
    spec->set_module_name(module_name);
    spec_ = spec;
    
    wrap_file_.open(output_dir_+"/"+spec->wrapper_name()+"_"+module_name+"_wrap."+wrap_file_extension_,
                   ios_base::out);
    
    wrap_file_ << spec->FileHeader() << endl;
    
    for (auto input_file : input_files_) {
        wrap_file_ << "#include <" << input_file << ">" << endl;
    }
    wrap_file_ << endl << spec->MiddleBlock() << endl;

    
    iterateAndWrapScope(root);
    
    wrap_file_ << spec->FinishFile() << endl;
    wrap_file_ << generateBootstrap() << endl;
    wrap_file_.close();
}

std::string WrapperGenerator::generateBootstrap() const {
    string bootstrap =
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
        "    cout << \"Bootstrapping "+spec_->wrapper_name()+" module "+spec_->module_name()+"\" << endl;\n"
        "    "+spec_->wrapper_name()+"Wrapper *wrapper = dynamic_cast<"+spec_->wrapper_name()+"Wrapper*>(\n"
        "        SCRIPT_MANAGER()->GetWrapper(\""+spec_->wrapper_name()+"\")\n"
        "    );\n"
        "    if (wrapper == NULL) {\n"
        "        wrapper = new "+spec_->wrapper_name()+"Wrapper;\n"
        "        SCRIPT_MANAGER()->Register(wrapper);\n"
        "    }\n";

    for (auto scriptMod : spec_->GetGeneratedModules()) {
        bootstrap +=
            "    wrapper->RegisterModule("
                    "Module<"+spec_->LoadFuncSignature()+">("
                          "\""+scriptMod.module_name+"\", "
                          +scriptMod.init_func_name+
                    ")"
                ");\n";
    }
    bootstrap +=
        "}\n\n"
        "} // unnamed namespace\n";
    return bootstrap;
}

void WrapperGenerator::iterateAndWrapScope(const Ptr<const md::Scope>& scope) {
    for (auto entry : scope->IterateFunctions()) {
        wrap_file_ << spec_->WrapFunction(entry.second) << endl;
    }
    for (auto entry : scope->IterateVariables()) {
        wrap_file_ << spec_->WrapVariable(entry.second) << endl;
    }
    for (auto entry : scope->IterateEnums()) {
        wrap_file_ << spec_->WrapEnum(entry.second) << endl;
    }
    for (auto entry : scope->IterateClasses()) {
        wrap_file_ << spec_->WrapClass(entry.second) << endl;
    }
    for (auto entry : scope->IterateNamespaces()) {
        handleNamespace(entry.second);
    }
}

void WrapperGenerator::handleNamespace(const Ptr<const md::Namespace>& nspace) {
    wrap_file_ << spec_->WrapNamespace(nspace, false) << endl;
    iterateAndWrapScope(nspace);
    wrap_file_ << spec_->WrapNamespace(nspace, true) << endl;
}

} //end namespace gen
} //end namespace opwig
