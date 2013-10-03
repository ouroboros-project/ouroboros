
add_executable (opwig-lua src/opwig_lua.cxx)
target_link_libraries (
  opwig-lua
  libouroboros
  libouroboros-lua
)

add_executable (opwig-python src/opwig_python.cxx)
target_link_libraries (
  opwig-python
  libouroboros
  libouroboros-python
)

