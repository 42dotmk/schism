cmake_minimum_required(VERSION 3.18)
project(schism)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

option(ENABLE_SANITIZER "Enable ASAN/LSAN/UBSAN." OFF)
option(ENABLE_SANITIZER_THREAD "Enable TSAN ()." OFF)
option(USE_CPPCHECK "Enable cppcheck, must have cppcheck in the PATH")

# Hardcode the architecture to arm64 on macOS
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    add_compile_definitions(SCHISM_PLATFORM_MAC)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_compile_definitions(SCHISM_PLATFORM_WIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_compile_definitions(SCHISM_PLATFORM_LINUX)
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    message("Using GCC compiler")
    set(USING_GCC 1)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clangd")
    message("Using Clangd compiler")
    set(USING_CLANGD 1)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    message("Using msvc compiler")
    set(USING_MSVC 1)
endif()


# set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
# find_package(glfw3 CONFIG REQUIRED)
include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(bgfx CONFIG REQUIRED)

# this is commented out because there is a bug in imguizmo version 1.83
# using it as a submodule
# find_package(imguizmo CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(lua REQUIRED)
find_path(PARALLEL_HASHMAP_INCLUDE_DIRS "parallel_hashmap/btree.h")

set(CMAKE_CXX_STANDARD 20)

if(${ENABLE_SANITIZER})
    add_compile_options(-fsanitize=address -fno-sanitize=alignment -fsanitize=enum -fsanitize=leak)
    if(ENABLE_SANITIZER_THREAD)
        add_compile_options(-fsanitize=undefined -fno-sanitize=alignment -fsanitize=enum -fsanitize=thread)
    endif()

    if(ENABLE_SANITIZER AND ENABLE_SANITIZER_THREAD)
        message(FATAL_ERROR "ENABLE_SANITIZER and ENABLE_SANITIZER_THREAD can not be used together!")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(USING_MSVC)
        add_compile_options(/W4)
    elseif(USING_GCC)
        add_compile_options(-fdiagnostics-color=always)
        add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)
    elseif(USING_CLANGD)
        add_compile_options(-Weverything)
        add_compile_options(-fcolor-diagnostics)
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
                    vendor/imguizmo/
                    ${Stb_INCLUDE_DIR}
                    ${LUA_INCLUDE_DIR}
                    src/
)


file(GLOB GUIZMO_SOURCES vendor/imguizmo/*.cpp)
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
                    ${GUIZMO_SOURCES}
                    ${PROJECT_CONFIGS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    asio::asio
    EnTT::EnTT
    glad::glad
    glfw
    glm::glm
    imgui::imgui
    # imguizmo::imguizmo
    OpenAL::OpenAL
    spdlog::spdlog
    sol2
    ${LUA_LIBRARIES}
    bgfx::bx
    bgfx::bgfx
    bgfx::bimg
    bgfx::bimg_decode
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PARALLEL_HASHMAP_INCLUDE_DIRS})


set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin/)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    LIBRART_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK AND ${USE_CPPCHECK})
    message("Enabled cppcheck")
    list(
        APPEND CMAKE_CXX_CPPCHECK 
            "--enable=warning"
            "--inconclusive"
            "--force" 
            "--inline-suppr"
            # "--suppressions-list=${CMAKE_SOURCE_DIR}/CppCheckSuppressions.txt"
    )
endif()

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
    DEPENDS ${RESOURCES})
