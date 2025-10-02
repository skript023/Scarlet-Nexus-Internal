include(FetchContent)

message("DiscordPresence")
FetchContent_Declare(
    discord
    GIT_REPOSITORY https://github.com/skript023/Discord.git
    GIT_TAG main
    GIT_PROGRESS TRUE
)
FetchContent_GetProperties(discord)

if (NOT discord_POPULATED)
    FetchContent_Populate(discord)
    # Find all source files
    file(GLOB_RECURSE DISCORD_RPC_SRC
        "${discord_SOURCE_DIR}/src/**.cpp"
        "${discord_SOURCE_DIR}/include/**.h"
    )

    # Exclude files with the `_unix` prefix in their filename
    list(FILTER DISCORD_RPC_SRC EXCLUDE REGEX "_unix\\.cpp$")
    list(FILTER DISCORD_RPC_SRC EXCLUDE REGEX "_linux\\.cpp$")

    add_library(discord STATIC ${DISCORD_RPC_SRC})

    target_include_directories(discord PRIVATE "${discord_SOURCE_DIR}/include")

    # Ensure WIN32_LEAN_AND_MEAN is undefined for this target
    target_compile_definitions(discord PRIVATE -UWIN32_LEAN_AND_MEAN)

    set_property(TARGET discord PROPERTY COMPILE_WARNING_AS_ERROR OFF)
endif()