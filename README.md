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

## 💾 Data storage

All data is stored in a single file (e.g. `library.txt`).

The file contains two sections:
- users
- books

Each section is saved as lines with fields separated by `;`.

## Case-insensitive search

Case-insensitive search works correctly for standard ASCII characters.
For Polish characters (e.g. Ż, Ł, Ś) the behavior may depend on the system and is not guaranteed.

### Config
cmake -S . -B build
### Build
cmake --build build
### Run
build\Debug\LibrarySystem.exe

### Run tests
build\Debug\LibrarySystemTests.exe
