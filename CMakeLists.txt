cmake_minimum_required(VERSION 3.21)

project(Coursework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(deps)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
endif()


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(101 "source/Main.cpp" "source/engine/Window.cpp" "source/engine/Application.cpp" "source/engine/rendering/Renderer.cpp" "source/engine/ResourceManager.cpp")
target_link_libraries(101 PRIVATE glfw glad glm::glm spdlog::spdlog)
target_compile_features(101 PRIVATE cxx_std_20)

add_custom_command(TARGET 101 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:101> "../../../run_tree/")

install(TARGETS 101)
