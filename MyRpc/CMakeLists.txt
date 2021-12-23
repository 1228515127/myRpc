project(myrpc)

#设置项目编译头文件搜索路径 -I
include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

#设置项目库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


add_library(myrpc ${SRC_LIST})

# target_link_libraries(myrpc muduo_net muduo_base pthread protobuf zookeeper_mt)
target_link_libraries(myrpc muduo_net muduo_base protobuf zookeeper_mt pthread)