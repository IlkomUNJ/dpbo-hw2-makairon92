#ifndef SELLER_H
#define SELLER_H

#include "user.h"
#include "item.h"
#include <vector>
#include <map>

class Seller : public User {
private:
    vector<Item> items;
    map<string, int> salesCount;

public:
    Seller();
    Seller(const string& uid, const string& pass);
    
    void addItem(const string& name, int quantity, double price, bool display);
    void updateItem(const string& itemId, int quantity, double price, bool display);
    Item* getItem(const string& itemId);
    
    void listItems() const;
    void recordSale(const string& itemId);
    void topKPopularItemsThisMonth(int k) const;
    void listLoyalCustomers() const;
    void listSales() const;
    
    vector<Item>& getItems();
    
    void serialize(ofstream& out) const override;
    void deserialize(ifstream& in) override;
};

#endif