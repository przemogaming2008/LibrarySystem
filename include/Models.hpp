#include <string>



class Book {
public:

    Book(int id,
         std::string title, std::string author, int year, std::string publisher)
        : id(id), title(std::move(title)), author(std::move(author)), year(year), publisher(std::move(publisher)) {}
    
    bool getIsBorrowed() const { return isBorrowed;}
    void setIsBorrowed(bool value) {isBorrowed = value;}

private:

    const int id;
    const std::string title;
    const std::string author;
    const int year;
    const std::string publisher;
    bool isBorrowed = false;
    
};

class User{
public:

    User(int id, std::string first_name, std::string last_name)
    : id(id), first_name(std::move(first_name)), last_name(std::move(last_name)) {}

    std::string getDepartment() const { return department;}
    void setDepartment(std::string name) {department = name;}

    std::string getEmail() const { return email;}
    void setemail(std::string name) {email = name;}

private:

    const int id;
    const std::string first_name;
    const std::string last_name;
    std::string department;
    std::string email;
};