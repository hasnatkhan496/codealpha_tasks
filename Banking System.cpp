#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

const int MAX_TXN = 20;
const int MAX_ACC = 5;
const int MAX_CUST = 10;

// --- Transaction Class -------------------------------------------
class Transaction {
    string type, date;
    double amount;
public:
    void set(string t, double a, string d) { type = t; amount = a; date = d; }
    void display() const {
        cout << left << setw(14) << date << setw(14) << type << "$" << fixed << setprecision(2) << amount << "\n";
    }
};

// --- Account Class -----------------------------------------------
class Account {
    int accNo;
    string accType;
    double balance;
    Transaction history[MAX_TXN];
    int txnCount;

    void addTxn(string type, double amount) {
        if (txnCount < MAX_TXN)
            history[txnCount++].set(type, amount, "2025-06-01");
    }
public:
    Account() : accNo(0), balance(0), txnCount(0) {}

    void create(int no, string type, double initDeposit) {
        accNo = no; accType = type; balance = initDeposit; txnCount = 0;
        addTxn("Opening", initDeposit);
    }

    bool deposit(double amt) {
        if (amt <= 0) { cout << "  [!] Invalid amount.\n"; return false; }
        balance += amt; addTxn("Deposit", amt);
        cout << "  [+] Deposited $" << fixed << setprecision(2) << amt << ". Balance: $" << balance << "\n";
        return true;
    }

    bool withdraw(double amt) {
        if (amt <= 0 || amt > balance) { cout << "  [!] Invalid/Insufficient funds.\n"; return false; }
        balance -= amt; addTxn("Withdrawal", amt);
        cout << "  [-] Withdrew $" << fixed << setprecision(2) << amt << ". Balance: $" << balance << "\n";
        return true;
    }

    void showHistory() const {
        cout << "\n  -- Transaction History (Acc #" << accNo << ") --\n";
        cout << left << setw(14) << "Date" << setw(14) << "Type" << "Amount\n";
        cout << string(40, '-') << "\n";
        for (int i = 0; i < txnCount; i++) history[i].display();
    }

    void showInfo() const {
        cout << "  Account No   : " << accNo << "\n"
             << "  Type         : " << accType << "\n"
             << "  Balance      : $" << fixed << setprecision(2) << balance << "\n";
    }

    int    getAccNo()  const { return accNo; }
    double getBalance() const { return balance; }
};

// --- Customer Class ----------------------------------------------
class Customer {
    int id;
    string name, phone;
    Account accounts[MAX_ACC];
    int accCount;
public:
    Customer() : id(0), accCount(0) {}

    void create(int custId) {
        id = custId; accCount = 0;
        cout << "  Name  : "; cin.ignore(); getline(cin, name);
        cout << "  Phone : "; getline(cin, phone);
    }

    void addAccount() {
        if (accCount >= MAX_ACC) { cout << "  [!] Account limit reached.\n"; return; }
        string type; double deposit;
        cout << "  Account Type (Saving/Current): "; cin >> type;
        cout << "  Initial Deposit: $"; cin >> deposit;
        int accNo = id * 100 + accCount + 1;
        accounts[accCount++].create(accNo, type, deposit);
        cout << "  [+] Account #" << accNo << " created.\n";
    }

    Account* findAccount(int accNo) {
        for (int i = 0; i < accCount; i++)
            if (accounts[i].getAccNo() == accNo) return &accounts[i];
        return NULL;
    }

    void showInfo() const {
        cout << "\n  Customer ID  : " << id << "\n"
             << "  Name         : " << name << "\n"
             << "  Phone        : " << phone << "\n"
             << "  Accounts     : " << accCount << "\n";
        for (int i = 0; i < accCount; i++) { cout << "  "; accounts[i].showInfo(); }
    }

    int getID() const { return id; }
    string getName() const { return name; }
};

// --- Bank Class --------------------------------------------------
class Bank {
    Customer customers[MAX_CUST];
    int custCount = 0;

    Customer* findCustomer(int id) {
        for (int i = 0; i < custCount; i++)
            if (customers[i].getID() == id) return &customers[i];
        return NULL;
    }
public:
    void createCustomer() {
        if (custCount >= MAX_CUST) { cout << "  [!] Customer limit reached.\n"; return; }
        cout << "\n-- New Customer --\n";
        customers[custCount].create(1001 + custCount);
        cout << "  [+] Customer ID: " << 1001 + custCount << " created.\n";
        custCount++;
    }

    void addAccount() {
        int id; cout << "\nEnter Customer ID: "; cin >> id;
        Customer* c = findCustomer(id);
        if (!c) { cout << "  [!] Customer not found.\n"; return; }
        c->addAccount();
    }

    void deposit() {
        int id, accNo; double amt;
        cout << "\nCustomer ID: "; cin >> id;
        Customer* c = findCustomer(id);
        if (!c) { cout << "  [!] Not found.\n"; return; }
        cout << "Account No : "; cin >> accNo;
        Account* a = c->findAccount(accNo);
        if (!a) { cout << "  [!] Account not found.\n"; return; }
        cout << "Amount     : $"; cin >> amt;
        a->deposit(amt);
    }

    void withdraw() {
        int id, accNo; double amt;
        cout << "\nCustomer ID: "; cin >> id;
        Customer* c = findCustomer(id);
        if (!c) { cout << "  [!] Not found.\n"; return; }
        cout << "Account No : "; cin >> accNo;
        Account* a = c->findAccount(accNo);
        if (!a) { cout << "  [!] Account not found.\n"; return; }
        cout << "Amount     : $"; cin >> amt;
        a->withdraw(amt);
    }

    void transfer() {
        int id1, acc1, id2, acc2; double amt;
        cout << "\nFrom - Customer ID: "; cin >> id1;
        cout << "From - Account No : "; cin >> acc1;
        cout << "To   - Customer ID: "; cin >> id2;
        cout << "To   - Account No : "; cin >> acc2;
        cout << "Amount            : $"; cin >> amt;

        Customer *c1 = findCustomer(id1), *c2 = findCustomer(id2);
        if (!c1 || !c2) { cout << "  [!] Customer not found.\n"; return; }
        Account *a1 = c1->findAccount(acc1), *a2 = c2->findAccount(acc2);
        if (!a1 || !a2) { cout << "  [!] Account not found.\n"; return; }
        if (a1->withdraw(amt)) { a2->deposit(amt); cout << "  [?] Transfer successful.\n"; }
    }

    void viewTransactions() {
        int id, accNo;
        cout << "\nCustomer ID: "; cin >> id;
        Customer* c = findCustomer(id);
        if (!c) { cout << "  [!] Not found.\n"; return; }
        cout << "Account No : "; cin >> accNo;
        Account* a = c->findAccount(accNo);
        if (!a) { cout << "  [!] Account not found.\n"; return; }
        a->showHistory();
    }

    void viewCustomer() {
        int id; cout << "\nCustomer ID: "; cin >> id;
        Customer* c = findCustomer(id);
        if (c) c->showInfo();
        else cout << "  [!] Not found.\n";
    }

    void menu() {
        int choice;
        do {
            cout << "\n====== BANKING SYSTEM ======\n"
                 << "1. New Customer\n2. Add Account\n3. Deposit\n"
                 << "4. Withdraw\n5. Transfer\n6. Transactions\n"
                 << "7. Customer Info\n0. Exit\n"
                 << "Choice: ";
            cin >> choice;
            switch (choice) {
                case 1: createCustomer();   break;
                case 2: addAccount();       break;
                case 3: deposit();          break;
                case 4: withdraw();         break;
                case 5: transfer();         break;
                case 6: viewTransactions(); break;
                case 7: viewCustomer();     break;
                case 0: cout << "  Goodbye!\n"; break;
                default: cout << "  [!] Invalid choice.\n";
            }
        } while (choice != 0);
    }
};

// --- Main --------------------------------------------------------
int main() {
    Bank bank;
    bank.menu();
}
