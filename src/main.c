#include "header.h"

int main() {
    sqlite3 *db;
    int rc;

    rc = sqlite3_open("data/atm.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY(); 
        return 1;
    }

    if (init_database(db) != 0) {
        sqlite3_close(db);
        return 1;
    }

    int choice;
    int logged_in_user_id = -1;

    while (1) {
        CLEAR_SCREEN();
        printf("=== ATM Management System ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                logged_in_user_id = login_user(db);
                if (logged_in_user_id >= 0) {
                    account_menu(db, logged_in_user_id);
                    logged_in_user_id = -1;
                }
                break;
            case 2:
                register_user(db);
                break;
            case 3:
                CLEAR_SCREEN();
                printf("Goodbye!\n");
                PAUSE_DISPLAY(); 
                sqlite3_close(db);
                return 0;
            default:
                CLEAR_SCREEN();
                printf("Invalid choice. Please try again.\n");
                PAUSE_DISPLAY(); 
                break;
        }
    }

    sqlite3_close(db);
    return 0;
}