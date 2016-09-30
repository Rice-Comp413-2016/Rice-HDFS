include_directories(${CMAKE_SOURCE_DIR}/rice-namenode/translator)

add_library(rpcserver rpcserver.cc)

target_link_libraries( rpcserver translator)
