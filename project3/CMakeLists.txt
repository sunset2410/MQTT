cmake_minimum_required(VERSION 3.0.0)
project (TestMosquitto)


include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(main ${SOURCES})

#g++ main.cpp -lmosquitto
#g++ main.cpp 
target_link_libraries(main PRIVATE mosquitto)

# broker.emqx.io ok
# broker.hivemq.com not ok