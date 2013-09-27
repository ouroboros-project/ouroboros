
include (opwig/parser/generate.cmake)

SET(OUROBOROS_SRC 
    ./src/opa/script.h 
    ./src/opa/baseproxy.h 
    ./src/opa/converter.h
    ./src/opa/defs.h
    ./src/opa/exceptions.h
    ./src/opa/initscripts.cxx 
    ./src/opa/langwrapper.h 
    ./src/opa/module.h 
    ./src/opa/scriptmanager.cxx 
    ./src/opa/scriptmanager.h 
    ./src/opa/type.h 
    ./src/opa/virtualdata.h 
    ./src/opa/virtualobj.cxx 
    ./src/opa/virtualobj.h 
    ./src/opa/virtualprimitive.h 
    ./src/opa/utils/idgenerator.cxx 
    ./src/opa/utils/idgenerator.h 
    ./src/opa/utils/uncopyable.h 

    ./opwig/opwig.h
    ./opwig/opwig.cxx
    ./opwig/md/accessspecifier.h
    ./opwig/md/metadataobject.h
    ./opwig/md/metadataobject.cxx
    ./opwig/md/container.h
    ./opwig/md/scope.h
    ./opwig/md/scope.cxx
    ./opwig/md/ptr.h
    ./opwig/md/type.h
    ./opwig/md/namespace.h
    ./opwig/md/variable.h
    ./opwig/md/class.h
    ./opwig/md/class.cxx
    ./opwig/md/semanticerror.h
    ./opwig/md/function.h
    ./opwig/md/enum.h
    ./opwig/md/parameter.h
    ./opwig/md/nestednamespecifier.h
    ./opwig/md/nestednamespecifier.cxx
    ./opwig/gen/proxygenerator.h
    ./opwig/gen/proxygenerator.cxx
    ./opwig/gen/proxycodeprovider.h
    ./opwig/gen/wrappergenerator.h
    ./opwig/gen/wrappergenerator.cxx
    ./opwig/gen/wrappergenerator.h
    ./opwig/gen/wrapperspecification.h

    ${OPWIG_MDPARSER_SRC}
    ${OPWIG_MDSCANNER_SRC}
)
