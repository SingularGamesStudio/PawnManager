project(PawnManager)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(PawnManager src/ClientMain.cpp src/GUI/PawnManagerClient.cpp src/GUI/PawnManagerClient.h src/GUI/GameWindowManager.cpp src/GUI/GameWindowManager.h src/GUI/GameWindow.cpp src/GUI/GameWindow.h src/GUI/MainMenuWindow.cpp src/GUI/MainMenuWindow.h src/GUI/PawnRenderer.cpp src/GUI/PawnRenderer.h src/Entities/Entity.cpp src/Player.h src/Player.cpp src/Player.cpp src/Task.cpp src/Entities/Buildings/Building.cpp src/Entities/Pawns/FighterPawn.cpp src/Entities/Pawns/Pawn.cpp src/Recipes/Recipe.cpp src/Entities/ResourceEntity.cpp src/Entities/Pawns/WorkerPawn.cpp src/testSystem.cpp src/Entities/Buildings/CraftBuilding.cpp)


target_link_libraries(PawnManager PRIVATE sfml-graphics)
target_compile_features(PawnManager PRIVATE cxx_std_20)


if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PawnManager POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PawnManager> $<TARGET_FILE_DIR:PawnManager> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS PawnManager)