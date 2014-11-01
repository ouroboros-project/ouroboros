
#include <opwig/opwig.h>
#include <lua/opwig/wrapperspecification.h>

int main (int argc, char** argv) {
    return opwig::gen::Execute<opa::lua::WrapperSpecification>(argc, argv);
}

