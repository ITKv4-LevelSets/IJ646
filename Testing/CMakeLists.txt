set( TEST_SOURCES
  SliceContiguousImageTest01
  SparseImageTest01
  SingleBitBinaryImageTest01
)

#
# Configure the Executables
#
foreach( var ${TEST_SOURCES} )
  add_executable( ${var} ${var}.cxx )
  target_link_libraries( ${var} ${ITK_LIBRARIES} )

  add_test( NAME ${var} COMMAND ${var} )
endforeach()
