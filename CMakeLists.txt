project(dlib)

set(CMAKE_CXX_STANDARD 17)

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

add_compile_definitions(BOOST_ASIO_STANDALONE)

add_executable(dlib test_client.cpp test_server.cpp)

target_include_directories(dlib PRIVATE ${Boost_SOURCE_DIR}/libs/asio/include)

install(TARGETS dlib)
target_link_libraries(dlib PRIVATE Boost::system)
if(WIN32)
    target_link_libraries(dlib PRIVATE wsock32 ws2_32)
endif()