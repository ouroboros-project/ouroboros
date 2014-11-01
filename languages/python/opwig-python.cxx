
#include <opwig/opwig.h>
#include <python/opwig/pythonspecification.h>

int main (int argc, char** argv) {
    return opwig::gen::Execute<opa::python::PythonSpecification>(argc, argv);
}

