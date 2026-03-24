cmake_minimum_required(VERSION 3.16)
project(LibrarySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost CONFIG REQUIRED COMPONENTS locale)

add_executable(LibrarySystem
    src/main.cpp
    src/LibraryManager.cpp
    src/DataStorage.cpp
    src/MenuUI.cpp
)
target_include_directories(LibrarySystem PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(LibrarySystem PRIVATE Boost::locale)

add_executable(LibrarySystemTests
    tests/test_library.cpp
    src/LibraryManager.cpp
    src/DataStorage.cpp
)
target_include_directories(LibrarySystemTests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(LibrarySystemTests PRIVATE Boost::locale)

enable_testing()
add_test(NAME LibrarySystemTests COMMAND LibrarySystemTests)