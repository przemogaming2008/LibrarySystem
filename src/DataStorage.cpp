#include "DataStorage.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>

static std::vector<std::string> splitToVector(const std::string& line) {
    std::vector<std::string> out;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, ';')) out.push_back(token);
    return out;
}
bool DataStorage::loadAll(LibraryManager& manager,
                          const std::string& booksFile,
                          const std::string& usersFile) {
    int maxBookId = 0;
    int maxUserId = 0;

    //USERS
    {
        std::ifstream in(usersFile);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.empty()) continue;

                auto f = splitToVector(line);
                // id;firstName;lastName;department;email
                if (f.size() < 3) continue;

                int id = std::stoi(f[0]);
                std::string first = f.size() > 1 ? f[1] : "";
                std::string last  = f.size() > 2 ? f[2] : "";
                std::string dept  = f.size() > 3 ? f[3] : "";
                std::string email = f.size() > 4 ? f[4] : "";

                User u{id, first, last};
                if (!dept.empty())  u.setDepartment(dept);
                if (!email.empty()) u.setEmail(email);

                manager.addUserFromStorage(u);
                if (id > maxUserId) maxUserId = id;
            }
        }
        //
    }

    //BOOKS
    {
        std::ifstream in(booksFile);
        if (in) {
            std::string line;
            while (std::getline(in, line)) {
                if (line.empty()) continue;

                auto f = splitToVector(line);
                // id;title;author;year;publisher;borrowerId;borrowDate
                if (f.size() < 5) continue;

                int id = std::stoi(f[0]);
                std::string title     = f.size() > 1 ? f[1] : "";
                std::string author    = f.size() > 2 ? f[2] : "";
                int year              = (f.size() > 3 && !f[3].empty()) ? std::stoi(f[3]) : 0;
                std::string publisher = f.size() > 4 ? f[4] : "";

                int borrowerId = (f.size() > 5 && !f[5].empty()) ? std::stoi(f[5]) : -1;
                std::string borrowDate = f.size() > 6 ? f[6] : "";

                Book b{id, title, author, year, publisher};

                if (borrowerId != -1) {
                    b.borrow(borrowerId);
                    b.setBorrowDate(borrowDate);
                }

                manager.addBookFromStorage(b);
                if (id > maxBookId) maxBookId = id;
            }
        }
    }

    manager.fixInvalidBorrowers();

    manager.setNextBookId(maxBookId + 1);
    manager.setNextUserId(maxUserId + 1);
    return true;
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
        out << u.getId() << ';'
            << u.getFirstName() << ';'
            << u.getLastName() << ';'
            << u.getDepartment() << ';'
            << u.getEmail()
            << "\n";
    }

    out.close();
    if (!out) return false;

    std::remove(usersFile.c_str());
    if (std::rename(tmpFile.c_str(), usersFile.c_str()) != 0)
        return false;
}

    //BOOKS
    {
    std::string tmpFile = booksFile + ".tmp";

    std::ofstream out(tmpFile, std::ios::trunc);
    if (!out) return false;

    for (const auto& b : manager.getBooks()) {
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

    std::remove(booksFile.c_str());
    if (std::rename(tmpFile.c_str(), booksFile.c_str()) != 0)
        return false;
    }

    return true;
}