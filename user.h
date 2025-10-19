#ifndef USER_H
#define USER_H

#include <string>
#include <fstream>

using namespace std;

class User {
protected:
    string userId;
    string password;
    string bankAccountId;

public:
    User();
    User(const string& uid, const string& pass);
    virtual ~User();

    string getUserId() const;
    string getPassword() const;
    string getBankAccountId() const;
    
    void setBankAccountId(const string& accountId);
    
    virtual void serialize(ofstream& out) const;
    virtual void deserialize(ifstream& in);
};

#endif