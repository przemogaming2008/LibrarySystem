#include "LibraryManager.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
    return s;
}

static std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

int LibraryManager::addBook(std::string title, std::string author, int year, std::string publisher) {
    if (title.find_first_not_of(" \t\n\r") == std::string::npos ||
        author.find_first_not_of(" \t\n\r") == std::string::npos) {
        return -1;
    }
    int id = nextBookId++;
    books.emplace_back(id, std::move(title), std::move(author), year, std::move(publisher));
    return id;
}

int LibraryManager::addUser(std::string first, std::string last, std::string department) {
    if (first.find_first_not_of(" \t\n\r") == std::string::npos ||
        last.find_first_not_of(" \t\n\r") == std::string::npos) {
        return -1;
    }
    int id = nextUserId++;
    User u{id, std::move(first), std::move(last)};
    if (!department.empty()) u.setDepartment(std::move(department));
    users.push_back(std::move(u));
    return id;
}
BorrowResult LibraryManager::borrowBook(int bookId, int userId) {

    for (auto& b : books) {
        if (b.getId() == bookId) {

            if (b.isBorrowed()) {
                return BorrowResult::AlreadyBorrowed;
            }

            bool userExists = false;
            for (const auto& u : users) {
                if (u.getId() == userId) {
                    userExists = true;
                    break;
                }
            }

            if (!userExists) {
                return BorrowResult::UserNotFound;
            }

            b.borrow(userId);
            b.setBorrowDate(getCurrentDate());
            return BorrowResult::Ok;
        }
    }

    return BorrowResult::BookNotFound;
}

ReturnResult LibraryManager::returnBook(int bookId) {
    for (auto& b : books) {
        if (b.getId() == bookId) {
            if (!b.isBorrowed()) return ReturnResult::NotBorrowed;

            b.giveBack();
            return ReturnResult::Ok;
        }
    }
    return ReturnResult::BookNotFound;
}

std::optional<BookStatus> LibraryManager::getBookStatus(int bookId) const {
    for (const auto& b : books) {
        if (b.getId() == bookId) {
            BookStatus st;
            st.isBorrowed = b.isBorrowed();
            st.borrowerId = b.getBorrowerId();
            st.borrowDate = b.getBorrowDate();
            return st;
        }
    }
    return std::nullopt;
}

std::vector<Book> LibraryManager::findBooksByTitle(const std::string& fragment) const {
    std::vector<Book> result;

    const std::string frag = toLower(fragment);

    for (const auto& b : books) {
        const std::string title = toLower(b.getTitle());
        if (title.find(frag) != std::string::npos) {
            result.push_back(b);
        }
    }
    return result;
}
std::vector<Book> LibraryManager::findBooksByAuthor(const std::string& fragment) const {
    std::vector<Book> result;

    const std::string frag = toLower(fragment);

    for (const auto& b : books) {
        const std::string author = toLower(b.getAuthor());
        if (author.find(frag) != std::string::npos) {
            result.push_back(b);
        }
    }
    return result;
}

const User* LibraryManager::findUserById(int userId) const {
    for (const auto& u : users) {
        if (u.getId() == userId) return &u;
    }
    return nullptr;
}
const Book* LibraryManager::findBookById(int bookId) const {
    for (const auto& b : books) {
        if (b.getId() == bookId) {
            return &b;
        }
    }
    return nullptr;
}
bool LibraryManager::addBookFromStorage(const Book& book) {
    for (const auto& b : books) {
        if (b.getId() == book.getId()) return false;
    }
    books.push_back(book);
    return true;
}

bool LibraryManager::addUserFromStorage(const User& user) {
    for (const auto& u : users) {
        if (u.getId() == user.getId()) return false;
    }
    users.push_back(user);
    return true;
}

void LibraryManager::fixInvalidBorrowers() {
    for (auto& b : books) {
        if (!b.isBorrowed()) {
            continue;
        }

        bool borrowerExists = false;
        for (const auto& u : users) {
            if (u.getId() == b.getBorrowerId()) {
                borrowerExists = true;
                break;
            }
        }

        if (!borrowerExists) {
            b.giveBack();
        }
    }
}
