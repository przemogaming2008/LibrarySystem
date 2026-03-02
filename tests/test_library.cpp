#include <cassert>
#include <iostream>
#include "LibraryManager.hpp"

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

int main() {
    test_add_book();
    test_add_user();
    test_borrow_return();

    std::cout << "\nALL TESTS PASSED\n";
    return 0;
}