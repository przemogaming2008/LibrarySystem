#include "LibraryManager.hpp"

bool LibraryManager::addBook(const Book& book) {
    // unique ID
    for (const auto& b : books) {
        if (b.getId() == book.getId()) {
            return false;
        }
    }
    books.push_back(book);
    return true;
}

bool LibraryManager::addUser(const User& user) {
    for (const auto& u : users) {
        if (u.getId() == user.getId()) {
            return false;
        }
    }
    users.push_back(user);
    return true;
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