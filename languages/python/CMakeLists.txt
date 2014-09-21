
SET(OUROBOROS_PYTHON_OPA_SRC 
    opa/modules.h 
    opa/pythondata.cxx 
    opa/pythondata.h 
    opa/pythonmodules.cxx 
    opa/pythonmachine.cxx 
    opa/pythonmachine.h 
    opa/pythonconverter.cxx
    opa/pythonconverter.h
    opa/wrapperbase.h
    opa/wrapperbase.cxx
)

SET(OUROBOROS_PYTHON_OPWIG_SRC 
    opwig/pythonspecification.h
    opwig/pythonspecification.cxx
    opwig/wrapscope.h
    opwig/wrapscope.cxx
    opwig/utilities.h
    opwig/utilities.cxx
)

find_package (PythonLibs 2.6 REQUIRED)

include_directories ()

set (OUROBOROS_PYTHON_LIBRARIES)
if(PYTHON_DEBUG_LIBRARY AND (CMAKE_VERSION VERSION_LESS "2.8.8"))
    LIST(APPEND OUROBOROS_PYTHON_LIBRARIES optimized;${PYTHON_LIBRARIES};debug;${PYTHON_DEBUG_LIBRARY})
else()
    LIST(APPEND OUROBOROS_PYTHON_LIBRARIES ${PYTHON_LIBRARIES})
endif()

message ("-- Python module added.")

add_library (libouroboros-python ${OUROBOROS_PYTHON_OPA_SRC})
target_include_directories(libouroboros-python PUBLIC ".." ${PYTHON_INCLUDE_DIRS})

target_link_libraries (libouroboros-python 
    LINK_PUBLIC
    ${OUROBOROS_PYTHON_LIBRARIES}
    libouroboros
)

set_target_properties (libouroboros-python PROPERTIES 
    LINK_FLAGS "${LDFLAGS}"
    PROJECT_NAME OUROBOROS-PYTHON
    OUTPUT_NAME ouroboros-python${FRAMEWORK_BIGVERSION}
    OUTPUT_NAME_DEBUG ouroboros-python${FRAMEWORK_BIGVERSION}-dbg
    SOVERSION ${FRAMEWORK_VERSION_PATCH}
    VERSION ${FRAMEWORK_VERSION}
)

ouroboros_add_langmodule(Python libouroboros-python)

######

add_library(libopwig-python ${OUROBOROS_PYTHON_OPWIG_SRC})
target_link_libraries(libopwig-python LINK_PUBLIC libopwig-common)
target_include_directories(libopwig-python PUBLIC "..")

ouroboros_generate_opwig (
  python
  python/opwig/pythonspecification.h
  opa::python::PythonSpecification
  libopwig-python
)

if (UNIX AND PYTHONLIBS_FOUND)
    set (OUROBOROS_INSTALL_LOCATION_DEFINE "#define OUROBOROS_INSTALL_LOCATION \"/usr/${OUROBOROS_INSTALL_LOCATION}\"")
endif (UNIX AND PYTHONLIBS_FOUND)

install(TARGETS libouroboros-python
    DESTINATION lib
#    CONFIGURATIONS Release
)

install(DIRECTORY . DESTINATION include/ouroboros${FRAMEWORK_BIGVERSION} FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc")


if (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
    install(DIRECTORY src/generated/ 
            DESTINATION ${OUROBOROS_INSTALL_LOCATION}/${FRAMEWORK_BIGVERSION}/python
            FILES_MATCHING PATTERN "*.py")
endif (PYTHONLIBS_FOUND AND PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
