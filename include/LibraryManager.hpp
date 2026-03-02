#pragma once

#include "Models.hpp"

#include <vector>
#include <string>
#include <optional>

/*Zaimplementuj też LibraryManager z metodami dodawania książki i użytkownika oraz listowania 
(na razie mogą być "puste" lub proste). - Kompiluj lokalnie, upewnij się że projekt się buduje (na tym etapie main()
 może być tymczasowy – np. testujesz dodawanie kilku książek do vectora i print). */

struct BookStatus {
    bool isBorrowed = false;
    int borrowerId = -1;
    std::string borrowDate;
};
enum class BorrowResult {
    Ok,
    UserNotFound,
    BookNotFound,
    AlreadyBorrowed
};

enum class ReturnResult {
    Ok,
    BookNotFound,
    NotBorrowed
};
class LibraryManager{
public:
    int addBook(std::string title, std::string author, int year, std::string publisher);
    int addUser(std::string first, std::string last, std::string department);

    BorrowResult borrowBook(int bookId, int userId);
    ReturnResult returnBook(int bookId);

    std::optional<BookStatus> getBookStatus(int bookId) const;
    std::vector<Book> findBooksByTitle(const std::string& fragment) const;

    void listBooks() const;
    void listUsers() const;
    
    const std::vector<Book>& getBooks() const { return books; }
    const std::vector<User>& getUsers() const { return users; }

    bool addBookFromStorage(const Book& book);
    bool addUserFromStorage(const User& user);

    void setNextBookId(int v) { nextBookId = v; }
    void setNextUserId(int v) { nextUserId = v; }
    
    const User* findUserById(int userId) const;

private:

    std::vector<Book> books;
    std::vector<User> users;

    int nextBookId = 1;
    int nextUserId = 1;
};