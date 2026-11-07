
cmake_minimum_required(VERSION 3.15)

project(CampusNavigation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    campus_backend
    backend/main.cpp
    backend/navigation.cpp
)

target_include_directories(
    campus_backend
    PRIVATE
    backend
)

target_compile_definitions(
    campus_backend
    PRIVATE
    ASIO_STANDALONE
)
if(WIN32)
    target_link_libraries(
        campus_backend
        PRIVATE
        ws2_32
        mswsock
    )
endif()