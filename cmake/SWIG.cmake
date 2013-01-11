
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${SRC_DIR}/generated")
set (GENERATED_SRC "")

# Does this work everywhere?
find_package (SWIG)

macro (ouroboros_add_scriptlang lang)

  swig_add_module (ouroboros_${lang} ${lang} ${OUROBOROS_MODULE_SRC} ${FRAMEWORK_SRC})
  set (TEMP_ONLY_CXX)
  foreach (it ${swig_generated_sources})
    if (${it} MATCHES ".cc$")
      set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
    endif ()
  endforeach (it)
  set (GENERATED_SRC ${GENERATED_SRC} ${TEMP_ONLY_CXX})
  unset (TEMP_ONLY_CXX)

endmacro (ouroboros_add_scriptlang lang)

if (SWIG_FOUND)

    include (cmake/UseSWIG.cmake)

    # Is OUROBOROS_MODULE_SRC defined?
    #if (NOT OUROBOROS_MODULE_SRC)
    #    message (FATAL_ERROR "Variable OUROBOROS_MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
    #endif (NOT OUROBOROS_MODULE_SRC)

    #set_source_files_properties (${OUROBOROS_MODULE_SRC} PROPERTIES CPLUSPLUS ON)
    #set_source_files_properties (${OUROBOROS_MODULE_SRC} PROPERTIES SWIG_FLAGS "")
    
    set(OUROBOROS_SWIG_ENABLED True)
  
    set(MODULES_LIST "")
    #foreach(it ${OUROBOROS_MODULE_SRC})
    #    get_filename_component(val ${it} NAME_WE)
    #    set(MODULES_LIST "${MODULES_LIST} \\\n    ACTION(LANG, ${val})")
    #endforeach()
    
    set(LANGUAGES_LIST "")
    foreach(it ${OUROBOROS_LANGUAGES_LIST})
        ouroboros_setup_package (${it})
        string(TOUPPER ${it} itUPPER)
        if(OUROBOROS_${itUPPER}_ENABLED)
            set(LANGUAGES_LIST "${LANGUAGES_LIST} \\\n    ACTION(${itUPPER})")
        endif()
    endforeach()
    
    set(MODULES_NAME UGDK)
    set(GENERATED_SRC ${GENERATED_SRC} modules.cc)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/modules.cc.in modules.cc)
  
else (SWIG_FOUND)

  message (WARNING "Could NOT find SWIG. No script modules will be compiled.")

endif (SWIG_FOUND)

