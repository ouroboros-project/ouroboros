
set (OUROBOROS_LANGUAGES_LIST)

if (OUROBOROS_LUA_BINDINGS)
    add_subdirectory( lua )
    list(APPEND OUROBOROS_LANGUAGES_LIST lua)
endif()

if (OUROBOROS_PYTHON_BINDINGS)
    add_subdirectory( python )
    list(APPEND OUROBOROS_LANGUAGES_LIST python)
endif()

