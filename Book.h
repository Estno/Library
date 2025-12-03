#pragma once
#include <string>
class Book
{
	friend class Catalogue;
private:
	static unsigned int book_id_counter;
	const unsigned int book_id;
	const std::string author; 
	const std::string title;
public:
	Book();
	Book(const std::string&, const std::string&);
	Book(const std::string&, const std::string&, const unsigned int);
};

