#include "transaction.h"
#include <cmath>

Transaction::Transaction() 
    : transactionId(""), userId(""), itemId(""), amount(0.0), 
      timestamp(time(nullptr)), status(PENDING), type(PURCHASE), quantity(1) {}

Transaction::Transaction(const string& tid, const string& uid, const string& iid, 
                         double amt, TransactionType ttype, int qty)
    : transactionId(tid), userId(uid), itemId(iid), amount(amt), 
      timestamp(time(nullptr)), status(PENDING), type(ttype), quantity(qty) {}

string Transaction::getTransactionId() const {
    return transactionId;
}

string Transaction::getUserId() const {
    return userId;
}

string Transaction::getItemId() const {
    return itemId;
}

double Transaction::getAmount() const {
    return amount;
}

time_t Transaction::getTimestamp() const {
    return timestamp;
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

TransactionType Transaction::getType() const {
    return type;
}

int Transaction::getQuantity() const {
    return quantity;
}

void Transaction::setStatus(TransactionStatus stat) {
    status = stat;
}

bool Transaction::isWithinDays(int days) const {
    time_t now = time(nullptr);
    double seconds = difftime(now, timestamp);
    return seconds <= (days * 24 * 3600);
}

bool Transaction::isToday() const {
    time_t now = time(nullptr);
    struct tm* nowTm = localtime(&now);
    struct tm* transTm = localtime(&timestamp);
    
    return (nowTm->tm_year == transTm->tm_year &&
            nowTm->tm_mon == transTm->tm_mon &&
            nowTm->tm_mday == transTm->tm_mday);
}

bool Transaction::isWithinMonth() const {
    return isWithinDays(30);
}

void Transaction::serialize(ofstream& out) const {
    size_t len = transactionId.length();
    out.write((char*)&len, sizeof(len));
    out.write(transactionId.c_str(), len);
    
    len = userId.length();
    out.write((char*)&len, sizeof(len));
    out.write(userId.c_str(), len);
    
    len = itemId.length();
    out.write((char*)&len, sizeof(len));
    out.write(itemId.c_str(), len);
    
    out.write((char*)&amount, sizeof(amount));
    out.write((char*)&timestamp, sizeof(timestamp));
    out.write((char*)&status, sizeof(status));
    out.write((char*)&type, sizeof(type));
    out.write((char*)&quantity, sizeof(quantity));
}

void Transaction::deserialize(ifstream& in) {
    size_t len;
    
    in.read((char*)&len, sizeof(len));
    transactionId.resize(len);
    in.read(&transactionId[0], len);
    
    in.read((char*)&len, sizeof(len));
    userId.resize(len);
    in.read(&userId[0], len);
    
    in.read((char*)&len, sizeof(len));
    itemId.resize(len);
    in.read(&itemId[0], len);
    
    in.read((char*)&amount, sizeof(amount));
    in.read((char*)&timestamp, sizeof(timestamp));
    in.read((char*)&status, sizeof(status));
    in.read((char*)&type, sizeof(type));
    in.read((char*)&quantity, sizeof(quantity));
}