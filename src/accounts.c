#include "header.h"

void account_menu(sqlite3 *db, int user_id) {
    int choice;

    while (1) {
        CLEAR_SCREEN();
        printf("=== Account Menu (User ID: %d) ===\n", user_id);
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
        getchar();

        switch (choice) {
            case 1:
                 printf("Check account details - Not implemented yet.\n");
                break;
            case 2:
                CLEAR_SCREEN();
                printf("Check account details - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 3:
                CLEAR_SCREEN();
                printf("Update account info - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 4:
                CLEAR_SCREEN();
                printf("Remove account - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 5:
                CLEAR_SCREEN();
                printf("List owned accounts - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 6:
                CLEAR_SCREEN();
                printf("Make transaction - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 7:
                CLEAR_SCREEN();
                printf("Transfer ownership - Not implemented yet.\n");
                PAUSE_DISPLAY();
                break;
            case 8:
                CLEAR_SCREEN();
                printf("Logging out...\n");
                PAUSE_DISPLAY();
                return;
            default:
                CLEAR_SCREEN();
                printf("Invalid choice. Please try again.\n");
                PAUSE_DISPLAY();
        }
    }
}