#pragma once
#include "LibraryWorker.h"
class LibraryAdmin : public LibraryWorker
{
    friend class Catalogue;
public:
    void delete_member();
    void add_worker();
    void add_admin();
    void delete_worker();
    void add_book();
    void delete_book();
    void delete_admin();
    bool is_admin() override;
    void change_max_books_per_member();
    void change_return_period();
    void change_worker_info();
    using LibraryWorker::LibraryWorker;
};

