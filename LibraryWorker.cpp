#include "LibraryWorker.h"
#include <iostream>
#include <regex>
#include "Catalogue.h"
#include "user_input.h"
#include <map>

LibraryWorker::LibraryWorker(const std::string& email, const std::string& name_surname, const std::string& password):
	Person(email, name_surname),
	password(password)
{
}

void LibraryWorker::add_member() {
    std::string email;
    std::string name_surname;
    auto extract_name_surname = [](const std::string& name_surname) -> std::string {
        auto is_alpha_or_space = [](char c) -> bool {
            return std::isalpha(c) || std::isspace(c);
        };
        auto first_space = name_surname.find(' ');
        std::string extracted_name_surname;
        if(first_space == std::string::npos) {
            return "";
        }
        auto second_space = name_surname.find(' ', first_space + 1);
        auto newline = name_surname.find('\n');

        if(second_space == std::string::npos) second_space = name_surname.size();
        if(newline == std::string::npos) newline = name_surname.size();
        auto end_index = std::min(second_space, newline);
        extracted_name_surname = name_surname.substr(0, end_index);
        if(!std::all_of(extracted_name_surname.begin(), extracted_name_surname.end(), is_alpha_or_space)) {
            return "";
        }
        return extracted_name_surname;
        };
	const enum AddStatus {
		EXISTS = -1,
        SUCCESS = 0
    };

    for(int i = 0; i < 2; ++i) {
        std::cout << "Enter the email of the new member:\n";
        std::cout << "(email)->" << std::flush;
        email = read_string();

        std::cout << "\nConfirm email:\n";
        std::cout << "(email)->" << std::flush;
        std::string confirm_email = read_string();

        if(email == confirm_email) {
            break;
        }
        email = "";
        std::cerr << "\nEmails do not match. Please try again." << std::endl;
    }
    if(!is_valid_email(email)) {
        std::cerr << "\nInvalid email. Please try again." << std::endl;
        return;
    }
	email_to_lowercase(email);

    std::cout << "\nEnter the name and surname of the new member (it should be two words separated by a space):\n";
    std::cout << "(name surname)->" << std::flush;
    name_surname = read_string();
    name_surname = extract_name_surname(name_surname);
    if(name_surname == "") {
        std::cerr << "\nInvalid name and surname. Please try again." << std::endl;
        return;
    }

    std::cout << "\nNew member with email: " << email << " and name: " << name_surname << ". Proceed? (y/n): " << std::flush;
    if(!user_confirms()) return;
    switch(Catalogue::add_member(email, name_surname)) {
        case EXISTS:
            std::cout << "\nA member with the given email already exists." << std::endl;
            break;
        case SUCCESS:
            std::cout << "\nMember added successfully." << std::endl;
            break;
    }
}

bool LibraryWorker::verify_password() const
{
    std::string input_password;
    std::cout << "\nEnter your password:\n";
    std::cout << "(password)->" << std::flush;
    input_password = read_string();
    std::cout << '\n';
    return input_password == password;
}

void LibraryWorker::new_loan()
{
    std::string email;
    int book_id;
	const enum AddStatus {
        NO_SUCH_MEMBER = -1,
        NO_SUCH_BOOK = -2,
        BOOK_ALREADY_LOANED = -3,
        MAX_BOOKS_REACHED = -4,
        FAILED = -5,
        SUCCESS = 0
    };

    std::cout << "Enter the email of the member:\n";
    std::cout << "(email)->" << std::flush;
    email = read_string();
    email_to_lowercase(email);

    std::cout << "\nEnter the ID of the book:\n";
    std::cout << "(ID)->" << std::flush;
    book_id = read_number(5);
    if(book_id == -1) {
        std::cerr << "\nNot a number. Please try again." << std::endl;
        return;
    }

    std::cout << "\nNew loan for user: " << email << " and book ID: " << std::to_string(book_id) << ". Proceed? (y/n): " << std::flush;
    if(!user_confirms()) return;

    switch(Catalogue::add_loan(email, book_id)) {

        case NO_SUCH_MEMBER: {
            std::cout << "\nNo member found with given email. Please try again." << std::endl;
            break;
        }
        case NO_SUCH_BOOK: {
            std::cout << "\nNo book found with given ID. Please try again." << std::endl;
            break;
        }
        case BOOK_ALREADY_LOANED: {
            unsigned int loan_id = Catalogue::check_books_loan(book_id);
            std::cout << "\nThe book is already loaned. Loan ID: " << std::to_string(loan_id) << "." << std::endl;
            break;
        }
        case MAX_BOOKS_REACHED: {
            std::cout << "\nThe member has reached the maximum number of books they can loan." << std::endl;
            break;
        }
        case FAILED: {
            std::cout << "\nAn error occurred while attempting to add the loan. Please try again." << std::endl;
            break;
        }
        case SUCCESS: {
            std::cout << "\nLoan added successfully." << std::endl;
            break;
        }
    }
}

void LibraryWorker::show_loan()
{
    const enum AddBook {
        BOOK = 1,
        LOAN = 2
    };
    std::cout << "Would you rather check loan by: \n";
    std::cout << "1.Book  id\n";
    std::cout << "2.Loan  id\n";
    std::cout << "(1/2) -> " << std::flush;
    switch (read_number(1)) {
    case BOOK: {
        int book_id;
        std::cout << "\nEnter book id: \n";
        std::cout << "(book id) -> " << std::flush;
        book_id = read_number(6);
        if (Catalogue::check_books_loan(book_id) == -1) {
            std::cout << "No active for this book.\n";
            break;
        }
        const std::array<std::string, 2> book_info = Catalogue::request_book_info(book_id);
     
        const std::pair<std::string, std::array<unsigned int, 3>>loan_info = Catalogue::request_loan_info(Catalogue::check_books_loan(book_id));
        std::cout << "Loan is active on book: \"" << book_info[1] << "\" written by: " << book_info[0] << "\n";
        std::cout << "book is loaned by: " << loan_info.first << " was loaned on: " << dateToString(loan_info.second[1]) << ", loan ends on: " << dateToString(loan_info.second[2]) << "\n";
        break;
    }
    case LOAN: {
        int loan_id;
        std::cout << "Enter loan id: \n";
        std::cout << "(loan id) -> " << std::flush;
        loan_id = read_number(8);
        const std::pair<std::string, std::array<unsigned int, 3>>loan_info = Catalogue::request_loan_info(loan_id);
        if (loan_info.first == "") {
            std::cout << "\nLoan is not active\n";
            break;
        }
        const std::array<std::string, 2> book_info = Catalogue::request_book_info(loan_info.second[0]);
        std::cout << "\nLoan is active on book: \"" << book_info[1] << "\" written by: " << book_info[0] << "\n";
        std::cout << "book is loaned by: " << loan_info.first << " was loaned on: " << dateToString(loan_info.second[1]) << ", loan ends on: " << dateToString(loan_info.second[2]) << "\n";
        break;
    }
    default: {
        std::cout << "Wrong input";
        break;
    }
    }

}

void LibraryWorker::end_loan() {
	const enum SearchType {
		LOAN_ID = 1,
		BOOK_ID = 2,
	};
	const enum DeleteStatus {
		NOT_FOUND = -1,
		FAILED = -3,
		SUCCESS = 0
	};

    auto db_request_loan_delete = [](const unsigned int loan_id) -> void {
        switch(Catalogue::delete_loan(loan_id)) {
            case NOT_FOUND:
                std::cout << "\nNo loan found with given ID." << std::endl;
                break;
            case FAILED:
                std::cout << "\nThe loan was found but there was an error while attempting to delete. Please try again." << std::endl;
                break;
            case SUCCESS:
                std::cout << "\nLoan ended successfully." << std::endl;
                break;
        }
        };

	std::cout << "How do you want to search for the loan? (1: By loan ID, 2: By book ID):\n";
	std::cout << "(number)->" << std::flush;
	int search_type = read_number(1);

	switch(search_type) {

		case LOAN_ID: {
			std::cout << "\nEnter the loan ID:\n";
			std::cout << "(ID)->" << std::flush;
			int loan_id = read_number(8);

			const auto& loan_info = Catalogue::request_loan_info(loan_id);
			//cannot have a loan without a member associated with it
			if(loan_info.first == "") {
                std::cout << "\nNo loan found with given ID." << std::endl;
                break;
            }

			const auto& book_info = Catalogue::request_book_info(loan_info.second[0]);
			if(loan_info.second[2] - today() < 0) {
				std::cout << "\nThe Loan is overdue by: " << std::to_string(loan_info.second[2] - today()) << " days.\n";
            }

			std::cout << "\nDeleting loan no. " << loan_id << " for book: \"" << book_info[0] << "\" borrowed by: " << loan_info.first << ".\n";
			std::cout << "Proceed? (y/n): " << std::flush;
			if(!user_confirms()) {
                std::cout << "\nAborting." << std::endl;
                break;
            }

			db_request_loan_delete(loan_id);
			break;
		}

		case BOOK_ID: {
			std::cout << "Enter the book ID:\n";
			std::cout << "(ID)->" << std::flush;
			int book_id = read_number(5);
			int loan_id = Catalogue::check_books_loan(book_id);

			if(loan_id == -1) {
                std::cout << "\nNo loan found for the book with given ID." << std::endl;
                break;
            }

			const auto& loan_info = Catalogue::request_loan_info(loan_id);
			const auto& book_info = Catalogue::request_book_info(book_id);
			if(loan_info.second[2] - today() < 0) {
				std::cout << "\nThe Loan is overdue by: " << std::to_string(loan_info.second[2] - today()) << " days.\n";
            }
			std::cout << "\nDeleting loan no. " << loan_id << " for book: \"" << book_info[0] << "\" borrowed by: " << loan_info.first << ".\n";
			std::cout << "Proceed? (y/n): " << std::flush;
			if(!user_confirms()) {
                std::cout << "\nAborting." << std::endl;
                break;
            }

            db_request_loan_delete(loan_id);
			break;
		}

		default: {
			std::cerr << "Invalid input." << std::endl;
			break;
		}
	}
}

void LibraryWorker::extend_loan()
{
    const enum AddStatus {
        BOOK_ID = 1,
        LOAN_ID = 2,
    };
    std::cout << "\nWould you rather extend loan by: \n";
    std::cout << "1.Book  id\n";
    std::cout << "2.Loan  id\n";
    std::cout << "(1/2) -> " << std::flush;
    switch (read_number(1)) {
    case BOOK_ID: {
        int book_id;
        std::cout << "Enter book id: \n";
        std::cout << "(book id) -> " << std::flush;
        book_id = read_number(6);
        if (Catalogue::check_books_loan(book_id) == -1) {
            std::cout << "\nLoan is not active \n";
            break;
        }
        const std::pair<std::string, std::array<unsigned int, 3>>loan_info = Catalogue::request_loan_info(Catalogue::check_books_loan(book_id));
        std::cout << "\nLoan is active and is loaned by: " << loan_info.first << " was loaned on: " << dateToString(loan_info.second[1]) << ", loan ends on: " << dateToString(loan_info.second[2]) << "\n";
        std::cout << "How long would you like to extend (max 31 days): \n";
        std::cout << "(extend) -> " << std::flush;
        const int days = read_number(2);
        if (days > 31 || days < 1) {
            std::cout << "\nInvalid input\n";
            break;
        }
        Catalogue::extend_loan(Catalogue::check_books_loan(book_id), days);
        break;
    }
    case LOAN_ID: {
        int loan_id;
        std::cout << "\nEnter loan id: \n";
        std::cout << "(loan id) -> " << std::flush;
        loan_id = read_number(8);
        const std::pair<std::string, std::array<unsigned int, 3>>loan_info = Catalogue::request_loan_info(loan_id);
        if (loan_info.first == "") {
            std::cout << "\nLoan is not active\n";
            break;
        }
        std::cout << "\nLoan is active and is loaned by: " << loan_info.first << " was loaned on: " << dateToString(loan_info.second[1]) << ", loan ends on: " << dateToString(loan_info.second[2]) << "\n";
        std::cout << "How long would you like to extend (max 31 days): \n";
        std::cout << "(extend) -> " << std::flush;
        const int days = read_number(2);
        if (days > 31 || days < 1) {
            std::cout << "\nInvalid input\n";
            break;
        }
        Catalogue::extend_loan(Catalogue::check_books_loan(loan_info.second[0]), days);
        break;
    }
    default: {
        std::cout << "Wrong input";
        break;
    }
    }

}

void LibraryWorker::view_member_loans()
{
    std::string email; 
    std::cout << "Enter member email whose loans you are looking for\n"; 
    std::cout << "(email) -> " << std::flush; 
    email = read_string();
    const std::vector<unsigned int> member_loans = Catalogue::request_member_loans(email);
    std::vector<std::pair<std::string,std::array<unsigned int, 3>>> loan_info;
    for (auto loan_id : member_loans) {
        loan_info.push_back(Catalogue::request_loan_info(loan_id));
    }
    for (const auto& [member_email, loan_info] : loan_info) {
        std::cout << "Loan owner email: " << member_email << "book id: " << std::to_string(loan_info[0]) << "loan started: " << dateToString(loan_info[1]) << "loan will end: " << dateToString(loan_info[2]) << "\n";
    }
    
}   

bool LibraryWorker::is_admin()
{
	return false;
}

void LibraryWorker::book_search()
{
    const enum AddBook {
        AUTHOR = 1,
        TITLE = 2,
    };
    std::cout << "\nWhich type of search do you prefer? \n";
    std::cout << "1. By author \n";
    std::cout << "2. By title \n";
    std::cout << "(1/2) -> " << std::flush;
    switch (read_number(1)) {
    case AUTHOR: {
        std::string name;
        std::cout << "\nEnter the author name \n";
        std::cout << "(name) -> " << std::flush;
        name = read_string();
        const std::vector<std::pair<unsigned int, std::array<std::string, 2>>> books = Catalogue::book_search(name, 0);
        if (books.empty()) {
            std::cout << "\nNo books found written by " << name <<"\n";
            break;
        }
        std::map<unsigned int, std::string> unique_books;
        for (const auto& book : books) {
            unique_books.insert({ book.first, book.second[1] });
        }
        std::cout << "All " << name << "'s books: \n";
        for (auto& [id, title] : unique_books) {
            std::cout << title << "\n";
        }
        break;
    }
    case TITLE: {
        std::string titles;
        std::cout << "\nEnter the title \n";
        std::cout << "(title) -> " << std::flush;
        titles = read_string();
        const std::vector<std::pair<unsigned int, std::array<std::string, 2>>> books = Catalogue::book_search(titles, 1);
        if (books.empty()) {
            std::cout << "\nNo books match title: " << titles << "\n";
            break;
        }
        std::cout << "\nAll books by the title: " << titles << " in the database.\n";
        for (const auto& book : books) {
            std::string loaned_message;
            if(Catalogue::check_books_loan(book.first) == -1) loaned_message = "No\n";
            else loaned_message = "Yes\n";

            std::cout << "ID: " << std::to_string(book.first) << " Author: " << book.second[0] << " Title: \"" << book.second[1] << "\" Is loaned: " << loaned_message;
        }
        break;
    }
    default: {
        std::cout << "Wrong input";
        break;
    }
    }
}

void LibraryWorker::member_search()
{
    std::string email;
    std::cout << "Enter member email that you are looking for\n";
    std::cout << "(email) -> " << std::flush;
    email = read_string();
    std::vector<std::string> people = Catalogue::email_search(email);
    for(const auto& person : people) {
        const std::string name_surname = Catalogue::request_person_name_surname(person);
        std::cout << "Email: " << person << "\n";
        std::cout << "Full name: " << name_surname << "\n";
    }
}

bool LibraryWorker::is_valid_email(const std::string& email) {
	// This regex pattern is taken from https://docs.mapp.com/docs/email-address-validation
	const std::regex pattern("^[-!#$%&'*+/=?^_`{|}~A-Za-z0-9]+(?:\.[-!#$%&'*+/=?^_`{|}~A-Za-z0-9]+)*@([A-Za-z0-9]([A-Za-z0-9-]*[A-Za-z0-9])?\.)+[A-Za-z0-9][A-Za-z0-9-]*[A-Za-z0-9]");
	return std::regex_match(email, pattern);
}

void LibraryWorker::change_member_info() {
    const enum ChangeInfo {
        EMAIL = 1,
        NAME = 2
    };
    constexpr unsigned int MEMBER = 0;
    std::cout << "Do you want to change member email or name? \n";
    std::cout << "1. Email \n";
    std::cout << "2. Name \n";
    std::cout << "(1/2) -> " << std::flush;
    switch(read_number(1)) {
        case EMAIL: {
            std::string email;
            std::string new_email;
            std::cout << "\nEnter member email that you are looking for\n";
            std::cout << "(email) -> " << std::flush;
            email = read_string();
            std::cout << "\nEnter new email\n";
            std::cout << "(email) -> " << std::flush;
            new_email = read_string();
            std::cout<<"\nOld email: " << email << " new email: " << new_email << " Proceed? (y/n)" << std::flush;
            if (!user_confirms()) return;
            if (!Catalogue::change_email(email, new_email, MEMBER)) {
                std::cout << "\nThere is no member with this email\n";
                break;
            } 
            std::cout << "\nEmail changed sucesfully \n";
            break;
        }
        case NAME: {
            std::string email;
            std::string new_name;
            std::cout << "\nEnter member email that you are looking for\n";
            std::cout << "(email) -> " << std::flush;
            email = read_string();
            std::cout << "\nEnter new name\n";
            std::cout << "(name) -> " << std::flush;
            new_name = read_string();
            std::cout << "\nEmail: " << email << "new name: " << new_name << ". Proceed? (y/n)" << std::flush;
            if (!user_confirms()) return;
            if(!Catalogue::change_name_surname(email, new_name, MEMBER)) {
                std::cout << "\nThere is no member with this email\n";
                break;
            }
            std::cout << "\nName changed sucesfully \n";
            break;
        }
        default: {
            std::cout << "Wrong input";
            break;
        }
    }
}
void LibraryWorker::change_password() {
    std::string new_password, repeat_password;

    if(!verify_password()) {
        std::cerr << "Incorrect password. Please try again." << std::endl;
        return;
    }

    std::cout << "Enter your new password: " << std::flush;
    new_password = read_string();

    std::cout << "\nRepeat your new password: " << std::flush;
    repeat_password = read_string();

    if(new_password != repeat_password) {
        std::cerr << "\nPasswords do not match. Please try again." << std::endl;
        return;
    }
    if(Catalogue::change_password(this->email, new_password)) {
        std::cout << "\nPassword changed successfully." << std::endl;
        return;
    }
        std::cerr << "\nPassword change failed. Please try again." << std::endl;
}
