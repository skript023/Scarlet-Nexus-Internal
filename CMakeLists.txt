cmake_minimum_required(VERSION 3.20)

project(ScarletNexus CXX)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/openssl")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")   # opsional
set(OPENSSL_LIBRARIES "${OPENSSL_ROOT_DIR}/lib")         # opsional
set(ZLIB_LIBRARIES "${OPENSSL_LIBRARIES}/zlib.lib")

find_package(OpenSSL REQUIRED)

get_target_property(ssl_loc OpenSSL::SSL LOCATION)
get_target_property(crypto_loc OpenSSL::Crypto LOCATION)

message(STATUS "OpenSSL::SSL    -> ${ssl_loc}")
message(STATUS "OpenSSL::Crypto -> ${crypto_loc}")

# Fetch modules
message("\nFetching modules")
include(scripts/minhook.cmake)
include(scripts/json.cmake)
include(scripts/g3log.cmake)

message("\nFetching custom modules")
include(scripts/imgui.cmake)
include(scripts/stbi.cmake)
include(scripts/asio.cmake)
include(scripts/websocket.cmake)
include(scripts/discord.cmake)

# ScarletNexus
message(STATUS "ScarletNexus")
file(GLOB_RECURSE SRC_MAIN
    "${SRC_DIR}/**.hpp"
    "${SRC_DIR}/**.h"
    "${SRC_DIR}/**.cpp"
    "${SRC_DIR}/**.cc"
    "${SRC_DIR}/**.cxx"
)

if (MSVC)
  add_compile_options(/bigobj)
else ()
  add_compile_options(-Wa,-mbig-obj)
endif ()

add_library(ScarletNexus MODULE "${SRC_MAIN}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(TARGET ScarletNexus PROPERTY CXX_STANDARD 23) # 23 Because std::format is not avalible in std:c++20 for some reason. Maybe it's because i use v142 toolset.

source_group(TREE ${SRC_DIR} PREFIX "src" FILES ${SRC_MAIN})

target_include_directories(ScarletNexus PRIVATE 
    "${SRC_DIR}"
    "${SRC_DIR}/unreal_engine"
    "${SRC_DIR}/services"
    "${json_SOURCE_DIR}/single_include"
    "${imgui_SOURCE_DIR}"
    "${stbi_SOURCE_DIR}"
    "${websocket_SOURCE_DIR}"
    "${asio_SOURCE_DIR}/asio/include"
    "${discord_SOURCE_DIR}/include"
    "${OPENSSL_INCLUDE_DIR}"
)

target_precompile_headers(ScarletNexus PRIVATE "${SRC_DIR}/common.hpp")
target_link_libraries(ScarletNexus PRIVATE minhook imgui g3log discord ${ZLIB_LIBRARIES} OpenSSL::SSL OpenSSL::Crypto Crypt32)

# Warnings as errors
#set_property(TARGET ScarletNexus PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_compile_definitions(ScarletNexus 
    "_CRT_SECURE_NO_WARNINGS"
    "NOMINMAX"
    "WIN32_LEAN_AND_MEAN"
)

add_subdirectory(injector)