#include "LibraryManager.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

static std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

int LibraryManager::addBook(std::string title, std::string author, int year, std::string publisher) {
    int id = nextBookId++;
    books.emplace_back(id, std::move(title), std::move(author), year, std::move(publisher));
    return id;
}

int LibraryManager::addUser(std::string first, std::string last, std::string department) {
    int id = nextUserId++;
    User u{id, std::move(first), std::move(last)};
    if (!department.empty()) u.setDepartment(std::move(department));
    users.push_back(std::move(u));
    return id;
}
bool LibraryManager::borrowBook(int bookId, int userId) {
    //if user exist?
    bool userExists = false;
    for (const auto& u : users) {
        if (u.getId() == userId) {
            userExists = true;
            break;
        }
    }
    if (!userExists) return false;

    //find and borrow
    for (auto& b : books) {
        if (b.getId() == bookId) {
            if (b.isBorrowed()) return false;
            b.borrow(userId);
            b.setBorrowDate(getCurrentDate());
            return true;
        }
    }

    //lack of book
    return false;
}

bool LibraryManager::returnBook(int bookId) {
    for (auto& b : books) {
        if (b.getId() == bookId) {
            if (!b.isBorrowed()) return false;
            b.giveBack();
            b.setBorrowDate("");
            return true;
        }
    }
    return false; //lack of book
}

bool LibraryManager::getBookStatus(int bookId) const {
    // true = borrowed, false = avaible or doesnt exist
    for (const auto& b : books) {
        if (b.getId() == bookId) {
            return b.isBorrowed();
        }
    }
    return false;
}

std::vector<Book> LibraryManager::findBooksByTitle(const std::string& fragment) const {
    std::vector<Book> result;
    for (const auto& b : books) {
        if (b.getTitle().find(fragment) != std::string::npos) {
            result.push_back(b);
        }
    }
    return result;
}

void LibraryManager::listUsers() const {
    if (users.empty()) {
        std::cout << "Brak użytkowników.\n";
        return;
    }

    std::cout << "Lista użytkowników:\n";
    for (const auto& u : users) {
        std::cout << u.getId() << ". "
                  << u.getFirstName() << " "
                  << u.getLastName();
        

        if (!u.getDepartment().empty())
            std::cout << " (Dział: " << u.getDepartment() << ")";
        std::cout << "\n";
    }
    std::cout << '\n';
}

void LibraryManager::listBooks() const {
    if (books.empty()) {
        std::cout << "Brak książek w bibliotece.\n";
        return;
    }

    std::cout << "ID | Tytuł | Autor | Status\n";
    std::cout << "---------------------------------------------------------------\n";

    for (const auto& b : books) {
        std::cout << b.getId() << " | "
                  << b.getTitle() << " | "
                  << b.getAuthor() << " | ";

        if (!b.isBorrowed()) {
            std::cout << "dostępna";
        } else {
            std::cout << "wypozyczona (ID " 
                        << b.getBorrowerId() 
                        << ", od " 
                        << b.getBorrowDate() 
                        << ")";
        }
        std::cout << "\n";
    }
    std::cout << '\n';
}