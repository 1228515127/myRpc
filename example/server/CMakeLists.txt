project(server)

#设置项目可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../MyRpc/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

add_executable(server ${SRC_LIST})

LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../MyRpc/lib)

target_link_libraries(server myrpc)