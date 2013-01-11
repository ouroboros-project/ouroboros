
find_package (PythonLibs 2.6)

if (PYTHONLIBS_FOUND)
    include_directories (${PYTHON_INCLUDE_DIRS})
    ouroboros_add_scriptlang (python)
    
    set(OUROBOROS_PYTHON_ENABLED True)

    LIST(APPEND OUROBOROS_SRC ${OUROBOROS_PYTHON_SRC}) 
    if(PYTHON_DEBUG_LIBRARY AND (CMAKE_VERSION VERSION_LESS "2.8.8"))
        LIST(APPEND OUROBOROS_LIBRARIES optimized;${PYTHON_LIBRARIES};debug;${PYTHON_DEBUG_LIBRARY})
    else()
        LIST(APPEND OUROBOROS_LIBRARIES ${PYTHON_LIBRARIES})
    endif()
    
    message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)
    message ("Could NOT find Python. No Python script modules will be compiled.")
endif (PYTHONLIBS_FOUND)

