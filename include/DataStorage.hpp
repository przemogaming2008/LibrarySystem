#pragma once
#include <string>
#include "LibraryManager.hpp"

class DataStorage {
public:
    /*
    File formats:

    books.txt:
    id;title;author;year;publisher;borrowerId;borrowDate

    users.txt:
    id;firstName;lastName;department;email

    borrowerId = -1 => book available
    */

    bool loadAll(LibraryManager& manager,
                 const std::string& booksFile = "../../data/books.txt",
                 const std::string& usersFile = "../../data/users.txt");

    bool saveAll(const LibraryManager& manager,
                 const std::string& booksFile = "../../data/books.txt",
                 const std::string& usersFile = "../../data/users.txt");
};