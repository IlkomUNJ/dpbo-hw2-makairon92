#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <fstream>

using namespace std;

class Item {
private:
    string id;
    string name;
    int quantity;
    double price;
    bool display;

public:
    Item();
    Item(const string& itemId, const string& itemName, int qty, double prc, bool disp);
    
    string getId() const;
    string getName() const;
    int getQuantity() const;
    double getPrice() const;
    bool isDisplay() const;
    
    void updatePrice(double newPrice);
    void updateQuantity(int newQty);
    void addQuantity(int qty);
    void reduceQuantity(int qty);
    void setDisplay(bool disp);
    
    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
};

#endif