project(PawnManagerGUI)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_compile_definitions(CLIENT_SIDE SERVER_SIDE)

add_executable(PawnManagerGUI BuildingRenderer.cpp CraftBuildingWindow.cpp FontManager.cpp GameWindow.cpp
        GameWindowManager.cpp MainMenuWindow.cpp PawnManagerClient.cpp PawnRenderer.cpp ResourceRenderer.cpp
        ClientMain.cpp Control.cpp ButtonControl.cpp SlotControl.cpp ArrowControl.cpp BuildBuildingWindow.cpp
        BuildingResultControl.cpp)

target_link_libraries(PawnManagerGUI PRIVATE sfml-graphics PawnManagerClient)
target_compile_features(PawnManagerGUI PRIVATE cxx_std_20)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PawnManagerGUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PawnManagerGUI> $<TARGET_FILE_DIR:PawnManagerGUI> COMMAND_EXPAND_LISTS)
endif ()
