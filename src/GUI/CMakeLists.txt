project(PawnManagerGUI)

set(CMAKE_CXX_STANDARD 20)
#
#set(BOOST_INCLUDE_LIBRARIES asio system)
#set(BOOST_ENABLE_CMAKE ON)

#include(FetchContent)
#
#Set(FETCHCONTENT_QUIET FALSE)
#FetchContent_Declare(
#        boost
#        GIT_REPOSITORY https://github.com/boostorg/boost.git
#        GIT_TAG boost-1.80.0
#)
#FetchContent_MakeAvailable(boost)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_compile_definitions(BOOST_ASIO_STANDALONE)

add_executable(PawnManagerGUI ClientMain.cpp BuildingRenderer.cpp CraftBuildingWindow.cpp FontManager.cpp GameWindow.cpp
        GameWindowManager.cpp MainMenuWindow.cpp PawnManagerClient.cpp PawnRenderer.cpp ResourceRenderer.cpp
        Control.cpp ButtonControl.cpp SlotControl.cpp ArrowControl.cpp BuildBuildingWindow.cpp
        BuildingResultControl.cpp TextInputControl.cpp)

target_compile_definitions(PawnManagerGUI PUBLIC CLIENT_SIDE BOOST_ASIO_STANDALONE)
target_include_directories(PawnManagerGUI PRIVATE ${Boost_SOURCE_DIR}/libs/asio/include)
target_link_libraries(PawnManagerGUI PRIVATE sfml-graphics PawnManagerClient)
target_link_libraries(PawnManagerGUI PRIVATE Boost::system)
if (WIN32)
    target_link_libraries(PawnManagerGUI PRIVATE wsock32 ws2_32)
else ()
    target_link_libraries(PawnManagerGUI PRIVATE pthread)
endif ()
target_compile_features(PawnManagerGUI PRIVATE cxx_std_20)

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PawnManagerGUI POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PawnManagerGUI> $<TARGET_FILE_DIR:PawnManagerGUI> COMMAND_EXPAND_LISTS)
endif ()
