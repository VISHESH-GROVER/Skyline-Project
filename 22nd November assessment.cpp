#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <thread>  // For std::this_thread::sleep_for

using namespace std;

// ANSI color codes
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string MAGENTA = "\033[35m";

// Function to clear console
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to display the colored logo
void displayLogo() {
    cout << "\n";
    cout << "                                      " << MAGENTA << " _____ _           _       _       " << RESET << "_____                \n";
    cout << "                                     " << MAGENTA << "/ ____| |         | |     | |     " << RESET << "/ ____|               \n";
    cout << "                                    " << MAGENTA << "| (___ | |__   __ _| |_ ___| |__ " << RESET << "| |  __  __ _ _ __ ___  \n";
    cout << "                                     " << MAGENTA << "\\___ \\| '_ \\ / _` | __/ _ \\ '_ \\ " << RESET << "| | |_ |/ _` | '__/ _ \\ \n";
    cout << "                                      " << MAGENTA << "____) | | | | (_| | ||  __/ | | | " << RESET << "|__| | | (_| | | | __/ \n";
    cout << "                                    " << MAGENTA << "|_____/|_| |_|\\__,_|\\__\\___|_| |_|" << RESET << "\\_____|\\__,_|_|  \\___| \n";
    cout << "                                                         " << GREEN << "Cyber Café" << RESET << "                          \n";
    cout << "\n" << GREEN << "Welcome to Skyline Cyber Café!" << RESET << endl;
}

// User class to represent a customer
class User {
public:
    string name;
    string email;
    string password;
    int userID;
    time_t joiningDate;
    vector<pair<time_t, time_t>> sessions; // Start and end time of sessions
    double totalBill = 0.0;

    User(string n, string e, string p, int id)
        : name(n), email(e), password(p), userID(id) {
        joiningDate = time(0); // Current time
    }

    void startSession() {
        sessions.emplace_back(time(0), 0); // Start time
        cout << GREEN << "Session started successfully!" << RESET << endl;
    }

    void endSession(double ratePerHour, double printCost, double scanCost, int prints, int scans) {
        if (!sessions.empty()) {
            sessions.back().second = time(0); // End time
            // Calculate bill for this session
            double duration = difftime(sessions.back().second, sessions.back().first) / 3600.0; // Convert to hours
            double sessionBill = (duration * ratePerHour) + (prints * printCost) + (scans * scanCost);
            totalBill += sessionBill;
            cout << BLUE << "Session ended successfully!" << RESET << endl;
            cout << "Session Duration: " << fixed << setprecision(2) << (duration * 60) << " minutes." << endl;
            cout << "Session Bill: NZD " << fixed << setprecision(2) << sessionBill << endl;
        }
        else {
            cout << RED << "No active session to end." << RESET << endl;
        }
    }
};

// Admin class to manage users
class Admin {
public:
    string email;
    string password;

    Admin(string e, string p) : email(e), password(p) {}

    void viewUsers(const vector<User>& users) {
        cout << BLUE << "---- User List ----" << RESET << endl;
        for (const auto& user : users) {
            cout << "User ID: " << user.userID << ", Name: " << user.name << ", Total Bill: NZD " << user.totalBill << endl;
        }
    }

    void editUser(vector<User>& users, int userID, const string& newName) {
        for (auto& user : users) {
            if (user.userID == userID) {
                user.name = newName;
                cout << GREEN << "User updated: " << newName << RESET << endl;
                return;
            }
        }
        cout << RED << "User not found." << RESET << endl;
    }

    void deleteUser(vector<User>& users, int userID) {
        auto it = remove_if(users.begin(), users.end(), [userID](User& u) { return u.userID == userID; });
        if (it != users.end()) {
            users.erase(it, users.end());
            cout << GREEN << "User deleted." << RESET << endl;
        }
        else {
            cout << RED << "User not found." << RESET << endl;
        }
    }
};

// Main program
int main() {
    vector<User> users;
    vector<Admin> admins = { Admin("admin@example.com", "admin123") }; // Predefined admin
    int userIDCounter = 1;
    double ratePerHour = 5.0, printCost = 0.20, scanCost = 0.30;

    while (true) {
        clearConsole();
        displayLogo();

        cout << YELLOW << "1. Register\n2. Login as User\n3. Login as Admin\n4. Exit\nChoose an option: " << RESET;
        int choice;
        cin >> choice;

        if (choice == 1) {
            string name, email, password;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            users.emplace_back(name, email, password, userIDCounter++);
            cout << GREEN << "User registered successfully! User ID: " << userIDCounter - 1 << RESET << endl;

        }
        else if (choice == 2) {
            string email, password;
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            bool userFound = false;
            for (auto& user : users) {
                if (user.email == email && user.password == password) {
                    userFound = true;
                    cout << CYAN << "Welcome " << user.name << "!" << RESET << endl;
                    user.startSession();

                    // Simulating session activity
                    cout << BLUE << "Simulating session activity for 2 seconds...\n" << RESET;
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                    // Ending session
                    user.endSession(ratePerHour, printCost, scanCost, 5, 2); // Example prints/scans for testing
                    break;
                }
            }
            if (!userFound) {
                cout << RED << "Invalid email or password." << RESET << endl;
            }

        }
        else if (choice == 3) {
            string email, password;
            cout << "Enter Admin Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;

            bool adminFound = false;
            for (auto& admin : admins) {
                if (admin.email == email && admin.password == password) {
                    adminFound = true;
                    cout << CYAN << "Welcome Admin!\n" << RESET;
                    admin.viewUsers(users);
                    break;
                }
            }
            if (!adminFound) {
                cout << RED << "Invalid admin credentials." << RESET << endl;
            }

        }
        else if (choice == 4) {
            cout << RED << "Exiting...\n" << RESET;
            break;
        }
        else {
            cout << RED << "Invalid choice, please try again." << RESET << endl;
        }

        cout << "\nPress Enter to return to the main menu...";
        cin.ignore();
        cin.get();  // Wait for Enter key
    }

    return 0;
}
