cmake_minimum_required(VERSION 3.23)
project(Game)

set(CMAKE_CXX_STANDARD 23)
set(SFML_ROOT C:/sdk/SFML)
set(SFML_DIR C:/sdk/SFML/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(Game main.cpp)

find_package(SFML COMPONENTS graphics audio REQUIRED)

include_directories(C:/sdk/SFML/include)

target_link_libraries(Game sfml-graphics sfml-audio)
