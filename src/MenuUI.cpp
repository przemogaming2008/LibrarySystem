#include "MenuUI.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

void MenuUI::printHeader() const {
    std::cout << "\n=== MENU BIBLIOTEKI ===\n";
}

void MenuUI::printMenu() const {
    std::cout
        << "1. Dodaj nowa ksiazke\n"
        << "2. Dodaj nowego czytelnika\n"
        << "3. Wyswietl liste ksiazek\n"
        << "4. Wyswietl liste czytelnikow\n"
        << "9. Wyjscie\n"
        << "Wybierz opcje [1-4,9]: ";
}



int MenuUI::readInt(const char* prompt) {
    while (true) {
        std::cout << prompt;
        int value{};
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Niepoprawna liczba. Sprobuj ponownie.\n";
    }
}

std::string MenuUI::readLine(const char* prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void MenuUI::printBooksTableHeader() {
    std::cout << "ID  | "
              << std::left << std::setw(25) << "Tytul" << " | "
              << std::left << std::setw(20) << "Autor" << " | "
              << "Status\n";
    std::cout << "-----------------------------------------------------------------\n";
}

void MenuUI::printUsersTableHeader() {
    std::cout << "ID  | "
              << std::left << std::setw(20) << "Imie i nazwisko" << " | "
              << "Dzial\n";
    std::cout << "-----------------------------------------------------------------\n";
}

void MenuUI::handleAddBook() {
    std::cout << "\n=== Dodaj nowa ksiazke ===\n";
    int id = readInt("Podaj ID: ");
    std::string title = readLine("Podaj tytul: ");
    std::string author = readLine("Podaj autora: ");
    int year = readInt("Podaj rok: ");
    std::string publisher = readLine("Podaj wydawnictwo: ");

    Book book{id, title, author, year, publisher};
    if (manager.addBook(book)) {
        std::cout << "OK: Dodano ksiazke.\n";
    } else {
        std::cout << "BLAD: Ksiazka o takim ID juz istnieje.\n";
    }
}

void MenuUI::handleAddUser() {
    std::cout << "\n=== Dodaj nowego czytelnika ===\n";
    int id = readInt("Podaj ID: ");
    std::string first = readLine("Podaj imie: ");
    std::string last = readLine("Podaj nazwisko: ");
    std::string dept = readLine("Podaj dzial (opcjonalnie): ");

    User user{id, first, last};
    if (!dept.empty()) user.setDepartment(dept);

    if (manager.addUser(user)) {
        std::cout << "OK: Dodano czytelnika.\n";
    } else {
        std::cout << "BLAD: Czytelnik o takim ID juz istnieje.\n";
    }
}

void MenuUI::handleListBooks() {
    std::cout << "\n=== LISTA KSIAZEK ===\n";
    manager.listBooks();
}

void MenuUI::handleListUsers() {
    std::cout << "\n=== LISTA CZYTELNIKOW ===\n";
    manager.listUsers();
}

void MenuUI::mainMenu() {
    while (true) {
        printHeader();
        printMenu();

        int choice{};
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Niepoprawny wybor.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: handleAddBook(); break;
            case 2: handleAddUser(); break;
            case 3: handleListBooks(); break;
            case 4: handleListUsers(); break;
            case 9: std::cout << "Koniec.\n"; return;
            default: std::cout << "Nie ma takiej opcji.\n"; break;
        }
    }
}