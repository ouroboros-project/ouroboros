#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/gen/converterprovider.h>
#include <opwig/md/scope.h>
#include <fstream>

namespace opwig {
namespace gen {

using std::ofstream;
using std::ios_base;
using std::endl;
using md::Ptr;

void WrapperGenerator::Generate(const std::string& module_name, const Ptr<const md::Scope>& root, Ptr<WrapperSpecification> spec) {
    spec->set_module_name(module_name);
    
    ofstream wrap_file;
    wrap_file.open(output_dir_+"/"+spec->WrapperName()+"_"+module_name+"_wrap."+wrap_file_extension_, ios_base::out);
    
    wrap_file << spec->FileHeader() << endl << endl;
    
    generateConverterClass(wrap_file, spec->WrapperName(), spec->GetConverterProvider());
    
    for (auto entry : root->IterateFunctions()) {
        // entry.second = Ptr<TIPO>
        wrap_file << spec->WrapFunction(entry.second) << endl;
    }
    
    wrap_file << spec->FinishFile() << endl;
    wrap_file.close();
}

void WrapperGenerator::generateConverterClass(ofstream& wrap_file, const std::string& wrapper_name, const Ptr<ConverterProvider>& provider) {
    wrap_file <<  "/***************** CONVERTER **************************/" << endl;
    wrap_file << "#ifndef __OPWIG_GENERATED_" << wrapper_name << "_CONVERTER_" << endl;
    wrap_file << "#define __OPWIG_GENERATED_" << wrapper_name << "_CONVERTER_" << endl;
    wrap_file << "class " << wrapper_name << "Converter final { public:" << endl;
    wrap_file << provider->GetConstructorCode() << endl << endl;
    wrap_file << provider->GetDestructorCode() << endl << endl;
    wrap_file << provider->GetFromFunctionsCode() << endl << endl;
    wrap_file << provider->GetToFunctionsCode() << endl;
    wrap_file << "};" << endl;
    wrap_file << "#endif //" << wrapper_name << "_CONVERTER" << endl;
    wrap_file << "/******************************************************/" << endl << endl;
}

}
}
