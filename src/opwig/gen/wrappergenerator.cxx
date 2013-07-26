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
    wrap_file.close();
}

} //end namespace gen
} //end namespace opwig
