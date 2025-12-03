#pragma once
#include "Person.h"
class LibraryWorker : public Person
{
    friend class Catalogue;
public:
    void add_member();
    bool verify_password() const;
    void new_loan();
    void show_loan();
    void end_loan();
    void extend_loan();
    void view_member_loans();
    bool is_admin() override;
    void book_search();
    void member_search();
    void change_member_info();
    void change_password();
    LibraryWorker(const std::string&,const std::string&,const std::string&);
protected:
    std::string password;
    bool is_valid_email(const std::string&);
};

