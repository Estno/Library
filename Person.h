#pragma once
#include <string>
class Person
{
	friend class Catalogue;
public:
	Person(const std::string&, const std::string&);
protected:
	std::string name_surname;
	std::string email;
	virtual bool is_admin() = 0;
};

