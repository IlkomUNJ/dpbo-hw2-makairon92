#include "buyer.h"
#include <iostream>

Buyer::Buyer() : User() {}

Buyer::Buyer(const string& uid, const string& pass) : User(uid, pass) {}

void Buyer::addOrder(const string& orderId) {
    orderIds.push_back(orderId);
}

vector<string> Buyer::getOrderIds() const {
    return orderIds;
}

void Buyer::listOrders(const string& filter) const {
    cout << "\n--- My Orders ---" << endl;
    cout << "Filter: " << filter << endl;
    for(const auto& orderId : orderIds) {
        cout << "Order ID: " << orderId << endl;
    }
}

void Buyer::checkSpendingLastKDays(int k) const {
    cout << "Checking spending for last " << k << " days..." << endl;
    cout << "Feature requires transaction history integration" << endl;
}

void Buyer::serialize(ofstream& out) const {
    User::serialize(out);
    
    size_t orderCount = orderIds.size();
    out.write((char*)&orderCount, sizeof(orderCount));
    
    for(const auto& orderId : orderIds) {
        size_t len = orderId.length();
        out.write((char*)&len, sizeof(len));
        out.write(orderId.c_str(), len);
    }
}

void Buyer::deserialize(ifstream& in) {
    User::deserialize(in);
    
    size_t orderCount;
    in.read((char*)&orderCount, sizeof(orderCount));
    
    orderIds.clear();
    for(size_t i = 0; i < orderCount; ++i) {
        size_t len;
        in.read((char*)&len, sizeof(len));
        string orderId;
        orderId.resize(len);
        in.read(&orderId[0], len);
        orderIds.push_back(orderId);
    }
}