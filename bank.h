#ifndef BANK_H
#define BANK_H

#include <string>
#include <map>
#include <vector>
#include "transaction.h"

using namespace std;

class BankAccount {
private:
    string accountId;
    string userId;
    double balance;
    vector<Transaction> transactions;

public:
    BankAccount();
    BankAccount(const string& accId, const string& uid);
    
    string getAccountId() const;
    string getUserId() const;
    double getBalance() const;
    vector<Transaction> getTransactions() const;
    
    void deposit(double amount);
    bool withdraw(double amount);
    void addTransaction(const Transaction& trans);
    
    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
};

class Bank {
private:
    map<string, BankAccount> accounts;
    int accountCounter;

public:
    Bank();
    
    string createAccount(const string& userId);
    BankAccount* getAccount(const string& accountId);
    
    void deposit(const string& accountId, double amount);
    bool withdraw(const string& accountId, double amount);
    double getBalance(const string& accountId);
    
    void listAllCustomers() const;
    void listWeekTransactions() const;
    void listDormantAccounts() const;
    void listTopActiveUsersToday(int n) const;
    void listCashFlowToday(const string& accountId) const;
    void listCashFlowMonth(const string& accountId) const;
    
    void serialize(ofstream& out) const;
    void deserialize(ifstream& in);
};

#endif