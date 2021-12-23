project(test)

include_directories(include)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(src2 DIR_SRCS2)

set (allexe ${DIR_SRCS} ${DIR_SRCS2})

ADD_EXECUTABLE(hello ${allexe})