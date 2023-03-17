project(dlib2)

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

add_executable(dlib2 test_client.cpp)

target_include_directories(dlib2 PRIVATE ${Boost_SOURCE_DIR}/libs/asio/include)

install(TARGETS dlib2)
target_link_libraries(dlib2 PRIVATE Boost::system)
if(WIN32)
    target_link_libraries(dlib2 PRIVATE wsock32 ws2_32)
else()
    target_link_libraries(dlib2 PRIVATE pthread)
endif()