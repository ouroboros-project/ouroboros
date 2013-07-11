#include <opwig/gen/wrappergenerator.h>

namespace opwig {
namespace gen {

void WrapperGenerator::Generate(const Ptr<const Scope>& root) {
    
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

}
}
