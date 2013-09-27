#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/md/accessspecifier.h>
#include <opwig/md/scope.h>
#include <opwig/md/namespace.h>
#include <opwig/md/function.h>
#include <opwig/md/variable.h>
#include <opwig/md/enum.h>
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
using md::AccessSpecifier;

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
        "    "+spec_->wrapper_name()+"Machine *vm = dynamic_cast<"+spec_->wrapper_name()+"Machine*>(\n"
        "        SCRIPT_MANAGER()->GetMachine(\""+spec_->wrapper_name()+"\")\n"
        "    );\n"
        "    if (vm == NULL) {\n"
        "        vm = new "+spec_->wrapper_name()+"Machine;\n"
        "        SCRIPT_MANAGER()->Register(vm);\n"
        "    }\n";

    for (auto scriptMod : spec_->GetGeneratedModules()) {
        bootstrap +=
            "    vm->RegisterModule("
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
        if (entry.second->access() != AccessSpecifier::PRIVATE)
            wrap_file_ << spec_->WrapFunction(entry.second) << endl;
    }
    for (auto entry : scope->IterateVariables()) {
        if (entry.second->access() != AccessSpecifier::PRIVATE)
            wrap_file_ << spec_->WrapVariable(entry.second) << endl;
    }
    for (auto entry : scope->IterateEnums()) {
        if (entry.second->access() != AccessSpecifier::PRIVATE)
            wrap_file_ << spec_->WrapEnum(entry.second) << endl;
    }
    for (auto entry : scope->IterateClasses()) {
        if (entry.second->access() != AccessSpecifier::PRIVATE)
            handleClass(entry.second);
    }
    // FIXME
    if (dynamic_cast<const md::Namespace*>(scope.get()))
        for (auto entry : scope->IterateNamespaces()) {
            if (entry.second->access() != AccessSpecifier::PRIVATE)
                handleNamespace(entry.second);
        }
}

void WrapperGenerator::handleNamespace(const Ptr<const md::Namespace>& nspace) {
    wrap_file_ << spec_->OpenNamespace(nspace) << endl;
    iterateAndWrapScope(nspace);
    wrap_file_ << spec_->CloseNamespace(nspace) << endl;
}

void WrapperGenerator::handleClass(const Ptr<const md::Class>& the_class) {
    wrap_file_ << spec_->OpenClass(the_class) << endl;
    iterateAndWrapScope(the_class);
    wrap_file_ << spec_->CloseClass(the_class) << endl;
}

} //end namespace gen
} //end namespace opwig
