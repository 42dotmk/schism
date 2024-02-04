cmake_minimum_required(VERSION 3.18)
project(schism)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

# set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
# find_package(glfw3 CONFIG REQUIRED)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(lua REQUIRED)
find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/btree.h")

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    option(ENABLE_SANITIZER "Enable ASAN/LSAN/UBSAN." OFF)
    option(ENABLE_SANITIZER_THREAD "Enable TSAN (does not work together with ASAN)." OFF)

    if(ENABLE_SANITIZER)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=leak")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=leak")
    endif()
    if(ENABLE_SANITIZER_THREAD)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=thread")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=thread")
    endif()
    if(ENABLE_SANITIZER AND ENABLE_SANITIZER_THREAD)
        message(FATAL_ERROR "ENABLE_SANITIZER and ENABLE_SANITIZER_THREAD can not be used together!")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    endif()
    add_compile_definitions(SC_DEBUG)
else()
    add_compile_definitions(SC_NODEBUG)
endif()

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD USE_IMGUI_API)

include_directories(
                    vendor/json/include/
                    vendor/phmap/parallel_hashmap/
                    vendor/atomic_queue/include/
                    vendor/cppack/include/
                    ${Stb_INCLUDE_DIR}
                    ${LUA_INCLUDE_DIR}
                    src/)


file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
file(GLOB_RECURSE RESOURCES res/*)

source_group("Headers" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})

file(GLOB CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME}
                    ${SOURCES}
                    ${PROJECT_CONFIGS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    asio::asio
    EnTT::EnTT
    glad::glad
    glfw
    glm::glm
    imgui::imgui
    imguizmo::imguizmo
    OpenAL::OpenAL
    spdlog::spdlog
    sol2
    ${LUA_LIBRARIES}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PARALLEL_HASHMAP_INCLUDE_DIRS})

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    LIBRART_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
    DEPENDS ${RESOURCES})
