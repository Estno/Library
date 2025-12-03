#pragma once
#include "LibraryWorker.h"
#include "LibraryAdmin.h"
#include "Catalogue.h"
#include "SaveFileManager.h"
class Library
{
public:
    Library();
    bool running;
    void init();
    void menu();
private:
    std::shared_ptr<LibraryWorker> p_worker;
    std::shared_ptr<LibraryAdmin> p_admin;
    SaveFileManager sfm;
    Catalogue cat;
    bool admin;
    void login();
};

