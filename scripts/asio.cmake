include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        efdc25ab99786101351a5afb39f01dfaf0781401
    GIT_PROGRESS TRUE
) 
message("Asio")

FetchContent_MakeAvailable(asio)