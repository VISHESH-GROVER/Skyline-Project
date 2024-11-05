//main.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "userdata.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

int main() {
    std::vector<User> users;
    std::vector<Admin> admins = { Admin("admin@example.com", "admin123") };
    int userIDCounter = 1;

    loadUsersFromJson(users, userIDCounter);
    loadAdminsFromJson(admins);

    while (true) {
        std::vector<std::string> mainMenuItems = {
            "1. Register",
            "2. Login as User",
            "3. Login as Admin",
            "4. Exit"
        };

        displayCenteredMenu(mainMenuItems, "Main Menu");

        int choice;
        getUserInput("Choose an option:", choice);

        switch (choice) {
        case 1:
            handleRegistration(users, userIDCounter);
            break;

        case 2: {
            std::string email, password;
            getUserInput("Enter Email:", email);
            getUserInput("Enter Password:", password);

            bool userFound = false;
            for (auto& user : users) {
                if (user.email == email && user.password == password) {
                    userFound = true;
                    centerText(CYAN + "Welcome " + user.name + "!" + RESET);
                    user.startSession();
                    handleUserMenu(user);
                    break;
                }
            }
            if (!userFound) {
                centerText(RED + "Invalid email or password." + RESET);
            }
            break;
        }

        case 3: {
            std::string email, password;
            getUserInput("Enter Admin Email:", email);
            getUserInput("Enter Password:", password);

            bool adminFound = false;
            for (auto& admin : admins) {
                if (admin.email == email && admin.password == password) {
                    adminFound = true;
                    centerText(CYAN + "Welcome Admin!" + RESET);
                    handleAdminMenu(admin, users, admins);
                    break;
                }
            }
            if (!adminFound) {
                centerText(RED + "Invalid admin credentials." + RESET);
            }
            break;
        }

        case 4:
            centerText(RED + "Exiting..." + RESET);
            return 0;

        default:
            centerText(RED + "Invalid choice, please try again." + RESET);
        }

        if (choice != 4) {
            std::cout << "\nPress Enter to return to the main menu...";
            std::cin.get();
        }
    }

    return 0;
}