include(FetchContent)

FetchContent_Declare(
    websocket
    GIT_REPOSITORY https://gitlab.com/eidheim/Simple-WebSocket-Server.git
    GIT_TAG        266edcc925eb17f8e5b3f35be0b055b35f65267f
    GIT_PROGRESS TRUE
) 
message("websocket")

FetchContent_GetProperties(websocket)
if(NOT websocket_POPULATED)
    FetchContent_Populate(websocket)
endif()