#include "DataStorage.hpp"
#include "LibraryManager.hpp"
#include "MenuUI.hpp"
#include "Models.hpp"
#include <iostream>
#include <clocale>
#include <filesystem>

int main(){
    if (!std::setlocale(LC_ALL, "pl_PL.UTF-8") &&
        !std::setlocale(LC_ALL, "Polish_Poland.1250")) {
        std::setlocale(LC_ALL, "");
        std::cerr << "Warning: Polish locale not available,\n";
    }

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