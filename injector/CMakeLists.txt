cmake_minimum_required(VERSION 3.20)

project(Injector CXX ASM_MASM)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

# Injector
message(STATUS "Injector")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
    "${SRC_DIR}/**.asm"
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  enable_language("RC")
  set (ICON ${SRC_DIR}/injector.rc)
endif()

if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

add_executable(Injector "${SRC_MAIN}" ${ICON})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET Injector PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN} )

target_include_directories(Injector PRIVATE 
    "${SRC_DIR}"
    "${json_SOURCE_DIR}/single_include"
    "${gtav_classes_SOURCE_DIR}"
    "${imgui_SOURCE_DIR}"
)

target_precompile_headers(Injector PRIVATE "${SRC_DIR}/common.hpp")

# Warnings as errors
set_property(TARGET Injector PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(Injector 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)