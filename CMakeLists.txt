cmake_minimum_required(VERSION 3.16)
project(LibrarySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(LibrarySystem
    src/main.cpp
    src/LibraryManager.cpp
    src/DataStorage.cpp
    src/MenuUI.cpp
)
target_include_directories(LibrarySystem PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)