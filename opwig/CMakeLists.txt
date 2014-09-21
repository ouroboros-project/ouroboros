
SET(OUROBOROS_OPWIG_SRC
    src/opwig/opwig.h
    src/opwig/opwig.cxx
    src/opwig/md/accessspecifier.h
    src/opwig/md/metadataobject.h
    src/opwig/md/metadataobject.cxx
    src/opwig/md/container.h
    src/opwig/md/scope.h
    src/opwig/md/scope.cxx
    src/opwig/md/ptr.h
    src/opwig/md/type.h
    src/opwig/md/namespace.h
    src/opwig/md/variable.h
    src/opwig/md/class.h
    src/opwig/md/class.cxx
    src/opwig/md/semanticerror.h
    src/opwig/md/function.h
    src/opwig/md/enum.h
    src/opwig/md/parameter.h
    src/opwig/md/nestednamespecifier.h
    src/opwig/md/nestednamespecifier.cxx
    src/opwig/gen/proxygenerator.h
    src/opwig/gen/proxygenerator.cxx
    src/opwig/gen/proxycodeprovider.h
    src/opwig/gen/wrappergenerator.h
    src/opwig/gen/wrappergenerator.cxx
    src/opwig/gen/wrapperspecification.h
)

add_library(libopwig-common ${OUROBOROS_OPWIG_SRC})
target_include_directories(libopwig-common PUBLIC src)
target_link_libraries(libopwig-common libjson)


