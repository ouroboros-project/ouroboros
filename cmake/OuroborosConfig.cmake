
#include(CMakeFindDependencyMacro)
#find_dependency(Stats 2.6.4)

include("${CMAKE_CURRENT_LIST_DIR}/OuroborosTargets.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/OuroborosOPWIG.cmake")

foreach(_comp ${Ouroboros_FIND_COMPONENTS})
    set(comp_file "${CMAKE_CURRENT_LIST_DIR}/Ouroboros${_comp}Targets.cmake")
    if(NOT EXISTS "${comp_file}")
        set(Ouroboros_FOUND False)
        set(Ouroboros_NOTFOUND_MESSAGE "Could not found language module: ${_comp}")
    endif()
    include(${comp_file})
endforeach()