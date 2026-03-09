#include "DataStorage.hpp"
#include "LibraryManager.hpp"
#include "MenuUI.hpp"
#include "Models.hpp"
#include <iostream>
#include "DataStorage.hpp"
#include <clocale>
#include <filesystem>

int main(){
    if (!std::setlocale(LC_ALL, "pl_PL.UTF-8") &&
        !std::setlocale(LC_ALL, "Polish_Poland.1250")) {
        std::setlocale(LC_ALL, "");
        std::cerr << "Warning: Polish locale not available,\n";
    }

    // LibraryManager manager;

    // Book book{1, "Hobbit", "Tolkien", 1937, "Allen & Unwin"};
    // User user{1,"Jan","Kowalski"};

    // manager.addBook(book);
    // manager.addUser(user);

    // std::cout << "is book borrowed: "<< book.isBorrowed() << std::endl;
    
    // std::cout << "\n=== LISTA KSIAZEK ===\n";
    // manager.listBooks();

    // std::cout << "=== LISTA UZYTKOWNIKOW ===\n";
    // manager.listUsers();

    // return 0;

    LibraryManager manager;
    DataStorage storage;

    const std::string dataDir = "data";
    const std::string booksFile = dataDir + "/books.txt";
    const std::string usersFile = dataDir + "/users.txt";

    std::filesystem::create_directories(dataDir);

    if (!storage.loadAll(manager, booksFile, usersFile)) {
        std::cerr << "Nie udalo sie wczytac danych.\n";
    }

    MenuUI ui(manager);
    ui.mainMenu();

    if (!storage.saveAll(manager, booksFile, usersFile)) {
        std::cerr << "Nie udalo sie zapisac danych.\n";
        return 1;
    }
    return 0;

}