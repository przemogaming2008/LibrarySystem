#include "DataStorage.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <filesystem>

static std::vector<std::string> splitToVector(const std::string& line) {
    std::vector<std::string> out;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, ';')) {
        out.push_back(token);
    }
    return out;
}

static std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() &&
           std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    size_t end = s.size();
    while (end > start &&
           std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    return s.substr(start, end - start);
}

static bool containsForbiddenStorageChars(const std::string& text) {
    return text.find(';') != std::string::npos ||
           text.find('\n') != std::string::npos ||
           text.find('\r') != std::string::npos;
}

static bool safeToInt(const std::string& text, int& out) {
    try {
        std::string t = trim(text);
        if (t.empty()) {
            return false;
        }

        size_t pos = 0;
        int value = std::stoi(t, &pos);

        if (pos != t.size()) {
            return false;
        }

        out = value;
        return true;
    }
    catch (...) {
        return false;
    }
}

static void removeUtf8Bom(std::string& line) {
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line.erase(0, 3);
    }
}

bool DataStorage::loadAll(LibraryManager& manager,
                          const std::string& booksFile,
                          const std::string& usersFile) {
    int maxBookId = 0;
    int maxUserId = 0;

    bool loadOk = true;
    bool usersExists = std::filesystem::exists(usersFile);
    bool booksExists = std::filesystem::exists(booksFile);

    // USERS
    {
        std::ifstream in(usersFile);
        if (usersExists && !in) {
            loadOk = false;
        }
        else if (in) {
            std::string line;
            while (std::getline(in, line)) {
                removeUtf8Bom(line);

                if (trim(line).empty()) {
                    continue;
                }

                auto f = splitToVector(line);
                // id;firstName;lastName;department;email
                if (f.size() < 3) {
                    std::cerr << "Pominięto niepełny rekord użytkownika: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                int id;
                if (!safeToInt(f[0], id)) {
                    std::cerr << "Pominięto rekord użytkownika z błędnym ID: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                std::string first = f.size() > 1 ? trim(f[1]) : "";
                std::string last  = f.size() > 2 ? trim(f[2]) : "";
                std::string dept  = f.size() > 3 ? trim(f[3]) : "";
                std::string email = f.size() > 4 ? trim(f[4]) : "";

                User u{id, first, last};
                if (!dept.empty()) {
                    u.setDepartment(dept);
                }
                if (!email.empty()) {
                    u.setEmail(email);
                }

                if (!manager.addUserFromStorage(u)) {
                    std::cerr << "Pominięto zduplikowany rekord użytkownika: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                if (id > maxUserId) {
                    maxUserId = id;
                }
            }
        }
    }

    // BOOKS
    {
        std::ifstream in(booksFile);
        if (booksExists && !in) {
            loadOk = false;
        }
        else if (in) {
            std::string line;
            while (std::getline(in, line)) {
                removeUtf8Bom(line);

                if (trim(line).empty()) {
                    continue;
                }

                auto f = splitToVector(line);
                // id;title;author;year;publisher;borrowerId;borrowDate
                if (f.size() < 5) {
                    std::cerr << "Pominięto niepełny rekord książki: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                int id;
                if (!safeToInt(f[0], id)) {
                    std::cerr << "Pominięto rekord książki z błędnym ID: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                std::string title     = f.size() > 1 ? trim(f[1]) : "";
                std::string author    = f.size() > 2 ? trim(f[2]) : "";
                std::string publisher = f.size() > 4 ? trim(f[4]) : "";

                int year = 0;
                if (f.size() > 3 && !trim(f[3]).empty()) {
                    if (!safeToInt(f[3], year)) {
                        std::cerr << "Pominięto rekord książki z błędnym rokiem: "
                                  << line << "\n";
                        loadOk = false;
                        continue;
                    }
                }

                int borrowerId = -1;
                if (f.size() > 5 && !trim(f[5]).empty()) {
                    if (!safeToInt(f[5], borrowerId)) {
                        std::cerr << "Pominięto rekord książki z błędnym borrowerId: "
                                  << line << "\n";
                        loadOk = false;
                        continue;
                    }
                }

                std::string borrowDate = f.size() > 6 ? trim(f[6]) : "";

                Book b{id, title, author, year, publisher};

                if (borrowerId != -1) {
                    b.borrow(borrowerId);
                    b.setBorrowDate(borrowDate);
                }

                if (!manager.addBookFromStorage(b)) {
                    std::cerr << "Pominięto zduplikowany rekord książki: "
                              << line << "\n";
                    loadOk = false;
                    continue;
                }

                if (id > maxBookId) {
                    maxBookId = id;
                }
            }
        }
    }

    manager.fixInvalidBorrowers();

    manager.setNextBookId(maxBookId + 1);
    manager.setNextUserId(maxUserId + 1);

    return loadOk;
}

bool DataStorage::saveAll(const LibraryManager& manager,
                          const std::string& booksFile,
                          const std::string& usersFile) {
    //USERS
    {
    std::string tmpFile = usersFile + ".tmp";

    std::ofstream out(tmpFile, std::ios::trunc);
    if (!out) return false;

    for (const auto& u : manager.getUsers()) {
        if (containsForbiddenStorageChars(u.getFirstName()) ||
            containsForbiddenStorageChars(u.getLastName()) ||
            containsForbiddenStorageChars(u.getDepartment()) ||
            containsForbiddenStorageChars(u.getEmail())) {
            std::remove(tmpFile.c_str());
            return false;
        }
        out << u.getId() << ';'
            << u.getFirstName() << ';'
            << u.getLastName() << ';'
            << u.getDepartment() << ';'
            << u.getEmail()
            << "\n";
    }

    out.close();
    if (!out) return false;

    if (std::rename(tmpFile.c_str(), usersFile.c_str()) != 0) {
        std::remove(tmpFile.c_str());
        return false;
    }
    }

    //BOOKS
    {
    std::string tmpFile = booksFile + ".tmp";

    std::ofstream out(tmpFile, std::ios::trunc);
    if (!out) return false;

    for (const auto& b : manager.getBooks()) {
        if (containsForbiddenStorageChars(b.getTitle()) ||
            containsForbiddenStorageChars(b.getAuthor()) ||
            containsForbiddenStorageChars(b.getPublisher()) ||
            containsForbiddenStorageChars(b.getBorrowDate())) {
            std::remove(tmpFile.c_str());
            return false;
        }
        out << b.getId() << ';'
            << b.getTitle() << ';'
            << b.getAuthor() << ';'
            << b.getYear() << ';'
            << b.getPublisher() << ';'
            << b.getBorrowerId() << ';'
            << b.getBorrowDate()
            << "\n";
    }

    out.close();
    if (!out) return false;

    if (std::rename(tmpFile.c_str(), booksFile.c_str()) != 0) {
        std::remove(tmpFile.c_str());
        return false;
    }
}

    return true;
}