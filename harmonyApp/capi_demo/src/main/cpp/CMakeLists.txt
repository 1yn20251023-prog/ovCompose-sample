# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(CapiRenderNodeDemo)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#if(DEFINED PACKAGE_FIND_FILE)
#    include(${PACKAGE_FIND_FILE})
#endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

add_library(nativeNode SHARED Manager.cpp Container.cpp init.cpp)

find_library(
hilog-lib
hilog_ndk.z)

find_library(
libace-lib
ace_ndk.z)

find_library(
libnapi-lib
ace_napi.z)

target_link_libraries(nativeNode PUBLIC ${hilog-lib} ${libace-lib} ${libnapi-lib})
target_link_libraries(nativeNode PUBLIC libnative_drawing.so)
target_link_libraries(nativeNode PUBLIC libhitrace_ndk.z.so)
