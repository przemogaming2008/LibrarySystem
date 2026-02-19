#pragma once
#include "LibraryManager.hpp"

class MenuUI {
public:
    MenuUI(LibraryManager& manager) : manager(manager) {}

    void mainMenu();

private:
    LibraryManager& manager;

    // handlers opcji 1-4
    void handleAddBook();
    void handleAddUser();
    void handleListBooks();
    void handleListUsers();
    void handleBorrowBook();
    void handleReturnBook();
    void handleCheckStatus();
    void handleSearchByTitle();


    // helpers UI
    void printHeader() const;
    void printMenu() const;

    int readInt(const char* prompt);
    std::string readLine(const char* prompt);

    // formatowanie
    static void printBooksTableHeader();
    static void printUsersTableHeader();
};