#ifndef BUYER_H
#define BUYER_H

#include "user.h"
#include "transaction.h"
#include <vector>

class Buyer : public User {
private:
    vector<string> orderIds;

public:
    Buyer();
    Buyer(const string& uid, const string& pass);
    
    void addOrder(const string& orderId);
    vector<string> getOrderIds() const;
    
    void listOrders(const string& filter) const;
    void checkSpendingLastKDays(int k) const;
    
    void serialize(ofstream& out) const override;
    void deserialize(ifstream& in) override;
};

#endif