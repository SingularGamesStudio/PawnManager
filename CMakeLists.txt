project(PawnManager)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(PawnManager ClientMain.cpp PawnManagerClient.cpp PawnManagerClient.h GameWindowManager.cpp GameWindowManager.h GameWindow.cpp GameWindow.h MainMenuWindow.cpp MainMenuWindow.h PawnRenderer.cpp PawnRenderer.h Entity.cpp Player.h Player.cpp Player.cpp Task.cpp Building.cpp FighterPawn.cpp Pawn.cpp Recipe.cpp ResourceEntity.cpp WorkerPawn.cpp)


target_link_libraries(PawnManager PRIVATE sfml-graphics)
target_compile_features(PawnManager PRIVATE cxx_std_20)


if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PawnManager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PawnManager> $<TARGET_FILE_DIR:PawnManager> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS PawnManager)