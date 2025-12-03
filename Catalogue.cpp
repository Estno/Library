#include "Catalogue.h"
#include <tuple>
#include "user_input.h"

unsigned int Catalogue::RETURN_PERIOD = 0;
unsigned int Catalogue::MAX_BOOKS_PER_MEMBER = 0;
std::unordered_map<unsigned int, Book> Catalogue::id_to_book_map;
std::unordered_map<unsigned int, Loan> Catalogue::id_to_loan_map;
std::unordered_map<std::string, std::shared_ptr<Person>> Catalogue::email_to_person_map;

Catalogue::Catalogue() 
{
}
void Catalogue::initialize_defaults(const SaveFileManager& sfm)
{
	RETURN_PERIOD = sfm.fetch_return_period();
	MAX_BOOKS_PER_MEMBER = sfm.fetch_max_books_per_member();
}
void Catalogue::initialize_library_workers(const SaveFileManager& sfm)
{
	const auto& worker_data = sfm.fetch_worker_data();
	for(auto& worker : worker_data)
	{
        std::shared_ptr<LibraryWorker> p_LibraryWorker = std::make_shared<LibraryWorker>(worker[0], worker[1], worker[2]);
        email_to_person_map.try_emplace(worker[0], std::move(p_LibraryWorker));
	}

	const auto& admin_data = sfm.fetch_admin_data();
	for(auto& admin : admin_data)
	{
        std::shared_ptr<LibraryAdmin> p_LibraryAdmin = std::make_shared<LibraryAdmin>(admin[0], admin[1], admin[2]);
        email_to_person_map.try_emplace(admin[0], std::move(p_LibraryAdmin));
	}
}
void Catalogue::initialize_library_members(const SaveFileManager& sfm)
{
	const auto& member_data = sfm.fetch_member_data();
	for(auto& member : member_data)
	{
		std::shared_ptr<LibraryMember> p_LibraryMember = std::make_shared<LibraryMember>(member[0], member[1]);
		email_to_person_map.try_emplace(member[0], std::move(p_LibraryMember));
	}
}
void Catalogue::initialize_loans(const SaveFileManager& sfm)
{
	const auto& loan_data = sfm.fetch_loan_data();
	for(auto& loan : loan_data)
	{
		Loan l(std::get<0>(loan), std::get<1>(loan), std::get<2>(loan), std::get<3>(loan), std::get<4>(loan));
		id_to_loan_map.insert({std::get<0>(loan), l});
	}

	Loan::loan_id_counter = sfm.fetch_loan_counter();
}
void Catalogue::initialize_books(const SaveFileManager& sfm)
{
	const auto& book_data = sfm.fetch_book_data();
	for(auto& book : book_data)
	{
		Book b(std::get<0>(book), std::get<1>(book), std::get<2>(book));
		id_to_book_map.insert({std::get<2>(book), b});
	}

	Book::book_id_counter = sfm.fetch_book_counter();
}
std::shared_ptr<LibraryWorker> Catalogue::login(const std::string& email, const std::string& password) {
	if(!email_to_person_map.contains(email)) {
        return std::shared_ptr<LibraryWorker>(nullptr);
    }
	if(std::shared_ptr<LibraryWorker> p_LibraryWorker = std::dynamic_pointer_cast<LibraryWorker>(email_to_person_map[email])) {
        if(p_LibraryWorker->password != password) {
			return std::shared_ptr<LibraryWorker>(nullptr);
        }
        return p_LibraryWorker;
    }
    return std::shared_ptr<LibraryWorker>(nullptr);
}

void Catalogue::save_to_file(const SaveFileManager& sfm) {
	auto get_loan_id_counter = [&]() -> unsigned int {
		if (id_to_loan_map.empty()) return 0;
		return id_to_loan_map.at(1).loan_id_counter;
		};
	auto get_book_id_counter = [&]() -> unsigned int {
		if(id_to_book_map.empty()) return 0;
		return id_to_book_map.at(1).book_id_counter;
        };
    sfm.new_data_workers_file(RETURN_PERIOD, MAX_BOOKS_PER_MEMBER, prep_worker_data(), prep_admin_data());
    sfm.new_data_library_file(get_loan_id_counter(), prep_loan_data(), get_book_id_counter(), prep_book_data(), prep_member_data());

}
const std::vector<std::array<std::string, 3>> Catalogue::prep_admin_data() {
    std::vector<std::array<std::string, 3>> admin_data;

	for(auto& pair : email_to_person_map) {
		if(std::shared_ptr<LibraryAdmin> p_LibraryAdmin = std::dynamic_pointer_cast<LibraryAdmin>(pair.second)) {
            admin_data.push_back({p_LibraryAdmin->email, p_LibraryAdmin->name_surname, p_LibraryAdmin->password});
        }
	}
    return admin_data;
}
const std::vector<std::array<std::string, 3>> Catalogue::prep_worker_data() {
    std::vector<std::array<std::string, 3>> worker_data;

	for(auto& [email, person] : email_to_person_map) {
		if(std::shared_ptr p_LibraryWorker = std::dynamic_pointer_cast<LibraryWorker>(person)) {
			if(!p_LibraryWorker->is_admin())
            worker_data.push_back({ p_LibraryWorker->email, p_LibraryWorker->name_surname, p_LibraryWorker->password });
        }
    }
    return worker_data;
}
const std::vector<std::array<std::string, 2>> Catalogue::prep_member_data() {
    std::vector<std::array<std::string, 2>> member_data;

	for(auto& [email, person] : email_to_person_map) {
		if(std::shared_ptr<LibraryMember> p_LibraryMember = std::dynamic_pointer_cast<LibraryMember>(person)) {
			member_data.push_back({ p_LibraryMember->email, p_LibraryMember->name_surname });
        }
    }
    return member_data;
}
const std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> Catalogue::prep_loan_data() {
    std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> loan_data;

	for(auto& [id, loan] : id_to_loan_map) {
        loan_data.push_back({loan.loan_id, loan.book_id, loan.start_date, loan.return_date, loan.member_email});
    }
    return loan_data;
}
const std::vector<std::tuple<std::string, std::string, unsigned int>> Catalogue::prep_book_data() {
    std::vector<std::tuple<std::string, std::string, unsigned int>> book_data;

	for(auto& [id, book] : id_to_book_map) {
        book_data.push_back({book.author, book.title, book.book_id});
    }
    return book_data;
}

const int Catalogue::delete_book(const unsigned int book_id)
{
	const enum DeleteStatus
	{
		NOT_FOUND = -1,
		LOANED = -2,
		FAILED = -3,
		SUCCESS = 0
	};

	auto is_loaned = [&book_id]() -> bool
		{
			for(const auto& [id, loan] : id_to_loan_map)
			{
				if(loan.book_id == book_id)
				{
					return true;
				}
			}
			return false;
		};

	if(!id_to_book_map.contains(book_id))
	{
		return NOT_FOUND;
	}
	// If the book is loaned, we can't delete it
	if(is_loaned()) return LOANED;

	if(id_to_book_map.erase(book_id) == 0)
	{
		return FAILED;
	}
	return SUCCESS;
}
const int Catalogue::delete_person(const std::string& email, const unsigned int person_type)
{
	const enum PersonType {
		MEMBER = 0,
		WORKER = 1,
		ADMIN = 2
	};

	const enum DeleteStatus {
        NOT_FOUND = -1,
        HAS_LOANS = -2,
        FAILED = -3,
		NOT_A_MEMBER = -4,
		NOT_A_WORKER = -5,
		NOT_AN_ADMIN = -6,
        SUCCESS = 0
    };

	auto has_loans = [&email]() -> bool
		{
			for(const auto& [id, loan] : id_to_loan_map)
			{
				if(loan.member_email == email)
				{
					return true;
				}
			}
			return false;
		};

	if(!email_to_person_map.contains(email))
	{
		return NOT_FOUND;
	}
	if(!std::dynamic_pointer_cast<LibraryMember>(email_to_person_map[email]))
	{
		if(person_type == MEMBER) return NOT_A_MEMBER;
		if(!std::dynamic_pointer_cast<LibraryAdmin>(email_to_person_map[email])) {
			if(person_type == ADMIN) return NOT_AN_ADMIN;
        }
		else if (person_type == WORKER) return NOT_A_WORKER;
	}
	else if(person_type != MEMBER) return NOT_A_WORKER;
    // If the member has active loans, we can't delete them
	// Workers can't have loans so we can skip the check
	else if(has_loans()) {
        return HAS_LOANS;
    }

	if(email_to_person_map.erase(email) == 0) {
		return FAILED;
	}
	return SUCCESS;
}
const int Catalogue::delete_loan(const unsigned int loan_id)
{
	const enum DeleteStatus {
		NOT_FOUND = -1,
		FAILED = -3,
		SUCCESS = 0
	};

	if(!id_to_loan_map.contains(loan_id))
	{
		return NOT_FOUND;
	}

	if(id_to_loan_map.erase(loan_id) == 0)
	{
		return FAILED;
	}
	return SUCCESS;
}

const std::array<std::string, 2> Catalogue::request_book_info(const unsigned int book_id) {
	if(!id_to_book_map.contains(book_id)) {
		return {"", ""};
	}

    const std::array<std::string, 2> info = {id_to_book_map[book_id].author, id_to_book_map[book_id].title};
    return info;
}
const std::pair<std::string, std::array<unsigned int, 3>> Catalogue::request_loan_info(const unsigned int loan_id) {
	if(!id_to_loan_map.contains(loan_id)) {
		return {"", {0, 0, 0}};
	}
    const std::pair<std::string, std::array<unsigned int, 3>> info = {id_to_loan_map[loan_id].member_email,
																						{id_to_loan_map[loan_id].book_id, id_to_loan_map[loan_id].start_date, id_to_loan_map[loan_id].return_date}};
    return info;
}
const std::vector<unsigned int> Catalogue::request_member_loans(const std::string& member_email) {
    std::vector<unsigned int> loans;

	for(const auto& [id, loan] : id_to_loan_map) {
		if(loan.member_email == member_email) {
            loans.push_back(id);
        }
    }
    return loans;
}
const std::string Catalogue::request_person_name_surname(const std::string& email) {
	if(email_to_person_map.contains(email)) {
		return email_to_person_map[email]->name_surname;
	}
	else return "";
}
const int Catalogue::check_books_loan(const unsigned int book_id) {
	for(const auto& [id, loan] : id_to_loan_map) {
		if(loan.book_id == book_id) {
			return id;
		}
	}
	return -1;
}
const std::string Catalogue::check_books_loaner(const unsigned int book_id) {
	for(const auto& [id, loan] : id_to_loan_map) {
		if(loan.book_id == book_id) {
			return loan.member_email;
		}
	}
	return "";
}
const std::vector<std::string> Catalogue::email_search(std::string& search_term) 	{
	email_to_lowercase(search_term);
	std::vector<std::string> results;

	for(const auto& [email, person] : email_to_person_map) {
		if(email.find(search_term) != std::string::npos) {
			results.push_back(email);
		}
	}
	return results;
}
const std::vector<std::pair<unsigned int, std::array<std::string, 2>>> Catalogue::book_search(const std::string& search_term, const unsigned int search_type) {
    const enum SearchType {
        AUTHOR = 0,
        TITLE = 1
    };

    std::vector<std::pair<unsigned int, std::array<std::string, 2>>> results;

    for(const auto& [id, book] : id_to_book_map) {
        if(search_type == AUTHOR && book.author.find(search_term) != std::string::npos) {
            results.push_back({ id, {book.author, book.title} });
        }
        else if(search_type == TITLE && book.title.find(search_term) != std::string::npos) {
            results.push_back({ id, {book.author, book.title} });
        }
    }
    return results;
}

const int Catalogue::add_member(const std::string& email, const std::string& name_surname) {
	const enum AddStatus {
		EXISTS = -1,
        SUCCESS = 0
    };

	if(email_to_person_map.contains(email)) {
        return EXISTS;
    }

    std::shared_ptr<LibraryMember> p_LibraryMember = std::make_shared<LibraryMember>(email, name_surname);
    email_to_person_map.try_emplace(email, std::move(p_LibraryMember));
return SUCCESS;
}
const int Catalogue::add_worker(const std::string& email, const std::string& name_surname, const std::string& password) {
	const enum AddStatus {
        EXISTS = -1,
        SUCCESS = 0
    };

	if(email_to_person_map.contains(email)) {
        return EXISTS;
    }

    std::shared_ptr<LibraryWorker> p_LibraryWorker = std::make_shared<LibraryWorker>(email, name_surname, password);
    email_to_person_map.try_emplace(email, std::move(p_LibraryWorker));
    return SUCCESS;
}
const int Catalogue::add_admin(const std::string& email, const std::string& name_surname, const std::string password) {
	const enum AddStatus {
        EXISTS = -1,
        SUCCESS = 0
    };

	if(email_to_person_map.contains(email)) {
        return EXISTS;
    }

    std::shared_ptr<LibraryAdmin> p_LibraryAdmin = std::make_shared<LibraryAdmin>(email, name_surname, password);
    email_to_person_map.try_emplace(email, std::move(p_LibraryAdmin));
    return SUCCESS;	
}
const int Catalogue::add_loan(const std::string& email, const unsigned int book_id) {
	unsigned int start_date = today();
	unsigned int return_date = start_date + RETURN_PERIOD;

	const enum AddStatus {
        NO_SUCH_MEMBER = -1,
        NO_SUCH_BOOK = -2,
        BOOK_ALREADY_LOANED = -3,
        MAX_BOOKS_REACHED = -4,
        FAILED = -5,
        SUCCESS = 0
    };

	if(!email_to_person_map.contains(email) || !std::dynamic_pointer_cast<LibraryMember>(email_to_person_map[email])) {
        return NO_SUCH_MEMBER;
    }
	if(!id_to_book_map.contains(book_id)) {
        return NO_SUCH_BOOK;
    }
	if(check_books_loan(book_id) != -1) {
        return BOOK_ALREADY_LOANED;
    }
	if(request_member_loans(email).size() >= MAX_BOOKS_PER_MEMBER) {
        return MAX_BOOKS_REACHED;
    }

	Loan l(book_id, start_date, return_date, email);
	if(!id_to_loan_map.insert({l.loan_id, l}).second) {
        return FAILED;
    }
    return SUCCESS;
}
void Catalogue::add_book(const std::string& author, const std::string& title) {
	Book b(author, title);
	id_to_book_map.insert({ b.book_id, b });
}
const int Catalogue::add_book(const unsigned int book_id, const unsigned int number) {
	const enum AddStatus {
		SUCCESS = 0,
		DOESNT_EXIST = -1
	};
	if(!id_to_book_map.contains(book_id)) return DOESNT_EXIST;
	Book* b = &id_to_book_map[book_id];
	for(unsigned int i = 0; i < number; ++i) {
		Book book (b->author, b->title);
        id_to_book_map.insert({book.book_id, book});
	}
    return SUCCESS;
}

void Catalogue::change_max_books_per_member(const unsigned int new_max) {
	MAX_BOOKS_PER_MEMBER = new_max;
}
void Catalogue::change_return_period(const unsigned int new_return_period) {
	RETURN_PERIOD = new_return_period;
}
bool Catalogue::change_name_surname(const std::string& email, const std::string& new_name_surname, const unsigned int person_type) {
	const enum PersonType {
        MEMBER = 0,
        WORKER = 1,
        ADMIN = 2
    };
	if(email_to_person_map.contains(email)) {
		if(person_type == MEMBER && std::dynamic_pointer_cast<LibraryMember>(email_to_person_map[email])) {
            email_to_person_map[email]->name_surname = new_name_surname;
            return true;
        }
		if(person_type == WORKER && std::dynamic_pointer_cast<LibraryWorker>(email_to_person_map[email])) {
            email_to_person_map[email]->name_surname = new_name_surname;
            return true;
        }
		if(person_type == ADMIN && std::dynamic_pointer_cast<LibraryAdmin>(email_to_person_map[email])) {
            email_to_person_map[email]->name_surname = new_name_surname;
            return true;
		}
	}
	return false;
}
bool Catalogue::extend_loan(const int loan_id, const unsigned int days) {
	if(id_to_loan_map.contains(loan_id)) {
		id_to_loan_map[loan_id].return_date += days;
		return true;
	}
	return false;
}
bool Catalogue::change_email(const std::string& old_email, const std::string& new_email, const unsigned int person_type) {
	const enum PersonType {
        MEMBER = 0,
        WORKER = 1,
        ADMIN = 2
    };
	if(email_to_person_map.contains(new_email) || !email_to_person_map.contains(old_email)) {
        return false;
    }
	if(person_type == MEMBER && std::dynamic_pointer_cast<LibraryMember>(email_to_person_map[old_email])) {
        email_to_person_map[new_email] = email_to_person_map[old_email];
        email_to_person_map.erase(old_email);
        const std::vector<unsigned int> member_loans = request_member_loans(old_email);
        for(auto& loan_id : member_loans) {
            id_to_loan_map[loan_id].member_email = new_email;
    }
        return true;
    }
	if(person_type == WORKER && std::dynamic_pointer_cast<LibraryWorker>(email_to_person_map[old_email])) {
        email_to_person_map[new_email] = email_to_person_map[old_email];
        email_to_person_map.erase(old_email);
        return true;
    }
	if(person_type == ADMIN && std::dynamic_pointer_cast<LibraryAdmin>(email_to_person_map[old_email])) {
        email_to_person_map[new_email] = email_to_person_map[old_email];
        email_to_person_map.erase(old_email);
        return true;
    }
	return false;
}
bool Catalogue::change_password(const std::string& email, const std::string& new_password) {
	if(email_to_person_map.contains(email)) {
		std::shared_ptr<LibraryWorker> p_LibraryWorker = std::dynamic_pointer_cast<LibraryWorker>(email_to_person_map[email]);
		p_LibraryWorker->password = new_password;
        return true;
    }
    return false;
}

