file(REMOVE_RECURSE
  "libcece.pdb"
  "libcece.dylib"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/cece.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
