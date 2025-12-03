#include "Library.h"
#include "Catalogue.h"
#include <iostream>
#include "user_input.h"

Library::Library() : sfm(), cat(), running(true), p_admin(), p_worker(), admin() {
}
void Library::init() {
    sfm.load_workers_file();
    cat.initialize_library_workers(sfm);
    cat.initialize_defaults(sfm);
    sfm.load_library_file();
    cat.initialize_books(sfm);
    cat.initialize_loans(sfm);
    cat.initialize_library_members(sfm);
    login();
}


void Library::menu() {
    const enum MenuOptions {
        NEW_LOAN = 1,
        END_LOAN,
        NEW_MEMBER,
        SEARCH_BOOK,
        SEARCH_MEMBER,
        SEARCH_LOAN,
        EXTEND_LOAN,
        VIEW_MEMBER_LOANS,
        CHANGE_MEMBER_INFO,
        SAVE,
        CHANGE_PASSWORD,
        LOGOUT,
        EXIT,
        ADD_BOOK,
        ADD_WORKER,
        ADD_ADMIN,
        DELETE_BOOK,
        DELETE_MEMBER,
        DELETE_WORKER,
        DELETE_ADMIN,
        CHANGE_MAX_BOOKS_PER_MEMBER,
        CHANGE_RETURN_PERIOD,
        CHANGE_WORKER_INFO
    };  
    const std::string menu_options = R"(1. Create new loan
2. End a loan
3. Create new member account
4. Search for a book
5. Search for a member
6. Search for a loan
7. Extend a loan
8. View a member's loans
9. Change a member's info
10. Save
11. Change your password
12. Log out
13. Exit)";
    const std::string admin_menu_options = R"(
14. Add a book
15. Create new worker account
16. Create new admin account
17. Delete a book
18. Delete a member
19. Delete a worker
20. Delete an admin
21. Change the maximum number of books a member can loan
22. Change the return period
23. Change a worker's info
)";


    int choice = 0;
    std::cout << "\nWhat would you like to do?\n";
    std::cout << (admin ? menu_options + admin_menu_options : menu_options + "\n") << std::endl;
    std::cout << "(choice)->" << std::flush;
    choice = read_number(2);
    std::cout << "\n";
    switch(choice) {
        case NEW_LOAN: {
            p_worker->new_loan();
            break;
        }
        case END_LOAN: {
            p_worker->end_loan();
            break;
        }
        case NEW_MEMBER: {
            p_worker->add_member();
            break;
        }
        case SEARCH_BOOK: {
            p_worker->book_search();
            break;
        }
        case SEARCH_MEMBER: {
            p_worker->member_search();
            break;
        }
        case SEARCH_LOAN: {
            p_worker->show_loan();
            break;
        }
        case EXTEND_LOAN: {
            p_worker->extend_loan();
            break;
        }
        case VIEW_MEMBER_LOANS: {
            p_worker->view_member_loans();
            break;
        }
        case CHANGE_MEMBER_INFO: {
            p_worker->change_member_info();
            break;
        }
        case SAVE: {
            cat.save_to_file(sfm);
            break;
        }
        case CHANGE_PASSWORD: {
            p_worker->change_password();
            break;
        }
        case LOGOUT: {
            login();
            break;
        }
        case EXIT: {
            std::cout << "Do you want to save before exiting? (y/n): " << std::flush;
            if(user_confirms()) {
                cat.save_to_file(sfm);
            }
            running = false;
            break;
        }
        case ADD_BOOK: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->add_book();
            break;
        }
        case ADD_WORKER: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->add_worker();
            break;
        }
        case ADD_ADMIN: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->add_admin();
            break;
        }
        case DELETE_BOOK: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->delete_book();
            break;
        }
        case DELETE_MEMBER: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->delete_member();
            break;
        }
        case DELETE_WORKER: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->delete_worker();
            break;
        }
        case DELETE_ADMIN: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->delete_admin();
            break;
        }
        case CHANGE_MAX_BOOKS_PER_MEMBER: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->change_max_books_per_member();
            break;
        }
        case CHANGE_RETURN_PERIOD: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->change_return_period();
            break;
        }
        case CHANGE_WORKER_INFO: {
            if(!admin) {
                std::cout << "Invalid choice.\n";
                break;
            }
            p_admin->change_worker_info();
            break;
        }
        default: {
            std::cerr << "Invalid input." << std::endl;
            break;
        }
    }
}

void Library::login() {
    std::string email, password;
    std::cout <<"Welcome to the library system!\n";
    std::cout << "Enter your email: " << std::flush;
    email = read_string();
    std::cout << "\nEnter your password: " << std::flush;
    password = read_string();
    p_worker = std::static_pointer_cast<LibraryWorker>(cat.login(email, password));
    if(p_worker== nullptr) {
        std::cout << "\nInvalid email or password. Try again." << std::endl;
        login();
    }
    else {
        std::cout << "Welcome, " << cat.request_person_name_surname(email) << "!" << std::endl;
        if(p_worker->is_admin()) {
            p_admin = std::static_pointer_cast<LibraryAdmin>(p_worker);
            admin = true;
            return;
        }
        admin = false;
    }
}
