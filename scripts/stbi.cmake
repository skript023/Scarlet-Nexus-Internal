include(FetchContent)

FetchContent_Declare(
    stbi
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
    GIT_PROGRESS TRUE
) 
message("stbi")

FetchContent_MakeAvailable(stbi)