project(PawnManager)

set(CMAKE_CXX_STANDARD 20)

add_library(PawnManager src/Entities/Entity.cpp src/Player.cpp src/Player.cpp src/Task.cpp src/Entities/Buildings/Building.cpp src/Entities/Pawns/FighterPawn.cpp src/Entities/Pawns/Pawn.cpp src/Recipes/Recipe.cpp src/Entities/ResourceEntity.cpp src/Entities/Pawns/WorkerPawn.cpp src/testSystem.cpp src/Entities/Buildings/CraftBuilding.cpp src/IDmanager.cpp src/Entities/Buildings/BuildingRegisty.cpp src/Recipes/CraftRecipe.cpp src/Recipes/BuildRecipe.cpp)

add_subdirectory(src/GUI)

install(TARGETS PawnManager)