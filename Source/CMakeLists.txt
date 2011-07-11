# The header files
SET(HEADERS
  itkSingleBitBinaryImage.h
  itkSliceContiguousImage.h
  itkPixelMapImage.h
)

# The implementation files
SET(SOURCES
  itkSingleBitBinaryImage.txx
  itkSliceContiguousImage.txx
  itkPixelMapImage.txx
  perf01.cxx
)

# Add this as include directory
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${SOURCE_PATH}
)

# Main library
ADD_EXECUTABLE(Perf01 ${HEADERS} ${SOURCES})
TARGET_LINK_LIBRARIES(Perf01 ${ITK_LIBRARIES})
