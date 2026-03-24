# LibrarySystem

## Project description

This is a simple console application written in C++ for managing a small library.  
The system allows adding books and users, borrowing and returning books, searching, and saving data to a file.

## Features

- Add new books and users
- List books and users
- Borrow and return books
- Check book status
- Search books by title or author
- Save and load data from file

## Architecture
The project is divided into modules:

- **Models** – classes `Book` and `User`
- **LibraryManager** – business logic
- **DataStorage** – saving and loading data
- **MenuUI** – console interface

## Data storage

All data is stored in a single file (e.g. `library.txt`).

The file contains two sections:
- users
- books

Each section is saved as lines with fields separated by `;`.

### Config
cmake -S . -B build
##  Build instructions

This project uses CMake and vcpkg (Boost.Locale).

### Windows

1. Install vcpkg  
git clone https://github.com/microsoft/vcpkg  
cd vcpkg  
.\bootstrap-vcpkg.bat  

2. Install dependencies  
cd LibrarySystem  
vcpkg install  

3. Build  
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG/scripts/buildsystems/vcpkg.cmake  
cmake --build build  

4. Run  
.\build\LibrarySystem.exe  

5. Tests  
build\Debug\LibrarySystemTests.exe

###  Linux

1. Install vcpkg  
git clone https://github.com/microsoft/vcpkg  
cd vcpkg  
./bootstrap-vcpkg.sh  

2. Install dependencies  
cd LibrarySystem  
vcpkg install  

3. Build  
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=PATH_TO_VCPKG/scripts/buildsystems/vcpkg.cmake  
cmake --build build  

4. Run  
./build/LibrarySystem  

5. Tests  
build\Debug\LibrarySystemTests.exe 

### Notes
- Replace PATH_TO_VCPKG with your actual path (e.g. C:/dev/vcpkg)  
- vcpkg must be installed locally (not included in the repository)  
- Dependencies are defined in vcpkg.json 


