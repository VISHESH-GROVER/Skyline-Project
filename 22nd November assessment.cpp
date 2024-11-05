#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <limits>

using namespace std;

struct UserAdminRegistration {
    string username;
    string password;
    string fullname;
    string email;
    int phoneno;
};

bool isValidEmail(const string& email) {
    const regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

bool isValidPassword(const string& password) {
    return password.length() >= 8;
}

void writeUserToJson(const UserAdminRegistration& user, const string& role) {
    const string filename = "User_Registration.json";
    ifstream readFile(filename);
    bool isNewFile = !readFile.good();
    string content;

    if (isNewFile) {
        readFile.close();
        ofstream newFile(filename);
        // Start new JSON with proper formatting
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
        // Read existing content
        string line;
        while (getline(readFile, line)) {
            content += line + "\n";
        }
        readFile.close();

        // Remove the closing brackets
        size_t pos = content.rfind("\n  ]\n}");
        if (pos != string::npos) {
            content = content.substr(0, pos);
        }

        // Add the new user and close the JSON structure
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

// Rest of the code remains the same
void registerUserOrAdmin(const string& role) {
    UserAdminRegistration userReg{};

    cout << "Enter the Username: ";
    getline(cin, userReg.username);

    cout << "Enter your Password (min 8 characters): ";
    getline(cin, userReg.password);
    while (!isValidPassword(userReg.password)) {
        cout << "Password must be at least 8 characters. Please enter a valid password: ";
        getline(cin, userReg.password);
    }

    cout << "Enter the Full name: ";
    getline(cin, userReg.fullname);

    cout << "Enter your Email Address: ";
    getline(cin, userReg.email);
    while (!isValidEmail(userReg.email)) {
        cout << "Invalid email format. Please enter a valid email: ";
        getline(cin, userReg.email);
    }

    cout << "Enter Your Mobile number: ";
    while (!(cin >> userReg.phoneno)) {
        cout << "Invalid input. Please enter a valid mobile number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();

    writeUserToJson(userReg, role);
}

void showRegistrationMenu() {
    cout << "\n=== Registration Menu ===" << endl;
    cout << "1. User Registration" << endl;
    /* cout << "2. Admin Registration" << endl;*/
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
            cout << "User Registration" << endl;
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