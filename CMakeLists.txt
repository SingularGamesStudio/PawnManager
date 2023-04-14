project(PawnManager)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_INCLUDE_LIBRARIES asio system)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.80.0
)
FetchContent_MakeAvailable(boost)

add_library(PawnManagerClient src/Entities/Entity.cpp src/Player.cpp src/Player.cpp src/Task.cpp
        src/Entities/Buildings/Building.cpp src/Entities/Pawns/FighterPawn.cpp src/Entities/Pawns/Pawn.cpp
        src/Recipes/Recipe.cpp src/Entities/ResourceEntity.cpp src/Entities/Pawns/WorkerPawn.cpp src/testSystem.cpp
        src/Entities/Buildings/CraftBuilding.cpp src/IDmanager.cpp src/Entities/Buildings/BuildingRegisty.cpp
        src/Recipes/CraftRecipe.cpp src/Recipes/BuildRecipe.cpp)
target_include_directories(PawnManagerClient PRIVATE ${Boost_SOURCE_DIR}/libs/asio/include)

target_compile_definitions(PawnManagerClient PUBLIC CLIENT_SIDE SERVER_SIDE BOOST_ASIO_STANDALONE)

add_subdirectory(src/GUI)

install(TARGETS PawnManagerClient)
target_link_libraries(PawnManagerClient PRIVATE Boost::system)
if (WIN32)
    target_link_libraries(PawnManagerClient PRIVATE wsock32 ws2_32)
else ()
    target_link_libraries(PawnManagerClient PRIVATE pthread)
endif ()

add_executable(PawnManagerServer src/Entities/Entity.cpp src/Player.cpp src/Player.cpp src/Task.cpp
        src/Entities/Buildings/Building.cpp src/Entities/Pawns/FighterPawn.cpp src/Entities/Pawns/Pawn.cpp
        src/Recipes/Recipe.cpp src/Entities/ResourceEntity.cpp src/Entities/Pawns/WorkerPawn.cpp src/testSystem.cpp
        src/Entities/Buildings/CraftBuilding.cpp src/IDmanager.cpp src/Entities/Buildings/BuildingRegisty.cpp
        src/Recipes/CraftRecipe.cpp src/Recipes/BuildRecipe.cpp src/ServerMain.cpp)

target_include_directories(PawnManagerServer PRIVATE ${Boost_SOURCE_DIR}/libs/asio/include)

target_compile_definitions(PawnManagerServer PUBLIC SERVER_SIDE BOOST_ASIO_STANDALONE)

install(TARGETS PawnManagerServer)
target_link_libraries(PawnManagerServer PRIVATE Boost::system)
if (WIN32)
    target_link_libraries(PawnManagerServer PRIVATE wsock32 ws2_32)
else ()
    target_link_libraries(PawnManagerServer PRIVATE pthread)
endif ()