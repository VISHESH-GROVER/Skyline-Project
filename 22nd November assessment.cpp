#include <iostream>

#include <fstream>

#include <vector>

#include <conio.h>

#include <string>

using namespace std;

struct UserAdminRegistration {

    string username;

    string password;

    string confirm_password;

    string fullname;

    string email;

    string confirm_email;

    string phoneno;

};

string getHiddenPassword() {

    string password;

    char ch;

    while (true) {

        ch = _getch();

        if (ch == 13) {

            cout << endl;

            break;

        }

        else if (ch == 8 && !password.empty()) {

            password.pop_back();

            cout << "\b \b";

        }

        else if (ch != 8) {

            password += ch;

            cout << '*';

        }

    }

    return password;

}

bool isValidUserName(const string& username) {

    if (username.length() < 6 || username.length() > 20) {

        return false;

    }

    if (!isalpha(username[0])) {

        return false;

    }

    for (int i = 0; i < username.size(); i++) {

        char ch = username[i];

        if (!(isalnum(ch) || ch == '@' || ch == '.' || ch == '-' || ch == '_')) {

            return false;

        }

    }

    return true;

}

bool isValidPassword(const string& password) {

    if (password.length() < 8 || password.length() > 20) {

        return false;

    }

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;

    for (char c : password) {

        if (c >= 'A' && c <= 'Z') hasUpper = true;

        else if (c >= 'a' && c <= 'z') hasLower = true;

        else if (c >= '0' && c <= '9') hasDigit = true;

        else if (!(c == ' ' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {

            hasSpecial = true;

        }

    }

    return hasUpper && hasLower && hasDigit && hasSpecial;

}

bool isValidEmail(const string& email) {

    vector<string> domain{

        "gmail.com", "outlook.com", "example.com", "test.com", "hotmail.com",

        "live.com", "yahoo.com", "icloud.com", "me.com", "mac.com",

        "protonmail.com", "protonmail.ch", "zoho.com", "aol.com", "gmx.com",

        "gmx.de", "gmx.net", "mail.com", "usa.com", "email.com", "consultant.com",

        "musician.org", "photographer.com", "yandex.com", "yandex.ru",

        "fastmail.com", "fastmail.fm", "tutanota.com", "tutanota.de",

        "posteo.de", "runbox.com", "kolabnow.com", "hushmail.com",

        "mail.ru", "list.ru"

    };

    vector<char> specialChars{

        '.', '-', '_', '+', '%', '&', '\'', '"', '!', '#', '$', '^', '*',

        '(', ')', ',', ':', ';', '=', '~', '`', '/', '?', '<', '>',

        '[', ']', '{', '}'

    };

    size_t atpos = email.find('@');

    size_t dotpos = email.find('.');

    if (atpos == string::npos || dotpos == string::npos ||

        email.find(' ') != string::npos ||

        email.find('@', atpos + 1) != string::npos ||

        email.find('.', dotpos + 1) != string::npos ||

        email.front() == '@' || email.front() == '.' ||

        email.back() == '@' || email.back() == '.' ||

        email.length() > 254)

    {

        return false;

    }

    for (char c : specialChars) {

        if (email.front() == c || email.back() == c) {

            return false;

        }

    }

    for (const string& validdomain : domain) {

        if (email.find(validdomain) != string::npos) {

            return true;

        }

    }

    return false;

}

bool isValidPhoneNumber(const string& phone) {

    string cleanNumber;

    for (char c : phone) {

        if (isdigit(c)) {

            cleanNumber += c;

        }

    }

    for (char c : phone) {

        if (!isdigit(c) && c != ' ' && c != '-' && c != '+' && c != '(' && c != ')') {

            return false;

        }

    }

    if (cleanNumber.length() < 10 || cleanNumber.length() > 13) {

        return false;

    }

    for (char c : cleanNumber) {

        if (!isdigit(c)) {

            return false;

        }

    }

    return true;

}

void writeUserToJson(const UserAdminRegistration& user, const string& role) {

    const string filename = "User_Registration.json";

    ifstream readFile(filename);

    bool isNewFile = !readFile.good();

    string content;

    if (isNewFile) {

        readFile.close();

        ofstream newFile(filename);

        newFile << "{\n  \"users\": [\n    {\n";

        newFile << "      \"role\": \"" << role << "\",\n";

        newFile << "      \"username\": \"" << user.username << "\",\n";

        newFile << "      \"password\": \"" << user.password << "\",\n";

        newFile << "      \"fullname\": \"" << user.fullname << "\",\n";

        newFile << "      \"email\": \"" << user.email << "\",\n";

        newFile << "      \"phoneno\": " << user.phoneno << "\n";

        newFile << "    }";

        newFile << "\n  ]\n}";

        newFile.close();

    }

    else {

        string line;

        while (getline(readFile, line)) {

            content += line + "\n";

        }

        readFile.close();

        size_t pos = content.rfind("\n  ]\n}");

        if (pos != string::npos) {

            content = content.substr(0, pos);

        }

        ofstream writeFile(filename);

        writeFile << content;

        writeFile << ",\n    {\n";

        writeFile << "      \"role\": \"" << role << "\",\n";

        writeFile << "      \"username\": \"" << user.username << "\",\n";

        writeFile << "      \"password\": \"" << user.password << "\",\n";

        writeFile << "      \"fullname\": \"" << user.fullname << "\",\n";

        writeFile << "      \"email\": \"" << user.email << "\",\n";

        writeFile << "      \"phoneno\": " << user.phoneno << "\n";

        writeFile << "    }";

        writeFile << "\n  ]\n}";

        writeFile.close();

    }

    cout << "\nRegistration successful! Data saved to " << filename << endl;

}

void registerUserOrAdmin(const string& role) {

    UserAdminRegistration userReg{};

    cout << "Enter the Username: ";

    getline(cin, userReg.username);

    while (!isValidUserName(userReg.username)) {

        cout << "HINT: more than 6 character | must start with alphabet | (@ . - _) only 4 special character is used: " << endl;

        cout << "Enter the Username: ";

        getline(cin, userReg.username);

    }

    while (true) {

        cout << "Enter your Password (min 8 characters): ";

        userReg.password = getHiddenPassword();

        while (!isValidPassword(userReg.password)) {

            cout << "Password must be at least 8 characters long, with 1 uppercase letter, 1 lowercase letter,\n"
                << "at least 1 number and 1 special character.\n";

            cout << "Enter your Password again: ";

            userReg.password = getHiddenPassword();

        }

        cout << "Confirm your Password: ";

        userReg.confirm_password = getHiddenPassword();

        if (userReg.password == userReg.confirm_password) {

            break;

        }

        cout << "Passwords do not match. Please try again.\n";

    }

    cout << "Enter the Full name: ";

    getline(cin, userReg.fullname);

    // Modified email confirmation loop

    while (true) {

        cout << "Enter your Email Address: ";

        getline(cin, userReg.email);

        while (!isValidEmail(userReg.email)) {

            cout << "Invalid email format. Please enter a valid email: ";

            getline(cin, userReg.email);

        }

        cout << "Confirm your Email Address: ";

        getline(cin, userReg.confirm_email);

        if (userReg.email == userReg.confirm_email) {

            break;  // Break the loop only if emails match

        }

        cout << "Emails do not match. Please try again.\n";

    }

    cout << "Enter Your Mobile number: ";

    getline(cin, userReg.phoneno);

    while (!isValidPhoneNumber(userReg.phoneno)) {

        cout << "Invalid input. Please enter a valid mobile number: ";

        getline(cin, userReg.phoneno);

    }

    writeUserToJson(userReg, role);

}

void showRegistrationMenu() {

    cout << "\n=== Registration Menu ===" << endl;

    cout << "1. User Registration" << endl;

    cout << "2. Exit" << endl;

}

int main() {
    

    while (true) {

        showRegistrationMenu();

        int choice;

        cout << "Enter your choice (1, 2): ";

        cin >> choice;

        cin.ignore();

        switch (choice) {

        case 1:

            cout << string(50, '*') << endl;

            cout << "\t\tUser Registration" << endl;

            cout << string(50, '*') << endl;

            registerUserOrAdmin("User");

            break;

        case 2:

            cout << "Exiting..." << endl;

            return 0;

        default:

            cout << "Invalid choice. Please try again." << endl;

        }

    }

    return 0;

}
