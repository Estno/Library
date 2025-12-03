#include "SaveFileManager.h"
#include "date.h"
#include "fstream"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;



SaveFileManager::SaveFileManager() :
	save_file_path("./saves/"),
	workers(json{}),
	library(json{})
{

}
//Checking the latest file
std::string SaveFileManager::find_latest_file(const std::string& filetype)
{
	const fs::path folder_path = save_file_path;
	const std::string file_extension = ".json";
	//Checking if a folder exists and is a directory
	if (!fs::exists(folder_path) || !fs::is_directory(folder_path)) {
		std::cerr << "The specified path does not exist or is not a directory: " << folder_path << std::endl;
		fs::create_directories(folder_path);
	}
	unsigned long newest_file = 0;
	for (const auto& entry : fs::directory_iterator(folder_path)) {
		if (!entry.is_regular_file()) continue;
		std::string file = entry.path().filename().string();
		if (file.substr(0, filetype.length()) != filetype) continue; 
		std::string cut = file.substr(filetype.length(), file.length() - filetype.length() - file_extension.length()); //Removing the part of the filename that is not the date
		if (std::stoul(cut) > newest_file) {
			newest_file = std::stoul(cut);
		}
	}
	return filetype + std::to_string(newest_file) + file_extension;
}

//Data of employees, admins and data on the maximum loan quantity and maximum return time
void SaveFileManager::load_workers_file()
{
	std::ifstream workers_data(save_file_path + find_latest_file("library_workers"));
	std::cout << "Loading workers data..." << std::endl;
	if (!workers_data.good()) {
		std::cout << "File can't be opened or does not exist!" << std::endl; // Possibility of file not existing
		std::cout << "Loading default data." << std::endl; //Checking the file with default data
        //Entering data into a file: default_workers
        const json data = json::parse(R"(
				{
					"RETURN_PERIOD": 14,
					"MAX_BOOKS_PER_MEMBER": 5,
					"LibraryWorkers": [
						],
					"LibraryAdmins": [
						[
						"admin@library.com", 
						"admin", 
						"admin" 
						]
					]
				}
				)");
        workers = data;
    }
	else {
		workers = json::parse(workers_data);
	}
}

unsigned int SaveFileManager::fetch_return_period() const
{
	return workers["RETURN_PERIOD"];
}

unsigned int SaveFileManager::fetch_max_books_per_member() const
{
	return workers["MAX_BOOKS_PER_MEMBER"];
}

std::vector<std::array<std::string, 3>> SaveFileManager::fetch_admin_data() const
{
	std::vector<std::array<std::string, 3>> admin_data;
	for (const auto& admin : workers["LibraryAdmins"]) {
		admin_data.push_back({ admin[0], admin[1], admin[2] });
	}
	return admin_data;
}

std::vector<std::array<std::string, 3>> SaveFileManager::fetch_worker_data() const
{
	std::vector<std::array<std::string, 3>> worker_data;
	for (const auto& worker : workers["LibraryWorkers"]) {
		worker_data.push_back({ worker[0], worker[1], worker[2] });
	}
	return worker_data;
}



//Books, loans and members data
void SaveFileManager::load_library_file()
{
	std::ifstream library_data(save_file_path + find_latest_file("save_file"));
	std::cout << "Loading library data..." << std::endl;
	if (!library_data.good()) {
		std::cout << "File can't be opened or does not exist!" << std::endl;
	}
	else {
		library = json::parse(library_data);
	}
}

std::vector< std::array< std::string, 2>> SaveFileManager::fetch_member_data() const
{
	std::vector<std::array<std::string, 2>> member_data;
	if (library == json{}) return member_data;
	for (const auto& member : library["Members"]) {
		member_data.push_back({ member[0], member[1] });
	}
	return member_data;
}

std::vector< std::tuple< unsigned int, unsigned int, unsigned int, unsigned int, std::string>> SaveFileManager::fetch_loan_data() const
{
	std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> loan_data;
	if (library == json{}) return loan_data;
	for (const auto& loan : library["Loans"]) {
		loan_data.push_back({ loan[0], loan[1], loan[2], loan[3], loan[4] });
	}
	return loan_data;
}

unsigned int SaveFileManager::fetch_loan_counter() const
{
	if(library != json{}) return library["Loan_counter"];
	return 0;
}

std::vector< std::tuple< std::string, std::string, unsigned int>> SaveFileManager::fetch_book_data() const
{
	std::vector<std::tuple<std::string, std::string, unsigned int>> book_data;
	if (library == json{}) return book_data;
	for (const auto& book : library["Books"]) {
		book_data.push_back({ book[0], book[1], book[2] });
	}
	return book_data;
}

unsigned int SaveFileManager::fetch_book_counter() const
{
	if(library != json{}) return library["Book_counter"];
	return 0;
}

//Saving new data to files
void SaveFileManager::new_data_workers_file(unsigned int RETURN_PERIOD, unsigned int MAX_BOOKS_PER_MEMBER, std::vector<std::array<std::string,3>> worker_data, std::vector<std::array<std::string,3>> admin_data) const
{
	std::ofstream new_workers_data(save_file_path + "library_workers" + std::to_string(today()) + ".json");
	if (!new_workers_data.is_open()) {
		std::cout << "Error! Unable to create the file! " << std::endl;
	}
	json w; //workers 
	for (auto& worker : worker_data) {
		json lw(worker);
		w.push_back(lw);
	}
	json a; //admins 
	for (auto& admin : admin_data) {
		json la(admin);
		a.push_back(la);
	}
	json workers_info = json{ {"RETURN_PERIOD", RETURN_PERIOD}, {"MAX_BOOKS_PER_MEMBER", MAX_BOOKS_PER_MEMBER},{"LibraryWorkers", w}, {"LibraryAdmins", a} };
	new_workers_data << workers_info;
}


void SaveFileManager::new_data_library_file(unsigned int loan_counter, std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> loan_data,   unsigned int book_counter,std::vector<std::tuple<std::string, std::string, unsigned int>> book_data,std::vector<std::array<std::string, 2>> member_data) const
{
	std::ofstream new_library_data(save_file_path + "save_file" + std::to_string(today()) + ".json");
	if (!new_library_data.is_open()) std::cout << "Error! Unable to create the file! " << std::endl;
	json l; //loans
	for (auto& loan : loan_data) {
		json t = {std::get<0>(loan), std::get<1>(loan), std::get<2>(loan), std::get<3>(loan), std::get<4>(loan)};
		l.push_back(t);
	}
	json b; //books
	for (auto& book : book_data) {
		json k = {std::get<0>(book), std::get<1>(book), std::get<2>(book)};
		b.push_back(k);
	}
	json m; //members
	for (auto& member : member_data) {
		json j_array = json::array({ member[0], member[1] });
		m.push_back(j_array);
	}
	json library_data = json{ {"Loan_counter", loan_counter}, { "Loans", l }, {"Book_counter", book_counter}, { "Books", b }, {"Members", m} };
	new_library_data << library_data;
}
