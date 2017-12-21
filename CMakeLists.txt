cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(cmake/HunterGate.cmake)

HunterGate(
URL "https://github.com/YAKOVLENKO/hunter/archive/v0.19.137.1.tar.gz"
SHA1 "3ebd749c5c1d8319feb8e3fb1c86aa6a53f99239"
)
project(builder)

hunter_add_package(Boost COMPONENTS program_options system filesystem)
find_package(Boost CONFIG REQUIRED program_options system filesystem)

add_executable(builder ${CMAKE_CURRENT_SOURCE_DIR}/builder.cpp)
target_link_libraries(builder Boost::program_options Boost::system Boost::filesystem)

install(TARGETS builder RUNTIME DESTINATION bin)
