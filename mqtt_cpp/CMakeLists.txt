cmake_minimum_required(VERSION 3.0.0)
project(lgmqtt VERSION 0.1.0)

include(CTest)
enable_testing()

set ( PROJECT_LINK_LIBS libmosquitto.so )
#link_directories( ~/usr/local/lib )
#include_directories(~/usr/include)

add_library(
    mqtt_lib
    MyMosquitto.h
    MyMosquitto.cpp
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE mqtt_lib mosquitto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
