#include <cassert>
#include <iostream>
#include <string>
#include <cstdio> 
#include "LibraryManager.hpp"
#include "DataStorage.hpp"

void test_add_book() {
    LibraryManager m;

    int id1 = m.addBook("Hobbit","Tolkien",1937,"AU");
    int id2 = m.addBook("Hobbit2","Tolkien",1938,"AU");

    assert(id1 == 1);
    assert(id2 == 2);

    std::cout << "test_add_book OK\n";
}

void test_add_user() {
    LibraryManager m;

    int u1 = m.addUser("Jan","Kowalski","IT");
    int u2 = m.addUser("Anna","Nowak","HR");

    assert(u1 == 1);
    assert(u2 == 2);

    std::cout << "test_add_user OK\n";
}

void test_borrow_return() {
    LibraryManager m;

    int bookId = m.addBook("Hobbit","Tolkien",1937,"AU");
    int userId = m.addUser("Jan","Kowalski","IT");

   assert(m.borrowBook(bookId, userId) == BorrowResult::Ok);
    assert(m.borrowBook(bookId, userId) == BorrowResult::AlreadyBorrowed);

    assert(m.returnBook(bookId) == ReturnResult::Ok);
    assert(m.returnBook(bookId) == ReturnResult::NotBorrowed);

    std::cout << "test_borrow_return OK\n";
}
void test_status_after_borrow() {
    LibraryManager m;

    int bookId = m.addBook("Hobbit", "Tolkien", 1937, "AU");
    int userId = m.addUser("Jan", "Kowalski", "IT");

    assert(m.borrowBook(bookId, userId) == BorrowResult::Ok);

    auto stOpt = m.getBookStatus(bookId);
    assert(stOpt.has_value());

    const auto& st = *stOpt;
    assert(st.isBorrowed == true);
    assert(st.borrowerId == userId);
    assert(!st.borrowDate.empty()); 

    std::cout << "test_status_after_borrow OK\n";
}
void test_search_case_insensitive() {
    LibraryManager m;

    m.addBook("Clean Code", "Robert C. Martin", 2008, "PH");
    m.addBook("Hobbit", "Tolkien", 1937, "AU");
    m.addBook("Wiedźmin: Ostatnie życzenie", "Andrzej Sapkowski", 1993, "SN");

    auto r1 = m.findBooksByTitle("clean");
    auto r2 = m.findBooksByTitle("CLEAN");
    auto r3 = m.findBooksByTitle("Code");

    assert(r1.size() == 1);
    assert(r2.size() == 1);
    assert(r3.size() == 1);

    assert(r1[0].getTitle() == "Clean Code");

    std::cout << "test_search_case_insensitive OK\n";
}
static void removeIfExists(const std::string& path) {
    std::remove(path.c_str());
}
void test_persistence_save_load() {
    const std::string booksFile = "books_test.txt";
    const std::string usersFile = "users_test.txt";

    removeIfExists(booksFile);
    removeIfExists(usersFile);

    DataStorage storage;

    int savedBookId = -1;
    int savedUserId = -1;

    //zapis
    {
        LibraryManager m;

        savedBookId = m.addBook("Hobbit", "Tolkien", 1937, "AU");
        savedUserId = m.addUser("Jan", "Kowalski", "IT");

        assert(m.borrowBook(savedBookId, savedUserId) == BorrowResult::Ok);

        bool ok = storage.saveAll(m, booksFile, usersFile);
        assert(ok == true);
    }

    //wczytanie do nowego managera i porównanie
    {
        LibraryManager m2;

        bool ok = storage.loadAll(m2, booksFile, usersFile);
        assert(ok == true);

        auto stOpt = m2.getBookStatus(savedBookId);
        assert(stOpt.has_value());
        assert(stOpt->isBorrowed == true);
        assert(stOpt->borrowerId == savedUserId);
        assert(!stOpt->borrowDate.empty());

        //czy nextId został ustawiony poprawnie
        int newBookId = m2.addBook("Nowa", "Autor", 0, "X");
        assert(newBookId != savedBookId);
    }

    removeIfExists(booksFile);
    removeIfExists(usersFile);

    std::cout << "test_persistence_save_load OK\n";
}
void test_book_status() {
    LibraryManager m;

    int bookId = m.addBook("Hobbit", "Tolkien", 1937, "AU");
    int userId = m.addUser("Jan", "Kowalski", "IT");

    m.borrowBook(bookId, userId);

    auto st = m.getBookStatus(bookId);

    assert(st.has_value());
    assert(st->isBorrowed);
    assert(st->borrowerId == userId);
    assert(!st->borrowDate.empty());

    std::cout << "test_book_status OK\n";
}
void test_search_title() {
    LibraryManager m;

    m.addBook("Clean Code", "Martin", 2008, "PH");
    m.addBook("Hobbit", "Tolkien", 1937, "AU");

    auto result = m.findBooksByTitle("Clean");

    assert(result.size() == 1);
    assert(result[0].getTitle() == "Clean Code");

    std::cout << "test_search_title OK\n";
}
void test_search_author() {
    LibraryManager m;

    m.addBook("Clean Code", "Robert Martin", 2008, "PH");
    m.addBook("Hobbit", "Tolkien", 1937, "AU");

    auto result = m.findBooksByAuthor("Martin");

    assert(result.size() == 1);
    assert(result[0].getAuthor() == "Robert Martin");

    std::cout << "test_search_author OK\n";
}
void test_persistence() {
    LibraryManager m;
    DataStorage storage;

    int bookId = m.addBook("Hobbit", "Tolkien", 1937, "AU");
    int userId = m.addUser("Jan", "Kowalski", "IT");

    m.borrowBook(bookId, userId);

    storage.saveAll(m, "books_test.txt", "users_test.txt");

    LibraryManager m2;
    storage.loadAll(m2, "books_test.txt", "users_test.txt");

    auto st = m2.getBookStatus(bookId);

    assert(st.has_value());
    assert(st->isBorrowed);
    assert(st->borrowerId == userId);

    std::cout << "test_persistence OK\n";
}

int main() {
    test_add_book();
    test_add_user();
    test_borrow_return();

    test_status_after_borrow();
    test_search_case_insensitive();
    test_persistence_save_load();

    test_book_status();
    test_search_title();
    test_search_author();
    test_persistence();

    std::cout << "\nALL TESTS PASSED\n";
    return 0;
}
