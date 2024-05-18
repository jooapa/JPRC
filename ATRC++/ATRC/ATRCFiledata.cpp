#include "./include/ATRC.h"
#include "./include/filer.h"

ATRCFiledata::ATRCFiledata() {
    // Constructor implementation
}

ATRCFiledata::~ATRCFiledata() {
    delete Variables;
    Variables = nullptr;
    delete Blocks;
    Blocks = nullptr;   
}


