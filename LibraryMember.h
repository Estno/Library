#pragma once
#include "Person.h"
class LibraryMember : public Person
{
    friend class Catalogue;
public:
	using Person::Person;
private:
    bool is_admin() override;
};

