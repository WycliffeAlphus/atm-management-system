#include "header.h"


// account_menu displays the account menu
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
        fflush(stdout);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            CLEAR_SCREEN();
            printf("Invalid choice. Please enter a number.\n");
            PAUSE_DISPLAY();
            continue;
        }
        getchar();

        switch (choice) {
            case 1:
                create_account(db, user_id);
                break;
            case 2:
                check_account_details(db, user_id);
                break;
            case 3:
                update_account_info(db, user_id);
                break;
            case 4:
                remove_account(db, user_id);
                break;
            case 5:
                list_owned_accounts(db, user_id);
                break;
            case 6:
                make_transaction(db, user_id);
                break;
            case 7:
                transfer_ownership(db, user_id);
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
                break;
        }
    }
}