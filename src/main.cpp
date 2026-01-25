#include "DataStorage.hpp"
#include "LibraryManager.hpp"
#include "MenuUI.hpp"
#include "Models.hpp"
#include <iostream>

int main(){
    Book book{1, "Hobbit", "Tolkien", 1937, "Allen & Unwin"};
    User user{1,"Jan","Kowalski"};
    std::cout << "is book borrowed: "<< book.getIsBorrowed() << std::endl;
    return 1;

}