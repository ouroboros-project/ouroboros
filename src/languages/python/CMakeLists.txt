
SET(OUROBOROS_PYTHON_SRC 
    ./src/languages/python/modules.h 
    ./src/languages/python/pythondata.cxx 
    ./src/languages/python/pythondata.h 
    ./src/languages/python/pythonmodules.cxx 
    ./src/languages/python/pythonwrapper.cxx 
    ./src/languages/python/pythonwrapper.h 
    ./src/languages/python/swigpyrun.h 
)


include_directories ("src")
include (cmake/ouroboros.cmake)

find_package (PythonLibs 2.6)

include_directories (${PYTHON_INCLUDE_DIRS})
ouroboros_add_scriptlang (python)

if(PYTHON_DEBUG_LIBRARY AND (CMAKE_VERSION VERSION_LESS "2.8.8"))
    LIST(OUROBOROS_PYTHON_LIBRARIES optimized;${PYTHON_LIBRARIES};debug;${PYTHON_DEBUG_LIBRARY})
else()
    LIST(OUROBOROS_PYTHON_LIBRARIES ${PYTHON_LIBRARIES})
endif()

message ("-- Python modules added.")

add_library (libouroboros-python ${OUROBOROS_PYTHON_SRC})

target_link_libraries (libouroboros-python
                       ${OUROBOROS_PYTHON_LIBRARIES})

target_link_libraries (libouroboros-python LINK_INTERFACE_LIBRARIES 
                       ${OUROBOROS_PYTHON_LIBRARIES})
