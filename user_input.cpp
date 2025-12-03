#include "user_input.h"
#include <locale>
#include <iostream>
#include <limits>
#include <cctype>

const int read_number(const int max_digits) {
        std::string inputString;
        int input = 0;
		char ch;

        for(int i = 0; i < max_digits; ++i) {
            ch = std::cin.get();

			if(ch == '\n') {
				break;
			}
			else if(i == max_digits - 1) {
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

            if(!std::isdigit(ch)) {
				std::cerr << "Invalid input: not a number" << std::endl;
				return -1;
			}
            inputString.push_back(ch);
        }
		std::cin.clear();
		try {
			input = std::stoi(inputString);
		}
		catch(const std::invalid_argument& e) {
			std::cerr << "Invalid input: not a number" << std::endl;
			return -1;
		}

        return input;
}
const std::string read_string() {
	std::string input{};
    std::getline(std::cin, input);
    if(std::cin.fail()) {
        std::cin.clear();
        input = "";
    }

    if(input.length() > 129) {
        std::cerr << "Input too long. Truncating to 129 characters." << std::endl;
        input = input.substr(0, 129);
    }

	return input;
}
const bool user_confirms() {
	char ch;
	for(int i = 0; i < 3; ++i) {
		std::cin.get(ch);
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y') {
			if(i == 2) {
                std::cerr << "Too many invalid inputs. Exiting." << std::endl;
                return false;
            }
			std::cerr << "Invalid input. Please enter 'y' or 'n'." << std::endl;
			ch = 0;
			continue;
		}
		break;
	}
    if(ch == 'y' || ch == 'Y') {
        return true;
    }
    return false;
}
void email_to_lowercase(std::string& email) {
	std::locale loc;
	for(char& c : email) {
		if(std::isalpha(static_cast<unsigned char>(c), loc)) {
			c = std::tolower(static_cast<unsigned char>(c), loc);
		}
	}
}
