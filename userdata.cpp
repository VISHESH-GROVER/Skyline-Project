#define _CRT_SECURE_NO_WARNINGS

#include "userdata.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

void centerText(const std::string& text) {
    const int CONSOLE_WIDTH = 80; // Default console width
    int padding = (CONSOLE_WIDTH - text.length()) / 2;
    if (padding > 0) {
        std::cout << std::string(padding, ' ');
    }
    std::cout << text << std::endl;
}

void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title) {
    clearConsole();
    displayLogo();

    if (!title.empty()) {
        std::cout << "\n";
        centerText(YELLOW + title + RESET);
        std::cout << "\n";
    }

    // Calculate dimensions
    const int ITEMS_PER_ROW = 2;  // Number of items per row
    const int ITEM_WIDTH = 35;     // Width of each menu box
    const int TOTAL_WIDTH = ITEM_WIDTH * ITEMS_PER_ROW + 10;  // Total width including borders and spacing

    // Create top border
    std::string topBorder = "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(topBorder);

    // Display items in grid
    for (size_t i = 0; i < menuItems.size(); i += ITEMS_PER_ROW) {
        std::string line = "| ";

        // Add items in current row
        for (size_t j = 0; j < ITEMS_PER_ROW && (i + j) < menuItems.size(); j++) {
            // Create a padded item string
            std::string item = menuItems[i + j];
            std::string paddedItem = item + std::string(ITEM_WIDTH - 2 - item.length(), ' ');
            line += paddedItem;
            if (j < ITEMS_PER_ROW - 1 && (i + j + 1) < menuItems.size()) {
                line += "| ";
            }
        }

        // Pad the rest of the line if we don't have enough items
        if ((i + ITEMS_PER_ROW) > menuItems.size()) {
            line += std::string(ITEM_WIDTH * (ITEMS_PER_ROW - (menuItems.size() - i)) - 2, ' ');
        }
        line += " |";

        centerText(line);

        // Add separator line between rows if not the last row
        if (i + ITEMS_PER_ROW < menuItems.size()) {
            std::string separator = "|" + std::string(TOTAL_WIDTH - 2, '-') + "|";
            centerText(separator);
        }
    }

    // Create bottom border
    std::string bottomBorder = "+" + std::string(TOTAL_WIDTH - 2, '-') + "+";
    centerText(bottomBorder);
    std::cout << "\n";
}

// Helper function to pad strings for grid display
std::string padString(const std::string& str, int width) {
    if (str.length() >= width) return str;
    return str + std::string(width - str.length(), ' ');
}

void getUserInput(const std::string& prompt, std::string& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::getline(std::cin, input);
}

void getUserInput(const std::string& prompt, int& input) {
    centerText(prompt);
    centerText(""); // Empty line for spacing
    std::cout << std::string((80 - 20) / 2, ' ') << "> ";
    std::cin >> input;
    std::cin.ignore();
}

User::User(std::string n, std::string e, std::string p, int id)
    : name(n), email(e), password(p), userID(id), totalBill(0.0),
    isLoggedIn(false), joinDate(time(0)) {}

void User::startSession() {
    Session newSession;
    newSession.startTime = time(0);
    sessions.push_back(newSession);
    isLoggedIn = true;
    centerText(GREEN + "Session started successfully!" + RESET);
}

void User::endSession(double sessionBill) {
    if (!sessions.empty() && isLoggedIn) {
        sessions.back().endTime = time(0);
        sessions.back().cost = sessionBill;
        totalBill += sessionBill;
        isLoggedIn = false;
    }
    centerText(BLUE + "Session ended successfully!" + RESET);
    centerText("Session Bill: NZD " + std::to_string(sessionBill));
}

int User::getTotalTime() const {
    int totalMinutes = 0;
    for (const auto& session : sessions) {
        if (session.endTime > 0) {
            totalMinutes += static_cast<int>(difftime(session.endTime, session.startTime) / 60);
        }
        else if (isLoggedIn) {
            totalMinutes += static_cast<int>(difftime(time(0), session.startTime) / 60);
        }
    }
    return totalMinutes;
}

bool User::hasPaidBill() const {
    return std::all_of(sessions.begin(), sessions.end(),
        [](const Session& s) { return s.isPaid; });
}

Admin::Admin(std::string e, std::string p) : email(e), password(p) {}

void Admin::viewAllUsers(const std::vector<User>& users) {
    clearConsole();
    centerText(BLUE + "\n---- Complete User List ----" + RESET);
    int count = 1;
    for (const auto& user : users) {
        std::string joinDateStr = std::ctime(&user.joinDate);
        centerText(YELLOW + "User #" + std::to_string(count++) + RESET);
        centerText("User ID: " + std::to_string(user.userID));
        centerText("Name: " + user.name);
        centerText("Email: " + user.email);
        centerText("Join Date: " + joinDateStr);
        centerText("Total Bill: NZD " + std::to_string(user.totalBill));
        centerText("Status: " + std::string(user.isLoggedIn ? "Online" : "Offline"));
        centerText("Total Time: " + std::to_string(user.getTotalTime()) + " minutes");
        centerText("-------------------");
    }
}

void Admin::viewOnlineUsers(const std::vector<User>& users) {
    clearConsole();
    centerText(GREEN + "\n---- Currently Online Users ----" + RESET);
    bool foundOnline = false;
    for (const auto& user : users) {
        if (user.isLoggedIn) {
            centerText("User ID: " + std::to_string(user.userID));
            centerText("Name: " + user.name);
            centerText("Current Session Time: " + std::to_string(user.getTotalTime()) + " minutes");
            centerText("Current Bill: NZD " + std::to_string(user.totalBill));
            centerText("-------------------");
            foundOnline = true;
        }
    }
    if (!foundOnline) {
        centerText("No users currently online.");
    }
}

void Admin::viewUsersByPaymentStatus(const std::vector<User>& users, bool paid) {
    clearConsole();
    centerText(YELLOW + "\n---- Users with " + (paid ? "Paid" : "Unpaid") + " Bills ----" + RESET);
    bool found = false;
    for (const auto& user : users) {
        if (user.hasPaidBill() == paid) {
            centerText("User ID: " + std::to_string(user.userID));
            centerText("Name: " + user.name);
            centerText("Total Bill: NZD " + std::to_string(user.totalBill));
            centerText("-------------------");
            found = true;
        }
    }
    if (!found) {
        centerText("No users found with " + std::string(paid ? "paid" : "unpaid") + " bills.");
    }
}

void Admin::searchAndEditUser(std::vector<User>& users) {
    clearConsole();
    std::string searchTerm;
    getUserInput("Enter user ID, name, or email to search:", searchTerm);

    for (auto& user : users) {
        if (std::to_string(user.userID) == searchTerm ||
            user.name == searchTerm ||
            user.email == searchTerm) {

            centerText("User found! Current details:");
            std::vector<std::string> editMenuItems = {
                "1. Name: " + user.name,
                "2. Email: " + user.email,
                "3. Password",
                "4. Phone Number",
                "5. Cancel"
            };
            displayCenteredMenu(editMenuItems, "Edit User");

            int choice;
            getUserInput("What would you like to edit? (1-5):", choice);

            switch (choice) {
            case 1: {
                std::string newName;
                getUserInput("Enter new name:", newName);
                if (!newName.empty()) {
                    user.name = newName;
                    centerText(GREEN + "Name updated successfully!" + RESET);
                }
                break;
            }
            case 2: {
                std::string newEmail;
                getUserInput("Enter new email:", newEmail);
                if (!newEmail.empty() && isValidEmail(newEmail)) {
                    user.email = newEmail;
                    centerText(GREEN + "Email updated successfully!" + RESET);
                }
                break;
            }
            case 3: {
                std::string newPassword;
                getUserInput("Enter new password (min 8 characters):", newPassword);
                if (!newPassword.empty() && isValidPassword(newPassword)) {
                    user.password = newPassword;
                    centerText(GREEN + "Password updated successfully!" + RESET);
                }
                break;
            }
            case 4: {
                int newPhone;
                getUserInput("Enter new phone number:", newPhone);
                user.phoneno = newPhone;
                centerText(GREEN + "Phone number updated successfully!" + RESET);
                break;
            }
            case 5:
                centerText(YELLOW + "Edit cancelled." + RESET);
                break;
            default:
                centerText(RED + "Invalid choice." + RESET);
            }
            return;
        }
    }
    centerText(RED + "User not found." + RESET);
}

void Admin::searchAndDeleteUser(std::vector<User>& users) {
    clearConsole();
    std::string searchTerm;
    getUserInput("Enter user ID, name, or email to search:", searchTerm);

    auto it = std::find_if(users.begin(), users.end(),
        [&searchTerm](const User& user) {
            return std::to_string(user.userID) == searchTerm ||
                user.name == searchTerm ||
                user.email == searchTerm;
        });

    if (it != users.end()) {
        centerText("User found: " + it->name + " (ID: " + std::to_string(it->userID) + ")");

        std::string confirmation;
        getUserInput("Are you sure you want to delete? (y/n):", confirmation);

        if (!confirmation.empty() && (confirmation[0] == 'y' || confirmation[0] == 'Y')) {
            users.erase(it);
            centerText(GREEN + "User deleted successfully!" + RESET);
        }
        else {
            centerText(YELLOW + "Deletion cancelled." + RESET);
        }
    }
    else {
        centerText(RED + "User not found." + RESET);
    }
}

void Admin::viewTotalStats(const std::vector<User>& users) {
    clearConsole();
    int totalTime = 0;
    double totalBill = 0.0;
    int totalUsers = users.size();
    int onlineUsers = 0;

    for (const auto& user : users) {
        totalTime += user.getTotalTime();
        totalBill += user.totalBill;
        if (user.isLoggedIn) onlineUsers++;
    }

    centerText(CYAN + "\n---- System Statistics ----" + RESET);
    centerText("Total Users: " + std::to_string(totalUsers));
    centerText("Currently Online: " + std::to_string(onlineUsers));
    centerText("Total Time Used: " + std::to_string(totalTime) + " minutes");
    centerText("Total Bills: NZD " + std::to_string(totalBill));
}


void handleUserMenu(User& user) {
    int userChoice;
    const double browsingRate = 0.50;
    const double gamingRate = 1.20;
    const double printCost = 0.70;
    const double scanCost = 0.52;

    do {
        std::vector<std::string> menuItems = {
            "1. Internet Browsing ($0.50/min)",
            "2. Gaming ($1.20/min)",
            "3. Printing ($0.70/page)",
            "4. Scanning ($0.52/page)",
            "5. View Total Bill",
            "6. Back"
        };

        displayCenteredMenu(menuItems, "User Menu");
        getUserInput("Choose an option:", userChoice);

        switch (userChoice) {
        case 1: {
            int minutes;
            getUserInput("Enter minutes used (max 60):", minutes);
            if (minutes > 60) minutes = 60;
            user.startSession();
            user.endSession(minutes * browsingRate);
            break;
        }
        case 2: {
            int minutes;
            getUserInput("Enter minutes played (max 60):", minutes);
            if (minutes > 60) minutes = 60;
            user.startSession();
            user.endSession(minutes * gamingRate);
            break;
        }
        case 3: {
            int pages;
            getUserInput("Enter pages to print:", pages);
            user.startSession();
            user.endSession(pages * printCost);
            break;
        }
        case 4: {
            int pages;
            getUserInput("Enter pages to scan:", pages);
            user.startSession();
            user.endSession(pages * scanCost);
            break;
        }
        case 5: {
            centerText(GREEN + "Total bill: NZD " + std::to_string(user.totalBill) + RESET);
            break;
        }
        case 6: {
            centerText(CYAN + "Returning to main menu..." + RESET);
            break;
        }
        default: {
            centerText(RED + "Invalid choice. Please try again." + RESET);
            break;
        }
        }

        if (userChoice != 6) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }
    } while (userChoice != 6);
}

bool isValidEmail(const std::string& email) {
    const std::regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return std::regex_match(email, pattern);
}

bool isValidPassword(const std::string& password) {
    return password.length() >= 8;
}

void saveUserToJson(const UserRegistration& user) {
    nlohmann::json userData;
    const std::string filename = "User_Registration.json";

    std::ifstream inFile(filename);
    if (inFile.good()) {
        userData = nlohmann::json::parse(inFile);
    }
    else {
        userData["users"] = nlohmann::json::array();
    }
    inFile.close();

    nlohmann::json newUser;
    newUser["username"] = user.username;
    newUser["password"] = user.password;
    newUser["fullname"] = user.fullname;
    newUser["email"] = user.email;
    newUser["phoneno"] = user.phoneno;
    newUser["joinDate"] = user.joinDate;

    userData["users"].push_back(newUser);

    std::ofstream outFile(filename);
    outFile << userData.dump(2);
    outFile.close();

    centerText(GREEN + "Registration successful! Data saved to " + filename + RESET);
}

void loadAdminsFromJson(std::vector<Admin>& admins) {
    const std::string filename = "Admin_List.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return;
    }

    try {
        nlohmann::json adminData = nlohmann::json::parse(file);

        for (const auto& admin : adminData["admins"]) {
            std::string email = admin["email"];
            std::string password = admin["password"];
            bool exists = false;
            for (const auto& existingAdmin : admins) {
                if (existingAdmin.email == email) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                admins.emplace_back(email, password);
            }
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error loading admins: " + e.what() + RESET);
    }
    file.close();
}

void saveAdminToJson(const std::string& email, const std::string& password) {
    nlohmann::json adminData;
    const std::string filename = "Admin_List.json";

    std::ifstream inFile(filename);
    if (inFile.good()) {
        adminData = nlohmann::json::parse(inFile);
    }
    else {
        adminData["admins"] = nlohmann::json::array();
    }
    inFile.close();

    nlohmann::json newAdmin;
    newAdmin["email"] = email;
    newAdmin["password"] = password;

    adminData["admins"].push_back(newAdmin);

    std::ofstream outFile(filename);
    outFile << adminData.dump(2);
    outFile.close();

    centerText(GREEN + "Admin added successfully!" + RESET);
}

void handleNewAdminRegistration(std::vector<Admin>& admins) {
    clearConsole();
    std::string email, password;

    getUserInput("Enter new admin email:", email);
    while (!isValidEmail(email)) {
        centerText(RED + "Invalid email format." + RESET);
        getUserInput("Enter new admin email:", email);
    }

    getUserInput("Enter password (min 8 characters):", password);
    while (!isValidPassword(password)) {
        centerText(RED + "Password must be at least 8 characters." + RESET);
        getUserInput("Enter password (min 8 characters):", password);
    }

    admins.emplace_back(email, password);
    saveAdminToJson(email, password);
}

void handleRegistration(std::vector<User>& users, int& userIDCounter) {
    clearConsole();
    UserRegistration newUser;
    newUser.joinDate = time(0);

    getUserInput("Enter Username:", newUser.username);

    getUserInput("Enter Password (min 8 characters):", newUser.password);
    while (!isValidPassword(newUser.password)) {
        centerText(RED + "Password must be at least 8 characters." + RESET);
        getUserInput("Enter Password (min 8 characters):", newUser.password);
    }

    getUserInput("Enter Full Name:", newUser.fullname);

    getUserInput("Enter Email:", newUser.email);
    while (!isValidEmail(newUser.email)) {
        centerText(RED + "Invalid email format." + RESET);
        getUserInput("Enter Email:", newUser.email);
    }

    getUserInput("Enter Phone Number:", newUser.phoneno);

    saveUserToJson(newUser);
    users.emplace_back(newUser.fullname, newUser.email, newUser.password, userIDCounter++);
}

void loadUsersFromJson(std::vector<User>& users, int& userIDCounter) {
    const std::string filename = "User_Registration.json";
    std::ifstream file(filename);

    if (!file.good()) {
        return;
    }

    try {
        nlohmann::json userData = nlohmann::json::parse(file);
        users.clear();
        userIDCounter = 1;

        for (const auto& user : userData["users"]) {
            std::string fullname = user["fullname"];
            std::string email = user["email"];
            std::string password = user["password"];
            users.emplace_back(fullname, email, password, userIDCounter++);
        }
    }
    catch (const std::exception& e) {
        centerText(RED + "Error loading users: " + e.what() + RESET);
    }
    file.close();
}

void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins) {
    int choice;
    do {
        std::vector<std::string> menuItems = {
            "1. View All Users",
            "2. View Online Users",
            "3. View Paid Bills",
            "4. View Unpaid Bills",
            "5. Search/Edit User",
            "6. Delete User",
            "7. System Stats",
            "8. Add New Admin",
            "9. Return to Main Menu"
        };

        displayCenteredMenu(menuItems, "Admin Menu");
        getUserInput("Enter choice:", choice);

        switch (choice) {
        case 1: admin.viewAllUsers(users); break;
        case 2: admin.viewOnlineUsers(users); break;
        case 3: admin.viewUsersByPaymentStatus(users, true); break;
        case 4: admin.viewUsersByPaymentStatus(users, false); break;
        case 5: admin.searchAndEditUser(users); break;
        case 6: admin.searchAndDeleteUser(users); break;
        case 7: admin.viewTotalStats(users); break;
        case 8: handleNewAdminRegistration(admins); break;
        case 9: centerText(CYAN + "Logging out...\n" + RESET); break;
        default: centerText(RED + "Invalid choice\n" + RESET);
        }

        if (choice != 9) {
            centerText("\nPress Enter to continue...");
            std::cin.get();
        }
    } while (choice != 9);
}

void displayLogo() {
    clearConsole();
    std::vector<std::string> logoLines = {
        MAGENTA + " _____ _           _       _       " + RESET + "_____                ",
        MAGENTA + "/ ____| |         | |     | |     " + RESET + "/ ____|               ",
        MAGENTA + "| (___ | |__   __ _| |_ ___| |__ " + RESET + "| |  __  __ _ _ __ ___  ",
        MAGENTA + "\\___ \\| '_ \\ / _` | __/ _ \\ '_ \\ " + RESET + "| | |_ |/ _` | '__/ _ \\ ",
        MAGENTA + "____) | | | | (_| | ||  __/ | | | " + RESET + "|__| | | (_| | | | __/ ",
        MAGENTA + "|_____/|_| |_|\\__,_|\\__\\___|_| |_|" + RESET + "\\_____|\\__,_|_|  \\___| ",
        GREEN + "Cyber Cafe" + RESET
    };

    std::cout << "\n\n";
    for (const auto& line : logoLines) {
        centerText(line);
    }
    std::cout << "\n";
    centerText(GREEN + "Welcome to Skyline Cyber Cafe!" + RESET);
    std::cout << "\n";
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}