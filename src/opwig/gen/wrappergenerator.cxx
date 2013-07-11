#include <opwig/gen/wrappergenerator.h>
#include <opwig/gen/wrapperspecification.h>
#include <opwig/gen/converterprovider.h>
#include <fstream>

namespace opwig {
namespace gen {

using std::ofstream;
using std::ios_base;
using std::endl;

void WrapperGenerator::Generate(const Ptr<const Scope>& root, Ptr<WrapperSpecification> spec) {
    
    ofstream wrap_file;
    wrap_file.open(output_dir_+"/"+spec->WrapperName()+"_wrap."+wrap_file_extension_, ios_base::out);
    
    wrap_file << spec->FileHeader() << endl;
    
    generateConverterClass(wrap_file, spec->GetConverterProvider());
    
    for (auto entry : root->IterateFunctions()) {
        // entry.second = Ptr<TIPO>
        wrap_file << spec->WrapFunction(entry.second) << endl;
    }
    
    wrap_file << spec->FinishFile() << endl;
    wrap_file.close();
/*
*Repeat this for each scripting language

-write HEADER to file(s)

for each metadata (MD) type:
iterate thru each instance of MD:
    -execute something for wrapping it -> generate CODE
    -write CODE(s) to file(s)
    -for scope MDs:
        call Generate() for this instance -> use recursion to
        traverse the entirety of the original given MD tree

-write FINISH/ Init function to file(s)

--------------

wrappeando uma função, gera:

scriptRTYPE OPWIG_wrap_func ( scriptARGS ) {

    T arg1 = scriptConverter.fromScript<T>(ARGS[1]);
    T arg2 = scriptConverter.fromScript<T>(ARGS[2]);
    ...
    
    RT value = func( arg1, arg2, ... );
    
    return scriptConverter.toScript<RT>(value);
}

*/
}

void WrapperGenerator::generateConverterClass(ofstream& wrap_file, const Ptr<ConverterProvider>& provider) {
    wrap_file << "class Converter final { public:" << endl;
    wrap_file << provider->GetConstructorCode() << endl;
    wrap_file << provider->GetDestructorCode() << endl;
    wrap_file << provider->GetFromFunctionsCode() << endl;
    wrap_file << provider->GetToFunctionsCode() << endl;
    wrap_file << "};" << endl;
}

}
}
