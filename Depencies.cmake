include(FetchContent)

set(IMGUI_VERSION 1.87)

FetchContent_Declare(
        imgui
        URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)

# Or by tag...
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG        master
        # GIT_COMMIT 5f54b69b6aee18db846c81633f86f78c2586dded
        # ^ or like this - sometimes it's better because FetchContent won't look
        # into remote to see if branch head was updated or not - good for stable
        # tags like 'vX.X' corresponding to releases
)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

# GLAD
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
    set(GLAD_API "gl=" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
    set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")
    add_subdirectory(${glad_SOURCE_DIR} ${glad_BINARY_DIR})
endif()

# Dear ImGui
FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/docking.zip
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL 2 REQUIRED)

add_library(imgui_glfw STATIC
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp

  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui_glfw PUBLIC glfw ${OPENGL_LIBRARIES})

target_include_directories(imgui_glfw
PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/imgui
  ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
)

# GLM
if(NOT DEPENDENCIES_FORCE_DOWNLOAD AND NOT EXISTS "${CMAKE_BINARY_DIR}/_deps/glm-external-src")
  find_package(glm CONFIG)
  # NOTE 1: GLM 0.9.9.0 in Ubuntu 18.04 repo doesn't install the IMPORTED
  #         INTERFACE target, only the legacy variable is defined in glm-config.cmake
  # NOTE 2: auto-fetched subproject build doesn't define the (legacy) variable
  #         anymore, only the INTERFACE target
  #
  # To avoid every test depening on GLM define their deps using
  #
  # add_sample(
  # LIBS
  #   $<$<TARGET_EXISTS:glm::glm>:glm::glm>
  # INCLUDES
  #   $<$<NOT:$<TARGET_EXISTS:glm::glm>>:"${GLM_INCLUDE_DIRS}">
  # )
  #
  # we create the INTERFACE target in case it didn't exist.
  if(glm_FOUND AND NOT TARGET glm::glm)
    add_library(glm::glm INTERFACE)
    target_include_directories(glm::glm INTERFACE "${GLM_INCLUDE_DIRS}")
  endif()
endif()

if(NOT (glm_FOUND OR TARGET glm::glm))
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/_deps/glm-external-src")
    if(DEPENDENCIES_FORCE_DOWNLOAD)
      message(STATUS "DEPENDENCIES_FORCE_DOWNLOAD is ON. Fetching glm.")
    else()
      message(STATUS "Fetching glm.")
    endif()
    message(STATUS "Adding glm subproject: ${CMAKE_BINARY_DIR}/_deps/glm-external-src")
  endif()
  cmake_minimum_required(VERSION 3.11)
  include(FetchContent)
  FetchContent_Declare(
    glm-external
    GIT_REPOSITORY      https://github.com/g-truc/glm
    GIT_TAG             0.9.9.8 # e79109058964d8d18b8a3bb7be7b900be46692ad
  )
  FetchContent_MakeAvailable(glm-external)
endif()

# Spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x
)

FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif ()
