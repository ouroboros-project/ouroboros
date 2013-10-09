
#include <opwig/opwig.h>
#include <languages/lua/wrapperspecification.h>

int main (int argc, char** argv) {
    return opwig::gen::Execute<opa::lua::WrapperSpecification>(argc, argv);
}
