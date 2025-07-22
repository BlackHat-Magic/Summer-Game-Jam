cmake_minimum_required(VERSION 4.0)
project (SpacePirates LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

option(COMPILE_SHADERS "Compile shaders with glslangValidator" ON)
if(COMPILE_SHADERS)
    find_program(GLSLANG_VALIDATOR glslangValidator)
    if(NOT GLSLANG_VALIDATOR)
        message(WARNING "glslangValidator not found; shaders won't be compiled automatically.")
    else()
        set(SHADER_DIR ${CMAKE_SOURCE_DIR}/src/shaders)
        set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
        file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

        set(SHADERS triangle.vert triangle.frag)

        foreach(SHADER ${SHADERS})
            set(INPUT ${SHADER_DIR}/${SHADER})
            set(OUTPUT ${SHADER_OUTPUT_DIR}/${SHADER}.spv)
            add_custom_command(
                OUTPUT ${OUTPUT}
                COMMAND ${GLSLANG_VALIDATOR} -G -o ${OUTPUT} ${INPUT} --target-env vulkan1.3
                DEPENDS ${INPUT}
                COMMENT "Compiling shader ${SHADER}"
            )
            list(APPEND SHADER_OUTPUTS ${OUTPUT})
        endforeach()

        add_custom_target(Shaders ALL DEPENDS ${SHADER_OUTPUTS})
    endif()
endif()

# add executable
add_executable(space-pirates src/main.c)

# link sdl3
target_link_libraries(space-pirates PRIVATE SDL3::SDL3 m)

# include dirs
target_include_directories(space-pirates PRIVATE ${SDL3_INCLUDE_DIRES})

# output dir
set_target_properties(space-pirates PROPERTIES RUNTIME_OUTPUT_DIRECTORU ${CMAKE_BINARY_DIR})

# if shaders compile, ensure target depends
if(COMPILE_SHADERS AND GLSLANG_VALIDATOR)
    add_dependencies(space-pirates Shaders)
endif()

# enable debug by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()