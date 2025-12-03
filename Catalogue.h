#pragma once
#include "Person.h"	
#include "SaveFileManager.h"
#include "LibraryAdmin.h"
#include "LibraryMember.h"
#include "Book.h"
#include "Loan.h"
#include <array>
#include <vector>
#include <string>
#include <unordered_map>
class Catalogue
{
private:
    static std::unordered_map<unsigned int, Book> id_to_book_map;
	static std::unordered_map<unsigned int, Loan> id_to_loan_map;
	static std::unordered_map<std::string, std::shared_ptr<Person>> email_to_person_map;
	static unsigned int RETURN_PERIOD;
	static unsigned int MAX_BOOKS_PER_MEMBER;

	static const std::vector<std::array<std::string, 3>> prep_admin_data();
	static const std::vector<std::array<std::string, 3>> prep_worker_data();
	static const std::vector<std::array<std::string, 2>> prep_member_data();
	static const std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> prep_loan_data();
	static const std::vector<std::tuple<std::string, std::string, unsigned int>> prep_book_data();

public:
	Catalogue();
	static void initialize_defaults(const SaveFileManager&);
	static void initialize_library_workers(const SaveFileManager&);
	static void initialize_library_members(const SaveFileManager&);
	static void initialize_loans(const SaveFileManager&);
	static void initialize_books(const SaveFileManager&);
	std::shared_ptr<LibraryWorker> login(const std::string&, const std::string&);

	static void save_to_file(const SaveFileManager&);

	static const int delete_book(const unsigned int);
	static const int delete_loan(const unsigned int);
	static const int delete_person(const std::string&, const unsigned int);

	static const std::string check_books_loaner(const unsigned int);
	static const int check_books_loan(const unsigned int);
	static const std::vector<std::string> email_search(std::string&);
	static const std::array<std::string,2> request_book_info(const unsigned int);
	static const std::pair<std::string, std::array<unsigned int,3>> request_loan_info(const unsigned int);
	static const std::vector<unsigned int> request_member_loans(const std::string&);
	static const std::string request_person_name_surname(const std::string&);
	static const std::vector<std::pair<unsigned int, std::array<std::string, 2>>> book_search(const std::string&, const unsigned int);

	static const int add_member(const std::string&, const std::string&);
	static const int add_worker(const std::string&, const std::string&, const std::string&);
    static const int add_admin(const std::string&, const std::string&, const std::string);
	static const int add_loan(const std::string&, const unsigned int);

    static void add_book(const std::string&, const std::string&);

	static const int add_book(const unsigned int, const unsigned int);

	static void change_max_books_per_member(const unsigned int);
	static void change_return_period(const unsigned int);
	static bool change_name_surname(const std::string&, const std::string&, const unsigned int);
	static bool extend_loan(const int, const unsigned int);
	static bool change_email(const std::string&, const std::string&, const unsigned int);
	static bool change_password(const std::string&, const std::string&);
};

