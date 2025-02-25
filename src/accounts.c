#include "header.h"

void account_menu(sqlite3 *db, int user_id) {
    int choice;

    while (1) {
        printf("\n=== Account Menu (User ID: %d) ===\n", user_id);
        printf("1. Create a new account\n");
        printf("2. Check account details\n");
        printf("3. Update account info\n");
        printf("4. Remove account\n");
        printf("5. List owned accounts\n");
        printf("6. Make transaction\n");
        printf("7. Transfer ownership\n");
        printf("8. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice) {
            case 1:
                printf("Create a new account - Not implemented yet.\n");
                break;
            case 2:
                printf("Check account details - Not implemented yet.\n");
                break;
            case 3:
                printf("Update account info - Not implemented yet.\n");
                break;
            case 4:
                printf("Remove account - Not implemented yet.\n");
                break;
            case 5:
                printf("List owned accounts - Not implemented yet.\n");
                break;
            case 6:
                printf("Make transaction - Not implemented yet.\n");
                break;
            case 7:
                printf("Transfer ownership - Not implemented yet.\n");
                break;
            case 8:
                printf("Logging out...\n");
                return; // Exit the menu, return to main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}