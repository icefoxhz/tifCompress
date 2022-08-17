cmake_minimum_required(VERSION 3.22)
project (tifCompress)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "Debug")

####################  Release生成pbd文件 ####################
#告诉编译器生成调试信息
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
#告诉链接器生成exe时加入调试信息
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
#告诉链接器生成dll时加入调试信息
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

####################  设置输出目录 ####################
#message(STATUS  ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(LIB_DIR ${BUILD_DIR}/Release)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(LIB_DIR ${BUILD_DIR}/Debug)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${LIB_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LIB_DIR}/bin)

####################  设置相关的include的目录 ####################
set(INC_DIR
        "E:/tools/windows/c++/thirdLib/LuaBridge-master/Source"
        "E:/tools/windows/lua/LuaJIT/src"
        )
include_directories(${INC_DIR})

####################  设置相关的库的目录 ####################
set(LINK_DIR
        "E:/tools/windows/lua/LuaJIT/src"
        )
link_directories(${LINK_DIR})

####################  获取所有.h .cpp文件，添加进去 ####################
file(GLOB HEAD_FILES src/*.h)
file(GLOB SOURCE_FILES src/*.cpp)
add_executable(${PROJECT_NAME} ${HEAD_FILES} ${SOURCE_FILES})

#message(${HEAD_FILES})
#message(${SOURCE_FILES})

####################  设置相关库文件 ####################
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_link_libraries(${PROJECT_NAME}
            lua51.lib
            luajit.lib
            )
else()
    target_link_libraries(${PROJECT_NAME}
            lua51.lib
            luajit.lib
            )
endif ()
