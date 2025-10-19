#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <fstream>

using namespace std;

enum TransactionStatus {
    PENDING,
    PAID,
    COMPLETED,
    CANCELLED
};

enum TransactionType {
    DEPOSIT,
    WITHDRAW,
    PURCHASE,
    SALE
};

class Transaction {
private:
    string transactionId;
    string userId;
    string itemId;
    double amount;
    time_t timestamp;
    TransactionStatus status;
    TransactionType type;
    int quantity;

public:
    Transaction();
    Transaction(const string& tid, const string& uid, const string& iid, 
                double amt, TransactionType ttype, int qty = 1);
    
    string getTransactionId() const;
    string getUserId() const;
    string getItemId() const;
    double getAmount() const;
    time_t getTimestamp() const;
    TransactionStatus getStatus() const;
    TransactionType getType() const;
    int getQuantity() const;
    
    void setStatus(TransactionStatus stat);
    bool isWithinDays(int days) const;
    bool isToday() const;
    bool isWithinMonth() const;
    
    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
};

#endif