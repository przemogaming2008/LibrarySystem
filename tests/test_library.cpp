#include <cassert>
#include <iostream>
#include "LibraryManager.hpp"

void test_add_book() {
    LibraryManager m;

    Book b1{1,"Hobbit","Tolkien",1937,"AU"};
    Book b2{1,"Hobbit2","Tolkien",1938,"AU"};

    assert(m.addBook(b1) == true);
    assert(m.addBook(b2) == false);

    std::cout << "test_add_book OK\n";
}
void test_add_user() {
    LibraryManager m;

    User u1{1,"Jan","Kowalski"};
    User u2{1,"Anna","Nowak"};

    assert(m.addUser(u1) == true);
    assert(m.addUser(u2) == false);

    std::cout << "test_add_user OK\n";
}
void test_borrow_return() {
    LibraryManager m;

    Book b{1,"Hobbit","Tolkien",1937,"AU"};
    User u{1,"Jan","Kowalski"};

    m.addBook(b);
    m.addUser(u);

    assert(m.borrowBook(1,1) == true);
    assert(m.borrowBook(1,1) == false);//już wypożyczona

    assert(m.returnBook(1) == true);
    assert(m.returnBook(1) == false);//już oddana

    std::cout << "test_borrow_return OK\n";
}

int main() {
    test_add_book();
    test_add_user();
    test_borrow_return();

    std::cout << "\nALL TESTS PASSED\n";
}