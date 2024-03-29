cmake_minimum_required(VERSION 3.20)

project(ScarletNexus CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Fetch modules
message("\nFetching modules")
include(scripts/minhook.cmake)
include(scripts/json.cmake)
include(scripts/g3log.cmake)

message("\nFetching custom modules")
include(scripts/imgui.cmake)


# ScarletNexus
message(STATUS "ScarletNexus")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

add_library(ScarletNexus MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ScarletNexus PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(ScarletNexus PRIVATE 
    "${SRC_DIR}"
    "${json_SOURCE_DIR}/single_include"
    "${gtav_classes_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}"
)

target_precompile_headers(ScarletNexus PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(ScarletNexus PRIVATE minhook imgui g3log)

# Warnings as errors
set_property(TARGET ScarletNexus PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(ScarletNexus 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

add_subdirectory(injector)