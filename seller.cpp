#include "seller.h"
#include <iostream>
#include <algorithm>

Seller::Seller() : User() {}

Seller::Seller(const string& uid, const string& pass) : User(uid, pass) {}

void Seller::addItem(const string& name, int quantity, double price, bool display) {
    static int itemCounter = 1001;
    string itemId = "Item" + to_string(itemCounter++);
    Item item(itemId, name, quantity, price, display);
    items.push_back(item);
    cout << "Item added with ID: " << itemId << endl;
}

void Seller::updateItem(const string& itemId, int quantity, double price, bool display) {
    for(auto& item : items) {
        if(item.getId() == itemId) {
            item.updatePrice(price);
            item.updateQuantity(quantity);
            item.setDisplay(display);
            cout << "Item updated successfully!" << endl;
            return;
        }
    }
    cout << "Item not found!" << endl;
}

Item* Seller::getItem(const string& itemId) {
    for(auto& item : items) {
        if(item.getId() == itemId) {
            return &item;
        }
    }
    return nullptr;
}

void Seller::listItems() const {
    cout << "\n--- My Items ---" << endl;
    for(const auto& item : items) {
        cout << "ID: " << item.getId() << " | Name: " << item.getName() 
             << " | Qty: " << item.getQuantity() << " | Price: $" << item.getPrice()
             << " | Display: " << (item.isDisplay() ? "Yes" : "No") << endl;
    }
}

void Seller::recordSale(const string& itemId) {
    salesCount[itemId]++;
}

void Seller::topKPopularItemsThisMonth(int k) const {
    cout << "\n--- Top " << k << " Popular Items This Month ---" << endl;
    
    vector<pair<string, int>> sales(salesCount.begin(), salesCount.end());
    sort(sales.begin(), sales.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for(const auto& sale : sales) {
        if(count >= k) break;
        cout << "Item: " << sale.first << " | Sales: " << sale.second << endl;
        count++;
    }
}

void Seller::listLoyalCustomers() const {
    cout << "Loyal customers feature - requires customer tracking" << endl;
}

void Seller::listSales() const {
    cout << "\n--- Sales Summary ---" << endl;
    for(const auto& sale : salesCount) {
        cout << "Item: " << sale.first << " | Count: " << sale.second << endl;
    }
}

vector<Item>& Seller::getItems() {
    return items;
}

void Seller::serialize(ofstream& out) const {
    User::serialize(out);
    
    size_t itemCount = items.size();
    out.write((char*)&itemCount, sizeof(itemCount));
    
    for(const auto& item : items) {
        item.serialize(out);
    }
    
    size_t salesSize = salesCount.size();
    out.write((char*)&salesSize, sizeof(salesSize));
    
    for(const auto& sale : salesCount) {
        size_t len = sale.first.length();
        out.write((char*)&len, sizeof(len));
        out.write(sale.first.c_str(), len);
        out.write((char*)&sale.second, sizeof(sale.second));
    }
}

void Seller::deserialize(ifstream& in) {
    User::deserialize(in);
    
    size_t itemCount;
    in.read((char*)&itemCount, sizeof(itemCount));
    
    items.clear();
    for(size_t i = 0; i < itemCount; ++i) {
        Item item;
        item.deserialize(in);
        items.push_back(item);
    }
    
    size_t salesSize;
    in.read((char*)&salesSize, sizeof(salesSize));
    
    salesCount.clear();
    for(size_t i = 0; i < salesSize; ++i) {
        size_t len;
        in.read((char*)&len, sizeof(len));
        string itemId;
        itemId.resize(len);
        in.read(&itemId[0], len);
        
        int count;
        in.read((char*)&count, sizeof(count));
        salesCount[itemId] = count;
    }
}