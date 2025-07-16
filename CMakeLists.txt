cmake_minimum_required_version(VERSION 4.0)
project(SummerGameJam)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

if(SDL3_FOUND AND SDL3_image_FOUND)
    include_directories(${SDL3_INCLUDE_DIRS} ${SDL3_image_INCLUDE_DIRS})

    add_executable(a.out main.c)
    target_link_libraries(SummerGameJam ${SDL3_LIBRARIES} ${SDL3_image_LIBRARIES})
else()
    message(FATAL_ERROR "SDL3 or SDL3_image not found. Please install them.")
endif()