
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

message ("-- Python module added.")

#########
# OPA

add_library (libouroboros-python ${OUROBOROS_PYTHON_OPA_SRC})
target_link_libraries (libouroboros-python LINK_PUBLIC libouroboros ${PYTHON_LIBRARIES})
target_include_directories(libouroboros-python PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>" ${PYTHON_INCLUDE_DIRS})

set_target_properties (libouroboros-python PROPERTIES 
    PROJECT_NAME OUROBOROS-PYTHON
    OUTPUT_NAME ouroboros-python
    OUTPUT_NAME_DEBUG ouroboros-python-dbg
    FOLDER "opa"
)

install(TARGETS libouroboros-python
    EXPORT OuroborosPython
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

########
# OPWIG

add_library(libopwig-python ${OUROBOROS_PYTHON_OPWIG_SRC})
target_link_libraries(libopwig-python LINK_PUBLIC libopwig-common)
target_include_directories(libopwig-python PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
set_target_properties (libopwig-python PROPERTIES 
    OUTPUT_NAME opwig-python
    OUTPUT_NAME_DEBUG opwig-python-dbg
    FOLDER "opwig"
)

ouroboros_generate_opwig (
  python
  python/opwig/pythonspecification.h
  opa::python::PythonSpecification
  libopwig-python
)

install(TARGETS opwig-python
    EXPORT OuroborosPython
    RUNTIME DESTINATION bin
    CONFIGURATIONS Release
)

###############

#install(DIRECTORY .
#    DESTINATION include/ouroboros
#    FILES_MATCHING PATTERN "*.h" PATTERN "*.th" PATTERN "*.tcc"
#)

install(EXPORT OuroborosPython
    FILE OuroborosPythonTargets.cmake
    DESTINATION ${ConfigPackageLocation}
)