#include "LibraryAdmin.h"
#include <iostream>
#include "Catalogue.h"
#include "user_input.h"

void LibraryAdmin::delete_member()
{
	constexpr unsigned int MEMBER = 0;
	std::string email;

	const enum DeleteStatus {
        NOT_FOUND = -1,
        HAS_LOANS = -2,
        FAILED = -3,
		NOT_A_MEMBER = -4,
        SUCCESS = 0
    };

	std::cout << "Enter the email of the member you want to delete:\n";
	std::cout << "(Email)->" << std::flush;
	email = read_string();
	email_to_lowercase(email);
	std::cout << "\nDeleting member with email: " << email << ". Proceed? (y/n): " << std::flush;
	if(!user_confirms()) return;

	switch(Catalogue::delete_person(email, MEMBER)) {
        case NOT_FOUND:
            std::cout << "\nNo member found with given email." << std::endl;
            break;
        case HAS_LOANS:
            std::cout << "\nThe member has active loans, and therefore can't be deleted." << std::endl;
            break;
        case FAILED:
            std::cout << "\nThe member was found but there was an error while attempting to delete. Please try again." << std::endl;
            break;
		case NOT_A_MEMBER:
			std::cout << "\nThe person you are trying to delete is not a member." << std::endl;
			break;
        case SUCCESS:
            std::cout << "\nMember deleted successfully." << std::endl;
            break;
	}
}

void LibraryAdmin::add_worker() {
    std::string email;
    std::string name_surname;
    std::string password;
    const enum AddStatus {
        EXISTS = -1,
        SUCCESS = 0
    };
    auto extract_name_surname = [](const std::string& name_surname) -> std::string {
        auto is_alpha_or_space = [](char c) -> bool {
            return std::isalpha(c) || std::isspace(c);
            };
        auto first_space = name_surname.find(' ');
        std::string extracted_name_surname;
        if (first_space == std::string::npos) {
            return "";
        }
        auto second_space = name_surname.find(' ', first_space + 1);
        auto newline = name_surname.find('\n');

        if (second_space == std::string::npos) second_space = name_surname.size();
        if (newline == std::string::npos) newline = name_surname.size();
        auto end_index = std::min(second_space, newline);
        extracted_name_surname = name_surname.substr(0, end_index);
        if (!std::all_of(extracted_name_surname.begin(), extracted_name_surname.end(), is_alpha_or_space)) {
            return "";
        }
        return extracted_name_surname;
        };
    for (int i = 0; i < 2; ++i) {
        std::cout << "Enter the email of the new worker:\n";
        std::cout << "(email)->" << std::flush;
        email = read_string();

        std::cout << "\nConfirm email:\n";
        std::cout << "(email)->" << std::flush;
        std::string confirm_email = read_string();

        if (email == confirm_email) {
            break;
        }
        email = "";
        std::cerr << "\nEmails do not match. Please try again." << std::endl;
    }
    if (!LibraryWorker::is_valid_email(email)) {
        std::cerr << "\nInvalid email. Please try again." << std::endl;
        return;
    }
    email_to_lowercase(email);
    std::cout << "\nEnter the name and surname of the worker:\n";
    std::cout << "(name_surname)->" << std::flush;
    name_surname = read_string();
    name_surname = extract_name_surname(name_surname);
    for (int i = 0; i < 2; ++i) {
        std::cout << "\nEnter the password of the worker:\n";
        std::cout << "(password)->" << std::flush;
        password = read_string();

        std::cout << "\nConfirm password:\n";
        std::cout << "(password)->" << std::flush;
        std::string confirm_password = read_string();

        if (password == confirm_password) {
            break;
        }
        password = "";
        std::cerr << "\nPasswords do not match. Please try again." << std::endl;
    }
    std::cout << "\nNew worker with email: " << email << " and name: " << name_surname  << ". Proceed? (y/n): " << std::flush;
    if (!user_confirms()) return;
    switch (Catalogue::add_worker(email, name_surname, password)) {
    case EXISTS:
        std::cout << "\nA worker with the given email already exists." << std::endl;
        break;
    case SUCCESS:
        std::cout << "\nWorker added successfully." << std::endl;
        break;
    }
}

void LibraryAdmin::add_admin() {
    std::string email;
    std::string name_surname;
    std::string password;
    const enum AddStatus {
        EXISTS = -1,
        SUCCESS = 0
    };
    auto extract_name_surname = [](const std::string& name_surname) -> std::string {
        auto is_alpha_or_space = [](char c) -> bool {
            return std::isalpha(c) || std::isspace(c);
            };
        auto first_space = name_surname.find(' ');
        std::string extracted_name_surname;
        if (first_space == std::string::npos) {
            return "";
        }
        auto second_space = name_surname.find(' ', first_space + 1);
        auto newline = name_surname.find('\n');

        if (second_space == std::string::npos) second_space = name_surname.size();
        if (newline == std::string::npos) newline = name_surname.size();
        auto end_index = std::min(second_space, newline);
        extracted_name_surname = name_surname.substr(0, end_index);
        if (!std::all_of(extracted_name_surname.begin(), extracted_name_surname.end(), is_alpha_or_space)) {
            return "";
        }
        return extracted_name_surname;
        };
    for (int i = 0; i < 2; ++i) {
        std::cout << "\nEnter the email of the new admin:\n";
        std::cout << "(email)->" << std::flush;
        email = read_string();

        std::cout << "\nConfirm email:\n";
        std::cout << "(email)->" << std::flush;
        std::string confirm_email = read_string();

        if (email == confirm_email) {
            break;
        }
        email = "";
        std::cerr << "\nEmails do not match. Please try again." << std::endl;
    }
    if (!LibraryWorker::is_valid_email(email)) {
        std::cerr << "\nInvalid email. Please try again." << std::endl;
        return;
    }
    email_to_lowercase(email);
    std::cout << "\nEnter the name and surname of the admin:\n";
    std::cout << "(name_surname)->" << std::flush;
    name_surname = read_string();
    name_surname = extract_name_surname(name_surname);
    for (int i = 0; i < 2; ++i) {
        std::cout << "\nEnter the password of the admin:\n";
        std::cout << "(password)->" << std::flush;
        password = read_string();

        std::cout << "\nConfirm password:\n";
        std::cout << "(password)->" << std::flush;
        std::string confirm_password = read_string();

        if (password == confirm_password) {
            break;
        }
        password = "";
        std::cerr << "\nPasswords do not match. Please try again." << std::endl;
    }
    std::cout << "\nNew admin with email: " << email << " and name: " << name_surname << ". Proceed? (y/n): " << std::flush;
    if (!user_confirms()) return;
    switch (Catalogue::add_admin(email, name_surname, password)) {
    case EXISTS:
        std::cout << "\nA admin with the given email already exists." << std::endl;
        break;
    case SUCCESS:
        std::cout << "\nAdmin added successfully." << std::endl;
        break;
    }
}

void LibraryAdmin::delete_worker() {
    constexpr unsigned int WORKER = 1;
    std::string email;

	const enum DeleteStatus {
        NOT_FOUND = -1,
        HAS_LOANS = -2,
        FAILED = -3,
        NOT_A_WORKER = -5,
        SUCCESS = 0
    };
    std::cout << "\nEnter the email of the worker you want to delete:\n";
    std::cout << "(Email)->" << std::flush;
    email = read_string();
    email_to_lowercase(email);
    std::cout << "\nDeleting worker with email: " << email << ". Proceed? (y/n): " << std::flush;
    if(!user_confirms()) return; //add password confirmation
    if(!verify_password()) std::cout << "\n Incorrect password" << std::endl;  return;

	switch(Catalogue::delete_person(email, WORKER)) {
        case NOT_FOUND:
            std::cout << "\nNo worker found with given email." << std::endl;
            break;
        case HAS_LOANS:
            std::cout << "\nThe worker has active loans, and therefore can't be deleted." << std::endl;
            break;
        case FAILED:
            std::cout << "\nThe worker was found but there was an error while attempting to delete. Please try again." << std::endl;
            break;
        case NOT_A_WORKER:
            std::cout << "\nThe person you are trying to delete is not a worker." << std::endl;
            break;
        case SUCCESS:
            std::cout << "\nWorker deleted successfully." << std::endl;
            break;
    }
}

void LibraryAdmin::delete_admin() {
    constexpr unsigned int ADMIN = 2;
    std::string email;

    const enum DeleteStatus {
        NOT_FOUND = -1,
        HAS_LOANS = -2,
        FAILED = -3,
        NOT_AN_ADMIN = -6,
        SUCCESS = 0
    };

    std::cout << "\nEnter the email of the admin you want to delete:\n";
    std::cout << "(Email)->" << std::flush;
    email = read_string();
    email_to_lowercase(email);
    if(this->email == email) {
        std::cout << "\nYou can't delete yourself." << std::endl;
        return;
    }
    std::cout << "\nDeleting admin with email: " << email << ". Proceed? (y/n): " << std::flush;
    if(!user_confirms()) return; 
    if(!verify_password()) std::cout << "\n Incorrect password" << std::endl;  return;

    switch(Catalogue::delete_person(email, ADMIN)) {
        case NOT_FOUND:
            std::cout << "\nNo admin found with given email." << std::endl;
            break;
        case HAS_LOANS:
            std::cout << "\nThe admin has active loans, and therefore can't be deleted." << std::endl;
            break;
        case FAILED:
            std::cout << "\nThe admin was found but there was an error while attempting to delete. Please try again." << std::endl;
            break;
        case NOT_AN_ADMIN:
            std::cout << "\nThe person you are trying to delete is not an admin." << std::endl;
            break;
        case SUCCESS:
            std::cout << "\nAdmin deleted successfully." << std::endl;
            break;
    }
}

void LibraryAdmin::add_book() {
    const enum AddBook {
        EXISTING_BOOK = 1,
        NEW_BOOK = 2,
    };
    std::cout << "\n1. Add a book that is already in a database. \n";
    std::cout << "2. Add new book. \n";
    std::cout << "(1/2) -> " << std::flush;
    switch (read_number(1)) {
    case EXISTING_BOOK: {
        unsigned int book_id;
        unsigned int amount;
        std::cout << "\nEnter the ID of the book you want to add:\n";
        std::cout << "(ID)-> " << std::flush;
        book_id = read_number(6);
        std::cout << "\nEnter how may books you want to add (max99): \n";
        std::cout << "(Amount) -> " << std::flush;
        amount = read_number(2);
        if(amount < 1) {
            std::cout << "Wrong input\n";
            break;
        }
        const std::array<std::string, 2> book_info = Catalogue::request_book_info(book_id);
        std::cout << "Adding " << std::to_string(amount) << " books of title \"" << book_info[1] << "\" written by: " << book_info[0] << ". Proceed? (y/n): " << std::flush;
        if (!user_confirms()) return;
        Catalogue::add_book(book_id, amount);
        break;
    }
    case NEW_BOOK: {
        std::string title;
        std::string author;
        std::cout << "\nEnter book's author: \n";
        std::cout << "(Author) -> " << std::flush;
        author = read_string();
        std::cout << "\nEnter book title: \n";
        std::cout << "(Title) -> " << std::flush;
        title = read_string();
        std::cout << "\nAdding book: \"" << title << "\" author: " << author << ". Proceed? (y/n): " << std::flush;
        if (!user_confirms()) return;
        Catalogue::add_book(author, title);
        break;
    }
    default: {
        std::cout << "Wrong input \n";
        break;
    }
    }
    
}

void LibraryAdmin::delete_book()
{
	enum DeleteStatus
	{
		NOT_FOUND = -1,
		LOANED = -2,
		FAILED = -3,
		SUCCESS = 0
	};

	unsigned int book_id;
	std::cout << "\nEnter the ID of the book you want to delete:\n";
	std::cout << "(ID)->" << std::flush;
	book_id = read_number(6);
	std::cout << "\nDeleting book with ID: " << book_id << ". Proceed? (y/n): " << std::flush;
	if(!user_confirms()) return;
	switch(Catalogue::delete_book(book_id)) {
		case NOT_FOUND:
			std::cout << "\nNo book found with given ID." << std::endl;
			break;
		case LOANED:
			std::cout << "\nThe book is currently loaned and can't be deleted." << std::endl;
			break;
		case FAILED:
			std::cout << "\nThe book was found but there was an error while attempting to delete. Please try again." << std::endl;
			break;
		case SUCCESS:
			std::cout << "\nBook deleted successfully." << std::endl;
			break;
	}
}

bool LibraryAdmin::is_admin()
{
	return true;
}

void LibraryAdmin::change_max_books_per_member()
{
    unsigned int new_max;
    std::cout << "\nWhat is the new max books per member(max 20)? \n";
    std::cout << "(new max) -> " << std::flush;
    new_max = read_number(2);
    if (new_max < 21 || new_max > 0) {
        std::cout << "\nNew limit is: " << new_max << ". Proceed? (y/n)\n" << std::flush;
        if (!user_confirms()) return;
        Catalogue::change_max_books_per_member(new_max);
    }
    std::cout << "Wrong input\n";
}

void LibraryAdmin::change_return_period()
{
    unsigned int new_return_period;
    std::cout << "\nWhat is the new return period(max 30)? \n";
    std::cout << "(new max) -> " << std::flush;
    new_return_period = read_number(2);
    if (new_return_period < 31 || new_return_period > 0) {
        std::cout << "\nNew limit is: " << new_return_period << ". Proceed? (y/n)\n" << std::flush;
        if (!user_confirms()) return;
        Catalogue::change_return_period(new_return_period);
    }
    std::cout << "Wrong input\n";
}

void LibraryAdmin::change_worker_info() {
    const enum ChangeInfo {
        EMAIL = 1, 
        NAME = 2
};
    const enum PersonType {
        WORKER = 1,
        ADMIN = 2
    };
    std::cout << "\nDo you want to edit worker or admin info? \n";
    std::cout << "1. Worker \n";
    std::cout << "2. Admin \n";
    std::cout << "(1/2) -> " << std::flush;
    switch(read_number(1)) {
        case WORKER: {
            std::cout << "\nDo you want to change worker email or name? \n";
            std::cout << "1. Email \n";
            std::cout << "2. Name \n";
            std::cout << "(1/2) -> " << std::flush;
            switch(read_number(1)) {
                case EMAIL: {
                    std::string email;
                    std::string new_email;
                    std::cout << "\nEnter worker email that you are looking for\n";
                    std::cout << "(email) -> " << std::flush;
                    email = read_string();
                    std::cout << "\nEnter new email\n";
                    std::cout << "(email) -> " << std::flush;
                    new_email = read_string();
                    std::cout << "\nOld email: " << email << " new email: " << new_email << ". Proceed? (y/n)" << std::flush;
                    if(!user_confirms()) return;
                    if(!Catalogue::change_email(email, new_email, WORKER)) {
                        std::cout << "\nThere is no worker with this email\n";
                        break;
                    }
                    std::cout << "\nEmail changed sucesfully \n";
                    break;
                }
                case NAME: {
                    std::string email;
                    std::string new_name;
                    std::cout << "\nEnter worker email that you are looking for\n";
                    std::cout << "(email) -> " << std::flush;
                    email = read_string();
                    std::cout << "\nEnter new name\n";
                    std::cout << "(name) -> " << std::flush;
                    new_name = read_string();
                    std::cout << "\nEmail: " << email << " new name: " << new_name << ". Proceed? (y/n)" << std::flush;
                    if(!user_confirms()) return;
                    if(!Catalogue::change_name_surname(email, new_name, WORKER)) {
                        std::cout << "\nThere is no worker with this email\n";
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
            break;
        }
        case ADMIN: {
            std::cout << "\nDo you want to change admin email or name? \n";
            std::cout << "1. Email \n";
            std::cout << "2. Name \n";
            std::cout << "(1/2) -> " << std::flush;
            switch(read_number(1)) {
                case EMAIL: {
                    std::string email;
                    std::string new_email;
                    std::cout << "\nEnter admin email that you are looking for\n";
                    std::cout << "(email) -> " << std::flush;
                    email = read_string();
                    std::cout << "\nEnter new email\n";
                    std::cout << "(email) -> " << std::flush;
                    new_email = read_string();
                    std::cout << "\nOld email: " << email << " new email: " << new_email << ". Proceed? (y/n)" << std::flush;
                    if(!user_confirms()) return;
                    if(!Catalogue::change_email(email, new_email, ADMIN)) {
                        std::cout << "\nThere is no admin with this email\n";
                        break;
                    }
                    std::cout << "\nEmail changed sucesfully \n";
                    break;
                }
                case NAME: {
                    std::string email;
                    std::string new_name;
                    std::cout << "\nEnter admin email that you are looking for\n";
                    std::cout << "(email) -> " << std::flush;
                    email = read_string();
                    std::cout << "\nEnter new name\n";
                    std::cout << "(name) -> " << std::flush;
                    new_name = read_string();
                    std::cout << "\nEmail: " << email << " new name: " << new_name << ". Proceed? (y/n)" << std::flush;
                    if(!user_confirms()) return;
                    if(!Catalogue::change_name_surname(email, new_name, ADMIN)) {
                        std::cout << "\nThere is no admin with this email\n";
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
            break;
        }
    }
}
