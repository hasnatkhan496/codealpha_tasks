#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <conio.h>
using namespace std;

const string DB = "users.txt";

// --- Hash Function (simple but effective for beginners) ----------
string hashPass(const string& pass) {
    unsigned long h = 5381;
    for (char c : pass) h = ((h << 5) + h) + c;
    return to_string(h);
}

// --- Validator Class ---------------------------------------------
class Validator {
public:
    static bool validUsername(const string& u) {
        if (u.length() < 4) { cout << "  [!] Username must be at least 4 characters.\n"; return false; }
        for (char c : u)
            if (!isalnum(c) && c != '_') { cout << "  [!] Only letters, digits, underscores allowed.\n"; return false; }
        return true;
    }
    static bool validPassword(const string& p) {
        if (p.length() < 6) { cout << "  [!] Password must be at least 6 characters.\n"; return false; }
        bool hasUpper = false, hasDigit = false;
        for (char c : p) {
            if (isupper(c)) hasUpper = true;
            if (isdigit(c)) hasDigit = true;
        }
        if (!hasUpper) { cout << "  [!] Password must contain at least one uppercase letter.\n"; return false; }
        if (!hasDigit) { cout << "  [!] Password must contain at least one digit.\n"; return false; }
        return true;
    }
};

// --- User Class --------------------------------------------------
class User {
    string username, passHash;
public:
    User() {}
    User(string u, string p) : username(u), passHash(hashPass(p)) {}

    string getUsername() const { return username; }
    bool checkPassword(const string& p) const { return passHash == hashPass(p); }

    // Serialize to file line
    string serialize() const { return username + ":" + passHash; }

    // Deserialize from file line
    static User deserialize(const string& line) {
        User u;
        size_t pos = line.find(':');
        u.username = line.substr(0, pos);
        u.passHash = line.substr(pos + 1);
        return u;
    }
};

// --- UserDB Class (File-based storage) ---------------------------
class UserDB {
public:
    static bool exists(const string& username) {
        ifstream file(DB);
        string line;
        while (getline(file, line)) {
            User u = User::deserialize(line);
            if (u.getUsername() == username) return true;
        }
        return false;
    }

    static bool save(const User& user) {
        ofstream file(DB, ios::app);
        if (!file) { cout << "  [!] Could not open database.\n"; return false; }
        file << user.serialize() << "\n";
        return true;
    }

    static bool verify(const string& username, const string& password) {
        ifstream file(DB);
        string line;
        while (getline(file, line)) {
            User u = User::deserialize(line);
            if (u.getUsername() == username && u.checkPassword(password)) return true;
        }
        return false;
    }
};

// --- Auth Class --------------------------------------------------
class Auth {
    string readPassword(const string& prompt) {
        string pass; cout << prompt;
        // Hide password input
        #ifdef _WIN32
            char c;
            while ((c = _getch()) != '\r') {
                if (c == '\b') { if (!pass.empty()) { pass.pop_back(); cout << "\b \b"; } }
                else { pass += c; cout << '*'; }
            }
            cout << "\n";
        #else
            // On Linux/Mac, basic hidden input
            system("stty -echo");
            getline(cin, pass);
            system("stty echo");
            cout << "\n";
        #endif
        return pass;
    }
public:
    void registerUser() {
        string username, pass, confirm;
        cout << "\n--- Register ---\n";
        cout << "  Username : "; cin >> username;

        if (!Validator::validUsername(username)) return;
        if (UserDB::exists(username)) { cout << "  [!] Username already taken.\n"; return; }

        cin.ignore();
        pass    = readPassword("  Password : ");
        confirm = readPassword("  Confirm  : ");

        if (pass != confirm) { cout << "  [!] Passwords do not match.\n"; return; }
        if (!Validator::validPassword(pass)) return;

        User newUser(username, pass);
        if (UserDB::save(newUser))
            cout << "  [?] Registration successful! Welcome, " << username << ".\n";
    }

    void loginUser() {
        string username, pass;
        cout << "\n--- Login ---\n";
        cout << "  Username : "; cin >> username;
        cin.ignore();
        pass = readPassword("  Password : ");

        if (UserDB::verify(username, pass))
            cout << "  [?] Login successful! Welcome back, " << username << ".\n";
        else
            cout << "  [?] Invalid username or password.\n";
    }
};

// --- Main --------------------------------------------------------
int main() {
    Auth auth;
    int choice;
    do {
        cout << "\n====== LOGIN & REGISTRATION ======\n"
             << "1. Register\n2. Login\n0. Exit\n"
             << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: auth.registerUser(); break;
            case 2: auth.loginUser();    break;
            case 0: cout << "  Goodbye!\n"; break;
            default: cout << "  [!] Invalid choice.\n";
        }
    } while (choice != 0);
}
