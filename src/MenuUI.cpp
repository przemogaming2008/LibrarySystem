#include "MenuUI.hpp"
#include <iostream>
#include <limits>
#include <iomanip>

static bool containsSemicolon(const std::string& text) {
    return text.find(';') != std::string::npos;
}

void MenuUI::printHeader() const {
    std::cout << "\n=== MENU BIBLIOTEKI ===\n";
}

void MenuUI::printMenu() const {
    std::cout
        << "1. Dodaj nową książkę\n"
        << "2. Dodaj nowego czytelnika\n"
        << "3. Wyświetl listę książek\n"
        << "4. Wyświetl listę czytelników\n"
        << "5. Wypożycz książkę\n"
        << "6. Zwróć książkę\n"
        << "7. Sprawdź status książki\n"
        << "8. Wyszukaj książki (tytuł / autor)\n"
        << "9. Wyjście\n"
        << "Wybierz opcję [1-9]: ";
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
        std::cout << "Niepoprawna liczba. Spróbuj ponownie.\n";
    }
}

std::string MenuUI::readLine(const char* prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}


void MenuUI::handleAddBook() {
    std::cout << "\n=== Dodaj nowa książkę ===\n";

    std::string title = readLine("Podaj tytuł: ");
    std::string author = readLine("Podaj autora: ");
    std::string yearStr = readLine("Podaj rok (opcjonalnie): ");

    int year = 0;
    if (!yearStr.empty()) {
        try {
            year = std::stoi(yearStr);
        } catch (...) {
            std::cout << "Niepoprawny rok – ustawiono brak.\n";
        }
    }
    std::string publisher = readLine("Podaj wydawnictwo: ");
    
    if (containsSemicolon(title) ||
        containsSemicolon(author) ||
        containsSemicolon(publisher)) {
        std::cout << "BŁĄD: Znak ';' nie jest dozwolony w danych książki.\n\n";
        return;
    }

    int id = manager.addBook(title, author, year, publisher);

    std::cout << "Książka dodana pomyślnie z ID = " << id << "\n";
}

void MenuUI::handleAddUser() {
    std::cout << "\n=== Dodaj nowego czytelnika ===\n";

    std::string first = readLine("Podaj imię: ");
    std::string last = readLine("Podaj nazwisko: ");
    std::string dept = readLine("Podaj dział (opcjonalnie): ");

    if (containsSemicolon(first) ||
        containsSemicolon(last) ||
        containsSemicolon(dept)) {
        std::cout << "BŁĄD: Znak ';' nie jest dozwolony w danych użytkownika.\n\n";
        return;
    }
    
    int id = manager.addUser(first, last, dept);

    std::cout << "Użytkownik dodany, ID = " << id << "\n";
}

void MenuUI::handleListBooks() {
    std::cout << "\n=== LISTA KSIĄŻEK ===\n";
    manager.listBooks();
}

void MenuUI::handleListUsers() {
    std::cout << "\n=== LISTA CZYTELNIKÓW ===\n";
    manager.listUsers();
}
void MenuUI::handleBorrowBook() {
    std::cout << "\n=== WYPOŻYCZ KSIĄŻKĘ ===\n";
    int bookId = readInt("Podaj ID książki: ");
    int userId = readInt("Podaj ID czytelnika: ");

    auto res = manager.borrowBook(bookId, userId);

    switch (res) {
        case BorrowResult::Ok:
            std::cout << "OK: Wypożyczono książkę.\n\n";
            break;
        case BorrowResult::UserNotFound:
            std::cout << "BŁĄD: Nie ma użytkownika o takim ID.\n\n";
            break;
        case BorrowResult::BookNotFound:
            std::cout << "BŁĄD: Nie ma książki o takim ID.\n\n";
            break;
        case BorrowResult::AlreadyBorrowed: {
            auto stOpt = manager.getBookStatus(bookId);

            if (stOpt && stOpt->isBorrowed) {
                std::cout << "BŁĄD: Książka jest już wypożyczona";

                if (const User* u = manager.findUserById(stOpt->borrowerId)) {
                    std::cout << " przez " << u->getFirstName() << " "
                            << u->getLastName()
                            << " (ID " << stOpt->borrowerId << ")";
                } else {
                    std::cout << " (ID użytkownika " << stOpt->borrowerId << ")";
                }

                if (!stOpt->borrowDate.empty()) {
                    std::cout << " od " << stOpt->borrowDate;
                }

                std::cout << ".\n";
            } else {
                std::cout << "BŁĄD: Książka jest już wypożyczona.\n";
            }
            break;
        }

    }
}

void MenuUI::handleReturnBook() {
    std::cout << "\n=== ZWRÓĆ KSIĄŻKĘ ===\n";
    int bookId = readInt("Podaj ID książki: ");

    auto res = manager.returnBook(bookId);

    switch (res) {
        case ReturnResult::Ok:
            std::cout << "OK: Zwrócono książkę.\n\n";
            break;
        case ReturnResult::BookNotFound:
            std::cout << "BŁĄD: Nie ma książki o takim ID.\n\n";
            break;
        case ReturnResult::NotBorrowed:
            std::cout << "BŁĄD: Ta książka nie jest wypożyczona.\n\n";
            break;
    }
}
void MenuUI::handleSearch() {
    std::cout << "\n=== WYSZUKIWANIE KSIĄŻEK ===\n";
    std::cout << "1. Po tytule\n";
    std::cout << "2. Po autorze\n";

    int mode = readInt("Wybierz [1-2]: ");
    std::string fragment = readLine("Podaj fragment: ");

    std::vector<Book> results;

    if (mode == 1) {
        results = manager.findBooksByTitle(fragment);
    } else if (mode == 2) {
        results = manager.findBooksByAuthor(fragment);
    } else {
        std::cout << "Niepoprawny wybór.\n\n";
        return;
    }

    if (results.empty()) {
        std::cout << "Brak wyników.\n\n";
        return;
    }

    std::cout << "Znalezione książki:\n";
    for (const auto& b : results) {
        std::cout << b.getId() << ". "
                  << b.getTitle() << " - "
                  << b.getAuthor() << "\n";
    }
    std::cout << "\n";
}


void MenuUI::handleCheckStatus() {
    std::cout << "\n=== STATUS KSIĄŻKI ===\n";
    int id = readInt("Podaj ID książki: ");

    auto stOpt = manager.getBookStatus(id);
    if (!stOpt) {
        std::cout << "BŁĄD: Nie znaleziono książki o ID " << id << ".\n\n";
        return;
    }

    const auto& st = *stOpt;

    if (!st.isBorrowed) {
        std::cout << "Książka jest dostępna.\n\n";
        return;
    }

    std::cout << "Książka jest wypożyczona (ID " << st.borrowerId;
    if (!st.borrowDate.empty())
        std::cout << ", od " << st.borrowDate;

    // jeśli dodasz findUserById:
    if (const User* u = manager.findUserById(st.borrowerId)) {
        std::cout << ", przez " << u->getFirstName() << " " << u->getLastName();
    }

    std::cout << ").\n\n";
}

void MenuUI::mainMenu() {
    while (true) {
        printHeader();
        printMenu();

        int choice{};
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Niepoprawny wybór.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: handleAddBook(); break;
            case 2: handleAddUser(); break;
            case 3: handleListBooks(); break;
            case 4: handleListUsers(); break;
            case 5: handleBorrowBook(); break;
            case 6: handleReturnBook(); break;
            case 7: handleCheckStatus(); break;
            case 8: handleSearch(); break;
            case 9: std::cout << "Koniec.\n"; return;
            default: std::cout << "Nie ma takiej opcji.\n"; break;
        }
    }
}