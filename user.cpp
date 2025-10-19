#include "user.h"

User::User() : userId(""), password(""), bankAccountId("") {}

User::User(const string& uid, const string& pass) 
    : userId(uid), password(pass), bankAccountId("") {}

User::~User() {}

string User::getUserId() const {
    return userId;
}

string User::getPassword() const {
    return password;
}

string User::getBankAccountId() const {
    return bankAccountId;
}

void User::setBankAccountId(const string& accountId) {
    bankAccountId = accountId;
}

void User::serialize(ofstream& out) const {
    size_t len = userId.length();
    out.write((char*)&len, sizeof(len));
    out.write(userId.c_str(), len);
    
    len = password.length();
    out.write((char*)&len, sizeof(len));
    out.write(password.c_str(), len);
    
    len = bankAccountId.length();
    out.write((char*)&len, sizeof(len));
    out.write(bankAccountId.c_str(), len);
}

void User::deserialize(ifstream& in) {
    size_t len;
    
    in.read((char*)&len, sizeof(len));
    userId.resize(len);
    in.read(&userId[0], len);
    
    in.read((char*)&len, sizeof(len));
    password.resize(len);
    in.read(&password[0], len);
    
    in.read((char*)&len, sizeof(len));
    bankAccountId.resize(len);
    in.read(&bankAccountId[0], len);
}