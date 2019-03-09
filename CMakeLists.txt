include (ACGCommon)

set( CMAKE_CXX_STANDARD 17 )

include_directories (
  ../../..
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# collect all header and source files
acg_append_files (headers "include/*.h" .)
acg_append_files (sources "main.cpp" .)
acg_append_files (sources "source/*.cpp" .)

acg_add_executable (Delaunay ${headers} ${sources})

target_link_libraries (Delaunay
  OpenMeshCore
  OpenMeshTools
)

