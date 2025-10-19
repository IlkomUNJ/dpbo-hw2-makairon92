#ifndef ONLINESTORE_H
#define ONLINESTORE_H

#include "buyer.h"
#include "seller.h"
#include "transaction.h"
#include "bank.h"
#include <map>
#include <vector>

class OnlineStore {
private:
    map<string, Buyer> buyers;
    map<string, Seller> sellers;
    vector<Transaction> transactions;
    int transactionCounter;

public:
    OnlineStore();
    
    Buyer* registerBuyer(const string& userId, const string& password);
    Seller* registerSeller(const string& userId, const string& password);
    
    Buyer* loginBuyer(const string& userId, const string& password);
    Seller* loginSeller(const string& userId, const string& password);
    
    void listAvailableItems() const;
    bool purchaseItem(Buyer* buyer, const string& itemId, int quantity, Bank& bank);
    
    void listTransactionsLastKDays(int k) const;
    void listPaidButIncomplete() const;
    void listMostMFrequentItems(int m) const;
    void listMostActiveBuyers() const;
    void listMostActiveSellersToday() const;
    
    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
};

#endif