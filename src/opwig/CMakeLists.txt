
SET(OUROBOROS_OPWIG_SRC
    opwig.h
    opwig.cxx
    md/accessspecifier.h
    md/metadataobject.h
    md/metadataobject.cxx
    md/container.h
    md/scope.h
    md/scope.cxx
    md/ptr.h
    md/type.h
    md/namespace.h
    md/variable.h
    md/class.h
    md/class.cxx
    md/semanticerror.h
    md/function.h
    md/enum.h
    md/parameter.h
    md/nestednamespecifier.h
    md/nestednamespecifier.cxx
    gen/proxygenerator.h
    gen/proxygenerator.cxx
    gen/proxycodeprovider.h
    gen/wrappergenerator.h
    gen/wrappergenerator.cxx
    gen/wrapperspecification.h
)

add_library(opwig-common ${OUROBOROS_OPWIG_SRC})


