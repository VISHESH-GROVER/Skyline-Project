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
    cout << "                                                         " << GREEN << "Cyber Cafe" << RESET << "                          \n";
    cout << "\n" << GREEN << "Welcome to Skyline Cyber Cafe!" << RESET << endl;
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

    void endSession(double sessionBill) {
        totalBill += sessionBill;
        cout << BLUE << "Session ended successfully!" << RESET << endl;
        cout << "Session Bill: NZD " << fixed << setprecision(2) << sessionBill << endl;
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
    double browsingRate = 0.50, gamingRate = 1.2, printCost = 0.70, scanCost = 0.52;

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
        else if (choice == 2) { // User login
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

                    // Loop to display options after successful login
                 // Inside the user login section after successful login
                    int userChoice;
                    do {
                        cout << YELLOW << "\n--- User Menu ---\n" << RESET;
                        cout << "1. INTERNET BROWSING\n";
                        cout << "2. GAMING\n";
                        cout << "3. PRINTING\n";
                        cout << "4. SCANNING\n";
                        cout << "5. View Total Bill\n";          // Added this option
                        cout << "6. Return to Main Menu\n";      // Changed to be 6
                        cout << "7. Exit Program\n";              // Changed to be 7
                        cout << "Choose an option: ";
                        cin >> userChoice;

                        switch (userChoice) {
                        case 1: {
                            cout << GREEN << "You can use this service for maximum 1 hour. The rate is $0.50 per minute.\n" << RESET;
                            int minutes;
                            cout << "Enter the number of minutes you used: ";
                            cin >> minutes;
                            if (minutes > 60) minutes = 60;  // Max 1 hour
                            double sessionBill = minutes * browsingRate;
                            user.endSession(sessionBill);
                            break;
                        }
                        case 2: {
                            cout << GREEN << "You can play for maximum 1 hour. The rate is $1.20 per minute.\n" << RESET;
                            int minutes;
                            cout << "Enter the number of minutes you played: ";
                            cin >> minutes;
                            if (minutes > 60) minutes = 60;  // Max 1 hour
                            double sessionBill = minutes * gamingRate;
                            user.endSession(sessionBill);
                            break;
                        }
                        case 3: {
                            int prints;
                            cout << "Enter the number of pages to print: ";
                            cin >> prints;
                            double printBill = prints * printCost;
                            user.endSession(printBill);
                            break;
                        }
                        case 4: {
                            int scans;
                            cout << "Enter the number of pages to scan: ";
                            cin >> scans;
                            double scanBill = scans * scanCost;
                            user.endSession(scanBill);
                            break;
                        }
                        case 5: {
                            cout << GREEN << "Your total bill is: NZD " << fixed << setprecision(2) << user.totalBill << RESET << endl; // Display total bill
                            break;
                        }
                        case 6:
                            cout << CYAN << "Returning to Main Menu...\n" << RESET;
                            break; // Return to main menu
                        case 7:
                            cout << RED << "Exiting program...\n" << RESET;
                            return 0; // Exit program
                        default:
                            cout << RED << "Invalid choice. Please try again.\n" << RESET;
                        }

                    } while (userChoice != 6 && userChoice != 7);// Exit program or return to main menu
                    // Exit program or return to main menu
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

    system("pause>0");

}