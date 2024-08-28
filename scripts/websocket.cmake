include(FetchContent)

FetchContent_Declare(
    websocket
    GIT_REPOSITORY https://github.com/dhbaird/easywsclient.git
    GIT_TAG        master
    GIT_PROGRESS TRUE
) 
message("websocket")

FetchContent_MakeAvailable(websocket)