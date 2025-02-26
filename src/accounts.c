#include "header.h"

void create_account(sqlite3 *db, int user_id) {
    // Unchanged, same as before
    char country[50], phone[20], type[20], date_created[11];
    double balance;
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Create a New Account ===\n");
    strcpy(date_created, "25/02/2025");

    printf("Enter country: ");
    fgets(country, sizeof(country), stdin);
    country[strcspn(country, "\n")] = 0;

    printf("Enter phone number: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    printf("Enter account type (current, savings, fixed01, fixed02, fixed03): ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;

    printf("Enter initial balance: ");
    scanf("%lf", &balance);
    getchar();

    const char *sql = "INSERT INTO accounts (user_id, user_name, account_id, date_created, country, phone, balance, type) "
                      "VALUES (?, (SELECT name FROM users WHERE id = ?), "
                      "(SELECT COALESCE(MAX(account_id), -1) + 1 FROM accounts WHERE user_id = ?), "
                      "?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, user_id);
    sqlite3_bind_text(stmt, 4, date_created, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 7, balance);
    sqlite3_bind_text(stmt, 8, type, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("Account created successfully!\n");
        PAUSE_DISPLAY();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to create account: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
    }

    sqlite3_finalize(stmt);
}

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

        // Check if scanf successfully read an integer
        if (scanf("%d", &choice) != 1) {
            // Invalid input (non-numeric), clear the buffer
            while (getchar() != '\n'); // Consume remaining input
            CLEAR_SCREEN();
            printf("Invalid choice. Please enter a number.\n");
            PAUSE_DISPLAY();
            continue; // Skip switch, go back to menu
        }
        getchar(); // Clear newline after valid number

        switch (choice) {
            case 1:
                create_account(db, user_id);
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
                break;
        }
    }
}