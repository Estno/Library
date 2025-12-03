#include "Book.h"

unsigned int Book::book_id_counter = 0;

Book::Book() :
	book_id(),
	author(""),
	title("")
{
}
Book::Book(const std::string& author, const std::string& title) :
	book_id(++book_id_counter),
	author(author),
	title(title)
{
}
Book::Book(const std::string& author, const std::string& title, const unsigned int book_id) :
	book_id(book_id),
	author(author),
	title(title)
{
}
