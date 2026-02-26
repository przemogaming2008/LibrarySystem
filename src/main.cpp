#include "DataStorage.hpp"
#include "LibraryManager.hpp"
#include "MenuUI.hpp"
#include "Models.hpp"
#include <iostream>
#include "DataStorage.hpp"

int main(){
    std::setlocale(LC_ALL, "pl_PL.UTF-8");

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

    storage.loadAll(manager);

    MenuUI ui(manager);
    ui.mainMenu();

    storage.saveAll(manager);
    return 0;

}