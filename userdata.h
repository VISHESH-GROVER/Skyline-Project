#ifndef USERDATA_H
#define USERDATA_H

#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include <regex>
#include "nlohmann/json.hpp"

// Color definitions
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string RESET = "\033[0m";

// Forward declarations
void clearConsole();
void centerText(const std::string& text);
void displayLogo();
bool isValidEmail(const std::string& email);
bool isValidPassword(const std::string& password);

// Session structure
struct Session {
    time_t startTime;
    time_t endTime;
    double cost;
    bool isPaid;

    Session() : startTime(0), endTime(0), cost(0.0), isPaid(false) {}
};

// User Registration structure
struct UserRegistration {
    std::string username;
    std::string password;
    std::string fullname;
    std::string email;
    int phoneno;
    time_t joinDate;
};

// User class
class User {
public:
    std::string name;
    std::string email;
    std::string password;
    int userID;
    int phoneno;
    double totalBill;
    bool isLoggedIn;
    time_t joinDate;
    std::vector<Session> sessions;

    User(std::string n, std::string e, std::string p, int id);
    void startSession();
    void endSession(double sessionBill);
    int getTotalTime() const;
    bool hasPaidBill() const;
};

// Admin class
class Admin {
public:
    std::string email;
    std::string password;

    Admin(std::string e, std::string p);
    void viewAllUsers(const std::vector<User>& users);
    void viewOnlineUsers(const std::vector<User>& users);
    void viewUsersByPaymentStatus(const std::vector<User>& users, bool paid);
    void searchAndEditUser(std::vector<User>& users);
    void searchAndDeleteUser(std::vector<User>& users);
    void viewTotalStats(const std::vector<User>& users);
};

// Function declarations for JSON operations
void saveUserToJson(const UserRegistration& user);
void loadAdminsFromJson(std::vector<Admin>& admins);
void saveAdminToJson(const std::string& email, const std::string& password);
void loadUsersFromJson(std::vector<User>& users, int& userIDCounter);

// Menu handling functions
void handleUserMenu(User& user);
void handleAdminMenu(Admin& admin, std::vector<User>& users, std::vector<Admin>& admins);
void handleNewAdminRegistration(std::vector<Admin>& admins);
void handleRegistration(std::vector<User>& users, int& userIDCounter);

// UI helper functions
void displayCenteredMenu(const std::vector<std::string>& menuItems, const std::string& title);
void getUserInput(const std::string& prompt, std::string& input);
void getUserInput(const std::string& prompt, int& input);
std::string padString(const std::string& str, int width);

#endif // USERDATA_H