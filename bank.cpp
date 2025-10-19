#include "bank.h"
#include <iostream>
#include <algorithm>

// BankAccount Implementation
BankAccount::BankAccount() : accountId(""), userId(""), balance(0.0) {}

BankAccount::BankAccount(const string& accId, const string& uid)
    : accountId(accId), userId(uid), balance(0.0) {}

string BankAccount::getAccountId() const {
    return accountId;
}

string BankAccount::getUserId() const {
    return userId;
}

double BankAccount::getBalance() const {
    return balance;
}

vector<Transaction> BankAccount::getTransactions() const {
    return transactions;
}

void BankAccount::deposit(double amount) {
    balance += amount;
    Transaction trans("T" + to_string(time(nullptr)), userId, "", amount, DEPOSIT);
    transactions.push_back(trans);
}

bool BankAccount::withdraw(double amount) {
    if(balance >= amount) {
        balance -= amount;
        Transaction trans("T" + to_string(time(nullptr)), userId, "", amount, WITHDRAW);
        transactions.push_back(trans);
        return true;
    }
    return false;
}

void BankAccount::addTransaction(const Transaction& trans) {
    transactions.push_back(trans);
}

void BankAccount::serialize(ofstream& out) const {
    size_t len = accountId.length();
    out.write((char*)&len, sizeof(len));
    out.write(accountId.c_str(), len);
    
    len = userId.length();
    out.write((char*)&len, sizeof(len));
    out.write(userId.c_str(), len);
    
    out.write((char*)&balance, sizeof(balance));
    
    size_t transCount = transactions.size();
    out.write((char*)&transCount, sizeof(transCount));
    for(const auto& trans : transactions) {
        trans.serialize(out);
    }
}

void BankAccount::deserialize(ifstream& in) {
    size_t len;
    
    in.read((char*)&len, sizeof(len));
    accountId.resize(len);
    in.read(&accountId[0], len);
    
    in.read((char*)&len, sizeof(len));
    userId.resize(len);
    in.read(&userId[0], len);
    
    in.read((char*)&balance, sizeof(balance));
    
    size_t transCount;
    in.read((char*)&transCount, sizeof(transCount));
    transactions.clear();
    for(size_t i = 0; i < transCount; ++i) {
        Transaction trans;
        trans.deserialize(in);
        transactions.push_back(trans);
    }
}

// Bank Implementation
Bank::Bank() : accountCounter(1000) {}

string Bank::createAccount(const string& userId) {
    string accountId = "ACC" + to_string(accountCounter++);
    accounts[accountId] = BankAccount(accountId, userId);
    cout << "Bank account created: " << accountId << endl;
    return accountId;
}

BankAccount* Bank::getAccount(const string& accountId) {
    auto it = accounts.find(accountId);
    if(it != accounts.end()) {
        return &(it->second);
    }
    return nullptr;
}

void Bank::deposit(const string& accountId, double amount) {
    BankAccount* acc = getAccount(accountId);
    if(acc) {
        acc->deposit(amount);
        cout << "Deposited $" << amount << ". New balance: $" << acc->getBalance() << endl;
    } else {
        cout << "Account not found!" << endl;
    }
}

bool Bank::withdraw(const string& accountId, double amount) {
    BankAccount* acc = getAccount(accountId);
    if(acc) {
        if(acc->withdraw(amount)) {
            cout << "Withdrawn $" << amount << ". New balance: $" << acc->getBalance() << endl;
            return true;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    } else {
        cout << "Account not found!" << endl;
    }
    return false;
}

double Bank::getBalance(const string& accountId) {
    BankAccount* acc = getAccount(accountId);
    if(acc) {
        return acc->getBalance();
    }
    return 0.0;
}

void Bank::listAllCustomers() const {
    cout << "\n--- All Bank Customers ---" << endl;
    for(const auto& pair : accounts) {
        cout << "Account: " << pair.first << " | User: " << pair.second.getUserId()
             << " | Balance: $" << pair.second.getBalance() << endl;
    }
}

void Bank::listWeekTransactions() const {
    cout << "\n--- Week Transactions ---" << endl;
    for(const auto& pair : accounts) {
        vector<Transaction> trans = pair.second.getTransactions();
        for(const auto& t : trans) {
            if(t.isWithinDays(7)) {
                cout << "Account: " << pair.first << " | Amount: $" << t.getAmount()
                     << " | Type: " << (t.getType() == DEPOSIT ? "Deposit" : "Withdraw") << endl;
            }
        }
    }
}

void Bank::listDormantAccounts() const {
    cout << "\n--- Dormant Accounts (No transaction in 30 days) ---" << endl;
    for(const auto& pair : accounts) {
        vector<Transaction> trans = pair.second.getTransactions();
        bool hasRecentTrans = false;
        for(const auto& t : trans) {
            if(t.isWithinDays(30)) {
                hasRecentTrans = true;
                break;
            }
        }
        if(!hasRecentTrans) {
            cout << "Account: " << pair.first << " | User: " << pair.second.getUserId() << endl;
        }
    }
}

void Bank::listTopActiveUsersToday(int n) const {
    cout << "\n--- Top " << n << " Active Users Today ---" << endl;
    
    map<string, int> userTransCount;
    for(const auto& pair : accounts) {
        int count = 0;
        vector<Transaction> trans = pair.second.getTransactions();
        for(const auto& t : trans) {
            if(t.isToday()) {
                count++;
            }
        }
        if(count > 0) {
            userTransCount[pair.second.getUserId()] = count;
        }
    }
    
    vector<pair<string, int>> sorted(userTransCount.begin(), userTransCount.end());
    sort(sorted.begin(), sorted.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    int count = 0;
    for(const auto& user : sorted) {
        if(count >= n) break;
        cout << "User: " << user.first << " | Transactions: " << user.second << endl;
        count++;
    }
}

void Bank::listCashFlowToday(const string& accountId) const {
    BankAccount* acc = const_cast<Bank*>(this)->getAccount(accountId);
    if(!acc) {
        cout << "Account not found!" << endl;
        return;
    }
    
    cout << "\n--- Cash Flow Today ---" << endl;
    double credit = 0, debit = 0;
    vector<Transaction> trans = acc->getTransactions();
    for(const auto& t : trans) {
        if(t.isToday()) {
            if(t.getType() == DEPOSIT) {
                credit += t.getAmount();
            } else {
                debit += t.getAmount();
            }
        }
    }
    cout << "Credit: $" << credit << " | Debit: $" << debit << endl;
}

void Bank::listCashFlowMonth(const string& accountId) const {
    BankAccount* acc = const_cast<Bank*>(this)->getAccount(accountId);
    if(!acc) {
        cout << "Account not found!" << endl;
        return;
    }
    
    cout << "\n--- Cash Flow This Month ---" << endl;
    double credit = 0, debit = 0;
    vector<Transaction> trans = acc->getTransactions();
    for(const auto& t : trans) {
        if(t.isWithinMonth()) {
            if(t.getType() == DEPOSIT) {
                credit += t.getAmount();
            } else {
                debit += t.getAmount();
            }
        }
    }
    cout << "Credit: $" << credit << " | Debit: $" << debit << endl;
}

void Bank::serialize(ofstream& out) const {
    out.write((char*)&accountCounter, sizeof(accountCounter));
    
    size_t accountCount = accounts.size();
    out.write((char*)&accountCount, sizeof(accountCount));
    
    for(const auto& pair : accounts) {
        pair.second.serialize(out);
    }
}

void Bank::deserialize(ifstream& in) {
    in.read((char*)&accountCounter, sizeof(accountCounter));
    
    size_t accountCount;
    in.read((char*)&accountCount, sizeof(accountCount));
    
    accounts.clear();
    for(size_t i = 0; i < accountCount; ++i) {
        BankAccount acc;
        acc.deserialize(in);
        accounts[acc.getAccountId()] = acc;
    }
}