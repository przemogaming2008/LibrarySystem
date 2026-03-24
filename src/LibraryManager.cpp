#include "LibraryManager.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <boost/locale.hpp>

static bool isBlank(const std::string& text) {
    return text.find_first_not_of(" \t\n\r") == std::string::npos;
}

static bool containsForbiddenChars(const std::string& text) {
    return text.find(';') != std::string::npos ||
           text.find('\n') != std::string::npos ||
           text.find('\r') != std::string::npos;
}

static bool isValidBookData(const std::string& title,
                            const std::string& author,
                            const std::string& publisher) {
    return !isBlank(title) &&
           !isBlank(author) &&
           !containsForbiddenChars(title) &&
           !containsForbiddenChars(author) &&
           !containsForbiddenChars(publisher);
}

static bool isValidUserData(const std::string& first,
                            const std::string& last,
                            const std::string& department,
                            const std::string& email) {
    return !isBlank(first) &&
           !isBlank(last) &&
           !containsForbiddenChars(first) &&
           !containsForbiddenChars(last) &&
           !containsForbiddenChars(department) &&
           !containsForbiddenChars(email);
}
static std::string toLowerUtf8(const std::string& text) {
    static boost::locale::generator gen;
    static const std::locale loc = gen("");
    return boost::locale::to_lower(text, loc);
}

static std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

int LibraryManager::addBook(std::string title, std::string author, int year, std::string publisher) {
    if (!isValidBookData(title, author, publisher)) {
        return -1;
    }
    int id = nextBookId++;
    books.emplace_back(id, std::move(title), std::move(author), year, std::move(publisher));
    return id;
}

int LibraryManager::addUser(std::string first, std::string last, std::string department, std::string email) {
    if (!isValidUserData(first, last, department, email)) {
        return -1;
    }

    int id = nextUserId++;
    User u{id, std::move(first), std::move(last)};

    if (!department.empty()) {
        u.setDepartment(std::move(department));
    }

    if (!email.empty()) {
        u.setEmail(std::move(email));
    }

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
    
    if (fragment.find_first_not_of(" \t\n\r") == std::string::npos) {
        return {};
    }

    std::vector<Book> result;

    const std::string frag = toLowerUtf8(fragment);

    for (const auto& b : books) {
        const std::string title = toLowerUtf8(b.getTitle());
        if (title.find(frag) != std::string::npos) {
            result.push_back(b);
        }
    }
    return result;
}
std::vector<Book> LibraryManager::findBooksByAuthor(const std::string& fragment) const {
    
     if (fragment.find_first_not_of(" \t\n\r") == std::string::npos) {
        return {};
    }

    std::vector<Book> result;

    const std::string frag = toLowerUtf8(fragment);

    for (const auto& b : books) {
        const std::string author = toLowerUtf8(b.getAuthor());
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
    if (!isValidBookData(book.getTitle(), book.getAuthor(), book.getPublisher())) {
        return false;
    }

    for (const auto& b : books) {
        if (b.getId() == book.getId()) return false;
    }

    books.push_back(book);
    return true;
}

bool LibraryManager::addUserFromStorage(const User& user) {
    if (!isValidUserData(user.getFirstName(),
                         user.getLastName(),
                         user.getDepartment(),
                         user.getEmail())) {
        return false;
    }

    for (const auto& u : users) {
        if (u.getId() == user.getId()) return false;
    }

    users.push_back(user);
    return true;
}

int LibraryManager::countInvalidBorrowers() const {
    int invalidCount = 0;

    for (const auto& b : books) {
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
            ++invalidCount;
        }
    }

    return invalidCount;
}
