# LibrarySystem

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
