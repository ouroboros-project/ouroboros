
if(CLANGINTROSPECTOR_EXECUTABLE)
    add_library(libwrappertest binary.cc)
    target_link_libraries(libwrappertest libouroboros)
    set_target_properties (libwrappertest PROPERTIES FOLDER "Tests")

    set(MAHHEADERS
        ../../opa/src/opa/script.h 
        ../../opa/src/opa/baseproxy.h 
        ../../opa/src/opa/converter.h
        ../../opa/src/opa/defs.h
        ../../opa/src/opa/exceptions.h)
        
    ouroboros_export_metadata(libwrappertest ${MAHHEADERS})
    ouroboros_create_wrappers(libwrappertest lua)
    ouroboros_create_wrappers(libwrappertest python)
endif()