
set (OUROBOROS_CMAKE_DIR ${CMAKE_CURRENT_LIST_DIR})

function (ouroboros_generate_opwig LANGUAGE HEADER SPECIFICATION)
  set (OPWIG_SPECIFICATION_HEADER ${HEADER})
  set (OPWIG_SPECIFICATION_CLASS ${SPECIFICATION})
  set (OPWIG_GENERATED_NAME opwig-${LANGUAGE})
  set (OPWIG_GENERATED_MAIN ${CMAKE_CURRENT_LIST_DIR}/${OPWIG_GENERATED_NAME}.cxx)
  configure_file (
    ${OUROBOROS_CMAKE_DIR}/opwig.cxx.in
    ${OPWIG_GENERATED_MAIN}
  )
  add_executable (${OPWIG_GENERATED_NAME} ${OPWIG_GENERATED_MAIN})
  target_link_libraries (
    ${OPWIG_GENERATED_NAME}
    libopwig-common
    libopwig-${LANGUAGE}
  )
  set_target_properties (${OPWIG_GENERATED_NAME} PROPERTIES FOLDER "opwig")
endfunction (ouroboros_generate_opwig)

