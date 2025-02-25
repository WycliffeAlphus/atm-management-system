#include "header.h"

int main() {
    sqlite3 *db;
    int rc;

    
    rc = sqlite3_open("data/atm.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    
    if (init_database(db) != 0) {
        sqlite3_close(db);
        return 1;
    }

    int choice;
    int logged_in_user_id = -1;

    while (1) {
        printf("\n=== ATM Management System ===\n");
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
                    printf("Login successful!\n");
                    account_menu(db, logged_in_user_id);
                    logged_in_user_id = -1; 
                }
                break;
            case 2:
                // register_user(db);
                break;
            case 3:
                printf("Goodbye!\n");
                sqlite3_close(db);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    sqlite3_close(db); 
    return 0;
}