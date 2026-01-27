#pragma once
#include <string>



class Book {
public:

    Book(int id,
         std::string title, std::string author, int year, std::string publisher)
        : id(id), title(std::move(title)), author(std::move(author)), year(year), publisher(std::move(publisher)), borrowerId(-1) {}
    
    int getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    int getYear() const { return year; }
    const std::string& getPublisher() const { return publisher; }

    bool isBorrowed() const { return borrowerId != -1; }
    int getBorrowerId() const { return borrowerId; }
    
    void borrow(int userId) { borrowerId = userId; }
    void giveBack() { borrowerId = -1; }

private:

    const int id;
    const std::string title;
    const std::string author;
    const int year;
    const std::string publisher;
    
    int borrowerId; // -1 = free 
    
};

class User{
public:

    User(int id, std::string first_name, std::string last_name)
    : id(id), first_name(std::move(first_name)), last_name(std::move(last_name)) {}

    const std::string& getDepartment() const { return department;}
    void setDepartment(std::string name) {department = name;}

    const std::string& getEmail() const { return email;}
    void setEmail(std::string name) {email = name;}

    int getId() const { return id; }

private:

    const int id;
    const std::string first_name;
    const std::string last_name;
    std::string department;
    std::string email;
};