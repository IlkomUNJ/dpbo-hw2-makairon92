#include "item.h"

Item::Item() : id(""), name(""), quantity(0), price(0.0), display(false) {}

Item::Item(const string& itemId, const string& itemName, int qty, double prc, bool disp)
    : id(itemId), name(itemName), quantity(qty), price(prc), display(disp) {}

string Item::getId() const {
    return id;
}

string Item::getName() const {
    return name;
}

int Item::getQuantity() const {
    return quantity;
}

double Item::getPrice() const {
    return price;
}

bool Item::isDisplay() const {
    return display;
}

void Item::updatePrice(double newPrice) {
    price = newPrice;
}

void Item::updateQuantity(int newQty) {
    quantity = newQty;
}

void Item::addQuantity(int qty) {
    quantity += qty;
}

void Item::reduceQuantity(int qty) {
    if(quantity >= qty) {
        quantity -= qty;
    }
}

void Item::setDisplay(bool disp) {
    display = disp;
}

void Item::serialize(ofstream& out) const {
    size_t len = id.length();
    out.write((char*)&len, sizeof(len));
    out.write(id.c_str(), len);
    
    len = name.length();
    out.write((char*)&len, sizeof(len));
    out.write(name.c_str(), len);
    
    out.write((char*)&quantity, sizeof(quantity));
    out.write((char*)&price, sizeof(price));
    out.write((char*)&display, sizeof(display));
}

void Item::deserialize(ifstream& in) {
    size_t len;
    
    in.read((char*)&len, sizeof(len));
    id.resize(len);
    in.read(&id[0], len);
    
    in.read((char*)&len, sizeof(len));
    name.resize(len);
    in.read(&name[0], len);
    
    in.read((char*)&quantity, sizeof(quantity));
    in.read((char*)&price, sizeof(price));
    in.read((char*)&display, sizeof(display));
}