add_library(translator ClientNamenodeProtocolImpl.cc)

target_link_libraries( translator protobuf )
target_link_libraries( translator messages )
