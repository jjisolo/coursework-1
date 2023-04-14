cmake_minimum_required(VERSION 3.21)

project(Coursework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(deps)

add_executable(101 source/main.cc source/game.cc source/engine.cc)
target_link_libraries(101 PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML spdlog::spdlog)
target_compile_features(101 PRIVATE cxx_std_20)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET 101 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS 101)
