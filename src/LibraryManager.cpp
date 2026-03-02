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
BorrowResult LibraryManager::borrowBook(int bookId, int userId) {
    bool userExists = false;
    for (const auto& u : users) {
        if (u.getId() == userId) {
            userExists = true;
            break;
        }
    }
    if (!userExists) return BorrowResult::UserNotFound;

    // find book
    for (auto& b : books) {
        if (b.getId() == bookId) {
            if (b.isBorrowed()) return BorrowResult::AlreadyBorrowed;

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
            std::cout << "wypożyczona (ID " 
                        << b.getBorrowerId() 
                        << ", od " 
                        << b.getBorrowDate() 
                        << ")";
        }
        std::cout << "\n";
    }
    std::cout << '\n';
}
const User* LibraryManager::findUserById(int userId) const {
    for (const auto& u : users) {
        if (u.getId() == userId) return &u;
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