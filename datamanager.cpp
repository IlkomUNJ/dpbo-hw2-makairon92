#include "datamanager.h"
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

DataManager::DataManager() {
    createDataDirectory();
}

void DataManager::createDataDirectory() {
#ifdef _WIN32
    mkdir("data");
#else
    mkdir("data", 0777);
#endif
}

void DataManager::saveBank(const Bank& bank) {
    ofstream out(BANK_FILE, ios::binary);
    if(!out) {
        cout << "Error: Cannot create bank file!" << endl;
        return;
    }
    bank.serialize(out);
    out.close();
    cout << "Bank data saved successfully!" << endl;
}

Bank DataManager::loadBank() {
    Bank bank;
    ifstream in(BANK_FILE, ios::binary);
    if(!in) {
        cout << "No existing bank data found. Starting fresh." << endl;
        return bank;
    }
    bank.deserialize(in);
    in.close();
    cout << "Bank data loaded successfully!" << endl;
    return bank;
}

void DataManager::saveStore(const OnlineStore& store) {
    ofstream out(STORE_FILE, ios::binary);
    if(!out) {
        cout << "Error: Cannot create store file!" << endl;
        return;
    }
    store.serialize(out);
    out.close();
    cout << "Store data saved successfully!" << endl;
}

OnlineStore DataManager::loadStore() {
    OnlineStore store;
    ifstream in(STORE_FILE, ios::binary);
    if(!in) {
        cout << "No existing store data found. Starting fresh." << endl;
        return store;
    }
    store.deserialize(in);
    in.close();
    cout << "Store data loaded successfully!" << endl;
    return store;
}