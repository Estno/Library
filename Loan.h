#pragma once
#include "date.h"
#include <string>
class Loan
{
	friend class Catalogue;
private:
	static unsigned int loan_id_counter;
	const unsigned int loan_id;
	const unsigned int book_id;
	std::string member_email;
	const unsigned int start_date;
	unsigned int return_date;
public:
	Loan();
	Loan(const unsigned int, const unsigned int, const unsigned int, const std::string&);
	Loan(const unsigned int, const unsigned int, const unsigned int, const unsigned int, const std::string&);
};

