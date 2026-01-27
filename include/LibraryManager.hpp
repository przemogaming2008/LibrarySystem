#pragma once

#include "Models.hpp"

#include <vector>
#include <string>

/*Zaimplementuj też LibraryManager z metodami dodawania książki i użytkownika oraz listowania 
(na razie mogą być "puste" lub proste). - Kompiluj lokalnie, upewnij się że projekt się buduje (na tym etapie main()
 może być tymczasowy – np. testujesz dodawanie kilku książek do vectora i print). */

class LibraryManager{
public:
    bool addBook(const Book& book);
    bool addUser(const User& user);

    bool borrowBook(int bookId, int userId);
    bool returnBook(int bookId);

    bool getBookStatus(int bookId) const;
    std::vector<Book> findBooksByTitle(const std::string& fragment) const;


private:

    std::vector<Book> books;
    std::vector<User> users;

};