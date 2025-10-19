#include <iostream>
#include <string>
#include "user.h"
#include "buyer.h"
#include "seller.h"
#include "bank.h"
#include "onlinestore.h"
#include "datamanager.h"

using namespace std;

void displayMainMenu() {
    cout << "\n=== ONLINE STORE SYSTEM ===" << endl;
    cout << "1. Bank Operations" << endl;
    cout << "2. Buyer Operations" << endl;
    cout << "3. Seller Operations" << endl;
    cout << "4. Store Operations" << endl;
    cout << "5. Save & Exit" << endl;
    cout << "Choice: ";
}

void bankMenu(Bank& bank) {
    int choice;
    do {
        cout << "\n--- BANK MENU ---" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. List All Customers" << endl;
        cout << "3. List Week Transactions" << endl;
        cout << "4. List Dormant Accounts" << endl;
        cout << "5. Top N Active Users Today" << endl;
        cout << "6. Deposit" << endl;
        cout << "7. Withdraw" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        string userId, accountId;
        double amount;
        int n;

        switch(choice) {
            case 1: {
                cout << "User ID: ";
                getline(cin, userId);
                accountId = bank.createAccount(userId);
                cout << "Account created: " << accountId << endl;
                break;
            }
            case 2:
                bank.listAllCustomers();
                break;
            case 3:
                bank.listWeekTransactions();
                break;
            case 4:
                bank.listDormantAccounts();
                break;
            case 5:
                cout << "Number of top users: ";
                cin >> n;
                bank.listTopActiveUsersToday(n);
                break;
            case 6:
                cout << "Account ID: ";
                getline(cin, accountId);
                cout << "Amount: ";
                cin >> amount;
                bank.deposit(accountId, amount);
                break;
            case 7:
                cout << "Account ID: ";
                getline(cin, accountId);
                cout << "Amount: ";
                cin >> amount;
                bank.withdraw(accountId, amount);
                break;
        }
    } while(choice != 0);
}

void buyerMenu(OnlineStore& store, Bank& bank) {
    int choice;
    string userId, password;
    
    do {
        cout << "\n--- BUYER MENU ---" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if(choice == 1) {
            cout << "Username: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);
            
            Buyer* buyer = store.registerBuyer(userId, password);
            if(buyer) {
                cout << "Registration successful!" << endl;
                
                // Create bank account
                string accountId = bank.createAccount(userId);
                buyer->setBankAccountId(accountId);
                cout << "Bank account created: " << accountId << endl;
            }
        }
        else if(choice == 2) {
            cout << "Username: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);
            
            Buyer* buyer = store.loginBuyer(userId, password);
            if(buyer) {
                cout << "Login successful!" << endl;
                
                int subChoice;
                do {
                    cout << "\n--- BUYER ACTIONS ---" << endl;
                    cout << "1. View Items" << endl;
                    cout << "2. Purchase Item" << endl;
                    cout << "3. List My Orders" << endl;
                    cout << "4. Check Balance" << endl;
                    cout << "5. Topup" << endl;
                    cout << "6. Withdraw" << endl;
                    cout << "7. Cash Flow Today" << endl;
                    cout << "8. Cash Flow This Month" << endl;
                    cout << "9. Spending Last K Days" << endl;
                    cout << "0. Logout" << endl;
                    cout << "Choice: ";
                    cin >> subChoice;
                    cin.ignore();

                    string itemId;
                    int quantity, k;
                    double amount;

                    switch(subChoice) {
                        case 1:
                            store.listAvailableItems();
                            break;
                        case 2:
                            cout << "\n=== Available Products ===" << endl;
                            store.listAvailableItems();
                            cout << "\nItem ID: ";
                            getline(cin, itemId);
                            cout << "Quantity: ";
                            cin >> quantity;
                            store.purchaseItem(buyer, itemId, quantity, bank);
                            break;
                        case 3:{
                            cout << "Filter (all/paid/cancelled/completed): ";
                            string filter;
                            getline(cin, filter);
                            buyer->listOrders(filter);
                            break;
                        }
                        case 4:
                            cout << "Balance: $" << bank.getBalance(buyer->getBankAccountId()) << endl;
                            break;
                        case 5:
                            cout << "Amount: ";
                            cin >> amount;
                            bank.deposit(buyer->getBankAccountId(), amount);
                            break;
                        case 6:
                            cout << "Amount: ";
                            cin >> amount;
                            bank.withdraw(buyer->getBankAccountId(), amount);
                            break;
                        case 7:
                            bank.listCashFlowToday(buyer->getBankAccountId());
                            break;
                        case 8:
                            bank.listCashFlowMonth(buyer->getBankAccountId());
                            break;
                        case 9:
                            cout << "Number of days: ";
                            cin >> k;
                            buyer->checkSpendingLastKDays(k);
                            break;
                    }
                } while(subChoice != 0);
            }
        }
    } while(choice != 0);
}

void sellerMenu(OnlineStore& store, Bank& bank) {
    int choice;
    string userId, password;
    
    do {
        cout << "\n--- SELLER MENU ---" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        cin.ignore();

        if(choice == 1) {
            cout << "Username: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);
            
            Seller* seller = store.registerSeller(userId, password);
            if(seller) {
                cout << "Registration successful!" << endl;
                
                string accountId = bank.createAccount(userId);
                seller->setBankAccountId(accountId);
                cout << "Bank account created: " << accountId << endl;
            }
        }
        else if(choice == 2) {
            cout << "Username: ";
            getline(cin, userId);
            cout << "Password: ";
            getline(cin, password);
            
            Seller* seller = store.loginSeller(userId, password);
            if(seller) {
                cout << "Login successful!" << endl;
                
                int subChoice;
                do {
                    cout << "\n--- SELLER ACTIONS ---" << endl;
                    cout << "1. Add New Item" << endl;
                    cout << "2. Update Item" << endl;
                    cout << "3. List My Items" << endl;
                    cout << "4. Top K Popular Items This Month" << endl;
                    cout << "5. Loyal Customers" << endl;
                    cout << "6. My Sales" << endl;
                    cout << "7. Check Balance" << endl;
                    cout << "0. Logout" << endl;
                    cout << "Choice: ";
                    cin >> subChoice;
                    cin.ignore();

                    string name, itemId;
                    int quantity, k;
                    double price;
                    bool display;

                    switch(subChoice) {
                        case 1:
                            cout << "Item Name: ";
                            getline(cin, name);
                            cout << "Quantity: ";
                            cin >> quantity;
                            cout << "Price: ";
                            cin >> price;
                            cout << "Display (1/0): ";
                            cin >> display;
                            seller->addItem(name, quantity, price, display);
                            break;
                        case 2:
                            cout << "Item ID: ";
                            getline(cin, itemId);
                            cout << "New Quantity: ";
                            cin >> quantity;
                            cout << "New Price: ";
                            cin >> price;
                            cout << "Display (1/0): ";
                            cin >> display;
                            seller->updateItem(itemId, quantity, price, display);
                            break;
                        case 3:
                            seller->listItems();
                            break;
                        case 4:
                            cout << "K: ";
                            cin >> k;
                            seller->topKPopularItemsThisMonth(k);
                            break;
                        case 5:
                            seller->listLoyalCustomers();
                            break;
                        case 6:
                            seller->listSales();
                            break;
                        case 7:
                            cout << "Balance: $" << bank.getBalance(seller->getBankAccountId()) << endl;
                            break;
                    }
                } while(subChoice != 0);
            }
        }
    } while(choice != 0);
}

void storeMenu(OnlineStore& store) {
    int choice;
    do {
        cout << "\n--- STORE MENU ---" << endl;
        cout << "1. List Transactions Last K Days" << endl;
        cout << "2. List Paid But Incomplete" << endl;
        cout << "3. Most M Frequent Items" << endl;
        cout << "4. Most Active Buyers" << endl;
        cout << "5. Most Active Sellers Today" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        
        int k, m, n;
        switch(choice) {
            case 1:
                cout << "K days: ";
                cin >> k;
                store.listTransactionsLastKDays(k);
                break;
            case 2:
                store.listPaidButIncomplete();
                break;
            case 3:
                cout << "M items: ";
                cin >> m;
                store.listMostMFrequentItems(m);
                break;
            case 4:
                store.listMostActiveBuyers();
                break;
            case 5:
                store.listMostActiveSellersToday();
                break;
        }
    } while(choice != 0);
}

int main() {
    DataManager dataManager;
    
    // Load data
    Bank bank = dataManager.loadBank();
    OnlineStore store = dataManager.loadStore();
    
    int choice;
    do {
        displayMainMenu();
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                bankMenu(bank);
                break;
            case 2:
                buyerMenu(store, bank);
                break;
            case 3:
                sellerMenu(store, bank);
                break;
            case 4:
                storeMenu(store);
                break;
            case 5:
                dataManager.saveBank(bank);
                dataManager.saveStore(store);
                cout << "Data saved. Goodbye!" << endl;
                break;
        }
    } while(choice != 5);

    return 0;
}