#include "Loan.h"

unsigned int Loan::loan_id_counter = 0;
Loan::Loan() :
	loan_id(),
	book_id(),
	start_date(),
	return_date(),
	member_email() 
{
}
Loan::Loan(const unsigned int book_id, const unsigned int start_date, const unsigned int return_date, const std::string& member_email) :
	loan_id(++loan_id_counter),
	book_id(book_id),
	start_date(start_date),
	return_date(return_date),
	member_email(member_email)
{
}
Loan::Loan(const unsigned int loan_id, const unsigned int book_id, const unsigned int start_date, const unsigned int return_date, const std::string& member_email) :
	loan_id(loan_id),
	book_id(book_id),
	start_date(start_date),
	return_date(return_date),
	member_email(member_email)
{
}
