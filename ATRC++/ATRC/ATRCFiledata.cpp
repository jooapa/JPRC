#include "./include/ATRC.h"
#include "./include/filer.h"
#include <iostream>

ATRCFiledata::ATRCFiledata() {
    // Constructor implementation
}

ATRCFiledata::~ATRCFiledata() {
    delete Variables;
    Variables = nullptr;
    delete Blocks;
    Blocks = nullptr;   
}
