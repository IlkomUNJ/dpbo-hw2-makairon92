#include "onlinestore.h"
#include <iostream>
#include <algorithm>

OnlineStore::OnlineStore() : transactionCounter(1) {}

Buyer* OnlineStore::registerBuyer(const string& userId, const string& password) {
    if(buyers.find(userId) != buyers.end()) {
        cout << "User already exists!" << endl;
        return nullptr;
    }
    buyers[userId] = Buyer(userId, password);
    return &buyers[userId];
}

Seller* OnlineStore::registerSeller(const string& userId, const string& password) {
    if(sellers.find(userId) != sellers.end()) {
        cout << "User already exists!" << endl;
        return nullptr;
    }
    sellers[userId] = Seller(userId, password);
    return &sellers[userId];
}

Buyer* OnlineStore::loginBuyer(const string& userId, const string& password) {
    auto it = buyers.find(userId);
    if(it != buyers.end() && it->second.getPassword() == password) {
        return &it->second;
    }
    cout << "Invalid credentials!" << endl;
    return nullptr;
}

Seller* OnlineStore::loginSeller(const string& userId, const string& password) {
    auto it = sellers.find(userId);
    if(it != sellers.end() && it->second.getPassword() == password) {
        return &it->second;
    }
    cout << "Invalid credentials!" << endl;
    return nullptr;
}

void OnlineStore::listAvailableItems() const {
    cout << "\n========================================" << endl;
    cout << "         AVAILABLE PRODUCTS" << endl;
    cout << "========================================" << endl;
    
    int count = 0;
    for(const auto& sellerPair : sellers) {
        for(const auto& item : const_cast<Seller&>(sellerPair.second).getItems()) {
            if(item.isDisplay() && item.getQuantity() > 0) {
                count++;
                cout << "\n[" << count << "] " << item.getName() << endl;
                cout << "    Item ID    : " << item.getId() << endl;
                cout << "    Price      : $" << item.getPrice() << endl;
                cout << "    Available  : " << item.getQuantity() << " units" << endl;
                cout << "    Seller     : " << sellerPair.first << endl;
                cout << "----------------------------------------" << endl;
            }
        }
    }
    
    if(count == 0) {
        cout << "\nNo items available at the moment." << endl;
    }
    cout << "========================================\n" << endl;
}

bool OnlineStore::purchaseItem(Buyer* buyer, const string& itemId, int quantity, Bank& bank) {
    // Find the item
    Item* item = nullptr;
    Seller* seller = nullptr;
    
    for(auto& sellerPair : sellers) {
        item = sellerPair.second.getItem(itemId);
        if(item) {
            seller = &sellerPair.second;
            break;
        }
    }
    
    if(!item || !seller) {
        cout << "Item not found!" << endl;
        return false;
    }
    
    if(item->getQuantity() < quantity) {
        cout << "Insufficient stock!" << endl;
        return false;
    }
    
    double totalPrice = item->getPrice() * quantity;
    
    // Check buyer balance
    double buyerBalance = bank.getBalance(buyer->getBankAccountId());
    if(buyerBalance < totalPrice) {
        cout << "Insufficient balance! Need $" << totalPrice << ", have $" << buyerBalance << endl;
        return false;
    }
    
    // Process payment
    if(!bank.withdraw(buyer->getBankAccountId(), totalPrice)) {
        return false;
    }
    bank.deposit(seller->getBankAccountId(), totalPrice);
    
    // Update item quantity
    item->reduceQuantity(quantity);
    
    // Create transaction
    string transId = "TRANS" + to_string(transactionCounter++);
    Transaction trans(transId, buyer->getUserId(), itemId, totalPrice, PURCHASE, quantity);
    trans.setStatus(PAID);
    transactions.push_back(trans);
    
    // Add to buyer's orders
    buyer->addOrder(transId);
    
    // Record seller's sale
    seller->recordSale(itemId);
    
    cout << "Purchase successful! Transaction ID: " << transId << endl;
    return true;
}

void OnlineStore::listTransactionsLastKDays(int k) const {
    cout << "\n--- Transactions Last " << k << " Days ---" << endl;
    for(const auto& trans : transactions) {
        if(trans.isWithinDays(k)) {
            cout << "ID: " << trans.getTransactionId() 
                 << " | User: " << trans.getUserId()
                 << " | Item: " << trans.getItemId()
                 << " | Amount: $" << trans.getAmount()
                 << " | Status: " << trans.getStatus() << endl;
        }
    }
}

void OnlineStore::listPaidButIncomplete() const {
    cout << "\n--- Paid But Incomplete Transactions ---" << endl;
    for(const auto& trans : transactions) {
        if(trans.getStatus() == PAID) {
            cout << "ID: " << trans.getTransactionId()
                 << " | User: " << trans.getUserId()
                 << " | Item: " << trans.getItemId()
                 << " | Amount: $" << trans.getAmount() << endl;
        }
    }
}

void OnlineStore::listMostMFrequentItems(int m) const {
    cout << "\n--- Top " << m << " Frequent Items ---" << endl;
    
    map<string, int> itemCount;
    for(const auto& trans : transactions) {
        if(trans.getType() == PURCHASE) {
            itemCount[trans.getItemId()] += trans.getQuantity();
        }
    }
    
    vector<pair<string, int>> sorted(itemCount.begin(), itemCount.end());
    sort(sorted.begin(), sorted.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for(const auto& item : sorted) {
        if(count >= m) break;
        cout << "Item: " << item.first << " | Count: " << item.second << endl;
        count++;
    }
}

void OnlineStore::listMostActiveBuyers() const {
    cout << "\n--- Most Active Buyers ---" << endl;
    
    map<string, int> buyerCount;
    for(const auto& trans : transactions) {
        if(trans.getType() == PURCHASE) {
            buyerCount[trans.getUserId()]++;
        }
    }
    
    vector<pair<string, int>> sorted(buyerCount.begin(), buyerCount.end());
    sort(sorted.begin(), sorted.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    for(const auto& buyer : sorted) {
        cout << "Buyer: " << buyer.first << " | Transactions: " << buyer.second << endl;
    }
}

void OnlineStore::listMostActiveSellersToday() const {
    cout << "\n--- Most Active Sellers Today ---" << endl;
    
    map<string, int> sellerCount;
    
    // Count transactions per seller today
    for(const auto& trans : transactions) {
        if(trans.getType() == PURCHASE && trans.isToday()) {
            // Find which seller owns this item
            for(const auto& sellerPair : sellers) {
                Item* item = const_cast<Seller&>(sellerPair.second).getItem(trans.getItemId());
                if(item) {
                    sellerCount[sellerPair.first]++;
                    break;
                }
            }
        }
    }
    
    vector<pair<string, int>> sorted(sellerCount.begin(), sellerCount.end());
    sort(sorted.begin(), sorted.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    for(const auto& seller : sorted) {
        cout << "Seller: " << seller.first << " | Transactions: " << seller.second << endl;
    }
}

void OnlineStore::serialize(ofstream& out) const {
    out.write((char*)&transactionCounter, sizeof(transactionCounter));
    
    // Serialize buyers
    size_t buyerCount = buyers.size();
    out.write((char*)&buyerCount, sizeof(buyerCount));
    for(const auto& pair : buyers) {
        pair.second.serialize(out);
    }
    
    // Serialize sellers
    size_t sellerCount = sellers.size();
    out.write((char*)&sellerCount, sizeof(sellerCount));
    for(const auto& pair : sellers) {
        pair.second.serialize(out);
    }
    
    // Serialize transactions
    size_t transCount = transactions.size();
    out.write((char*)&transCount, sizeof(transCount));
    for(const auto& trans : transactions) {
        trans.serialize(out);
    }
}

void OnlineStore::deserialize(ifstream& in) {
    in.read((char*)&transactionCounter, sizeof(transactionCounter));
    
    // Deserialize buyers
    size_t buyerCount;
    in.read((char*)&buyerCount, sizeof(buyerCount));
    buyers.clear();
    for(size_t i = 0; i < buyerCount; ++i) {
        Buyer buyer;
        buyer.deserialize(in);
        buyers[buyer.getUserId()] = buyer;
    }
    
    // Deserialize sellers
    size_t sellerCount;
    in.read((char*)&sellerCount, sizeof(sellerCount));
    sellers.clear();
    for(size_t i = 0; i < sellerCount; ++i) {
        Seller seller;
        seller.deserialize(in);
        sellers[seller.getUserId()] = seller;
    }
    
    // Deserialize transactions
    size_t transCount;
    in.read((char*)&transCount, sizeof(transCount));
    transactions.clear();
    for(size_t i = 0; i < transCount; ++i) {
        Transaction trans;
        trans.deserialize(in);
        transactions.push_back(trans);
    }
}