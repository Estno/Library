#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

class SaveFileManager
{
private:
	std::string save_file_path;
	json workers;
	json library;
	std::string find_latest_file(const std::string& filetype);

public:
	SaveFileManager();
	//Methods of workers, admins and  maximum loan quantity and maximum return time 
	unsigned int fetch_return_period() const;
	unsigned int fetch_max_books_per_member() const;
	std::vector<std::array<std::string, 3>> fetch_admin_data() const;
	std::vector<std::array<std::string, 3>> fetch_worker_data() const;
	void new_data_workers_file(unsigned int, unsigned int, std::vector<std::array<std::string, 3>>, std::vector<std::array<std::string, 3>>) const;
	void load_workers_file();

	//Methods of members, loans and books
	std::vector<std::array<std::string, 2>> fetch_member_data() const;
	std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>> fetch_loan_data() const;
	unsigned int fetch_loan_counter() const;
	std::vector<std::tuple<std::string, std::string, unsigned int>> fetch_book_data() const;
	unsigned int fetch_book_counter() const;
	void new_data_library_file(unsigned int, std::vector<std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, std::string>>, unsigned int, std::vector<std::tuple<std::string, std::string, unsigned int>>, std::vector<std::array<std::string, 2>>) const;
	void load_library_file();
};