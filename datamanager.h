#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "bank.h"
#include "onlinestore.h"
#include <string>

class DataManager {
private:
    const string BANK_FILE = "data/bank.dat";
    const string STORE_FILE = "data/store.dat";

public:
    DataManager();
    
    void saveBank(const Bank& bank);
    Bank loadBank();
    
    void saveStore(const OnlineStore& store);
    OnlineStore loadStore();
    
    void createDataDirectory();
};

#endif