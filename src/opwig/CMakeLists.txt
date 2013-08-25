
cmake_minimum_required (VERSION 2.8)

include (parser/generate.cmake)

set (
  OPWIG_SRC
  ${CMAKE_CURRENT_BINARY_DIR}/md/accessspecifier.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/metadataobject.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/metadataobject.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/md/container.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/scope.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/scope.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/md/ptr.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/type.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/namespace.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/variable.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/class.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/class.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/md/semanticerror.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/function.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/enum.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/parameter.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/nestednamespecifier.h
  ${CMAKE_CURRENT_BINARY_DIR}/md/nestednamespecifier.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/proxygenerator.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/proxygenerator.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/proxycodeprovider.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/wrappergenerator.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/wrappergenerator.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/wrappergenerator.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/wrapperspecification.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/wrapperspecification.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/wrapperspecification.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/codes.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/codes.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/wraps.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/wrapperstate.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/lua/wrapperstate.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/pythonspecification.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/pythonspecification.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/wrapmodule.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/wrapmodule.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/utilities.h
  ${CMAKE_CURRENT_BINARY_DIR}/gen/python/utilities.cxx
  ${OPWIG_MDPARSER_SRC}
  ${OPWIG_MDSCANNER_SRC}
)

add_executable (
  opwig
  main.cxx
  ${OPWIG_SRC}
)

# Export to parent scope
set (OPWIG_SRC ${OPWIG_SRC} PARENT_SCOPE)

