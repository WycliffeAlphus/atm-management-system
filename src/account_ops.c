#include "header.h"

// Helper function to check if account belongs to user
static int account_exists_and_owned(sqlite3 *db, int user_id, int account_id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM accounts WHERE user_id = ? AND account_id = ?;";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, account_id);

    rc = sqlite3_step(stmt);
    int exists = (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0);
    sqlite3_finalize(stmt);
    return exists;
}


int isValidAccountType(const char *type) {
    const char *validTypes[] = {"current", "savings", "fixed01", "fixed02", "fixed03"};
    int numTypes = sizeof(validTypes) / sizeof(validTypes[0]);

    for (int i = 0; i < numTypes; i++) {
        if (strcmp(type, validTypes[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void create_account(sqlite3 *db, int user_id) {
    char country[50], phone[20], type[20], date_created[11];
    double balance;
    sqlite3_stmt *stmt;
    int rc, new_account_id;

    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    strftime(date_created, sizeof(date_created), "%d/%m/%Y", tm);

    CLEAR_SCREEN();
    printf("=== Create a New Account ===\n");

    printf("Enter country: ");
    fgets(country, sizeof(country), stdin);
    country[strcspn(country, "\n")] = 0;

    printf("Enter phone number: ");
    fgets(phone, sizeof(phone), stdin);
    phone[strcspn(phone, "\n")] = 0;

    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) {
            printf("Error: Phone number must contain only digits.\n");
            PAUSE_DISPLAY();
            return;
        }
    }

    printf("Enter account type (current, savings, fixed01, fixed02, fixed03): ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = 0;

    if (!isValidAccountType(type)){
        printf("Error: Invalid account type entered.\n");
        PAUSE_DISPLAY();
        return;
    }

    printf("Enter initial balance (in dollars): $");
    scanf("%lf", &balance);

    getchar();

    if (balance < 0) {
          printf("Error: Initial balance cannot be negative.\n");
          PAUSE_DISPLAY();
          return;
    }

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
        // Get the newly created account_id
        const char *id_sql = "SELECT account_id FROM accounts WHERE user_id = ? ORDER BY account_id DESC LIMIT 1;";
        rc = sqlite3_prepare_v2(db, id_sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            CLEAR_SCREEN();
            fprintf(stderr, "Failed to retrieve account number: %s\n", sqlite3_errmsg(db));
            PAUSE_DISPLAY();
            sqlite3_finalize(stmt);
            return;
        }
        sqlite3_bind_int(stmt, 1, user_id);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            new_account_id = sqlite3_column_int(stmt, 0);
            CLEAR_SCREEN();
            printf("Account created successfully!\n");
            printf("Your new account number is: %d\n", new_account_id);
            printf("Use this number for future operations or list accounts (option 5) to see all.\n");
            PAUSE_DISPLAY();
            PAUSE_DISPLAY();
        } else {
            CLEAR_SCREEN();
            printf("Account created, but failed to retrieve account number.\n");
            PAUSE_DISPLAY();
        }
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to create account: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
    }
    sqlite3_finalize(stmt);
}
void check_account_details(sqlite3 *db, int user_id) {
    int account_id;
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Check Account Details ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &account_id) != 1) {
        while (getchar() != '\n');
        printf("Invalid account number. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    if (!account_exists_and_owned(db, user_id, account_id)) {
        CLEAR_SCREEN();
        printf("Account number %d not found or not owned by you.\n", account_id);
        PAUSE_DISPLAY();
        return;
    }

    const char *sql = "SELECT date_created, country, phone, balance, type FROM accounts WHERE user_id = ? AND account_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, account_id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *date = (const char *)sqlite3_column_text(stmt, 0);
        const char *country = (const char *)sqlite3_column_text(stmt, 1);
        const char *phone = (const char *)sqlite3_column_text(stmt, 2);
        double balance = sqlite3_column_double(stmt, 3);
        const char *type = (const char *)sqlite3_column_text(stmt, 4);

        // Extract day from date_created (DD/MM/YYYY)
        int day;
        sscanf(date, "%d/", &day); // Reads the day (first two chars before '/')

        CLEAR_SCREEN();
        printf("=== Account Details ===\n");
        printf("Account Number: %d\n", account_id);
        printf("Date Created: %s\n", date);
        printf("Country: %s\n", country);
        printf("Phone: %s\n", phone);
        printf("Balance: $%.2f\n", balance);
        printf("Type: %s\n", type);

        if (strcmp(type, "current") == 0) {
            printf("You will not get interest because the account is of type current.\n");
        } else {
            double rate = 0.0;
            if (strcmp(type, "savings") == 0) rate = 0.07;
            else if (strcmp(type, "fixed01") == 0) rate = 0.04;
            else if (strcmp(type, "fixed02") == 0) rate = 0.05;
            else if (strcmp(type, "fixed03") == 0) rate = 0.08;
            if (rate > 0) {
                double interest = balance * rate / 12;
                printf("You will get $%.2f as interest on day %d of every month.\n", interest, day);
            }
        }

        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
    }

    sqlite3_finalize(stmt);
}
void update_account_info(sqlite3 *db, int user_id) {
    int account_id, field_choice;
    char new_value[50];
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Update Account Info ===\n");
    printf("Enter account ID: ");
    if (scanf("%d", &account_id) != 1) {
        while (getchar() != '\n');
        printf("Invalid account ID. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    if (!account_exists_and_owned(db, user_id, account_id)) {
        CLEAR_SCREEN();
        printf("Account ID %d not found or not owned by you.\n", account_id);
        PAUSE_DISPLAY();
        return;
    }

    printf("Select field to update:\n");
    printf("1. Country\n");
    printf("2. Phone number\n");
    printf("Enter your choice: ");
    if (scanf("%d", &field_choice) != 1) {
        while (getchar() != '\n');
        CLEAR_SCREEN();
        printf("Invalid choice. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    printf("Enter new value: ");
    fgets(new_value, sizeof(new_value), stdin);
    new_value[strcspn(new_value, "\n")] = 0;

    const char *sql = (field_choice == 1) ?
        "UPDATE accounts SET country = ? WHERE user_id = ? AND account_id = ?;" :
        "UPDATE accounts SET phone = ? WHERE user_id = ? AND account_id = ?;";
    if (field_choice != 1 && field_choice != 2) {
        CLEAR_SCREEN();
        printf("Invalid field choice. Must be 1 or 2.\n");
        PAUSE_DISPLAY();
        return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_text(stmt, 1, new_value, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, account_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("Account updated successfully!\n");
        PAUSE_DISPLAY();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to update account: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
    }

    sqlite3_finalize(stmt);
}

void remove_account(sqlite3 *db, int user_id) {
    int account_id;
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Remove Account ===\n");
    printf("Enter account ID: ");
    if (scanf("%d", &account_id) != 1) {
        while (getchar() != '\n');
        printf("Invalid account ID. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    if (!account_exists_and_owned(db, user_id, account_id)) {
        CLEAR_SCREEN();
        printf("Account ID %d not found or not owned by you.\n", account_id);
        PAUSE_DISPLAY();
        return;
    }

    const char *sql = "DELETE FROM accounts WHERE user_id = ? AND account_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, account_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("Account ID %d removed successfully!\n", account_id);
        PAUSE_DISPLAY();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to remove account: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
    }

    sqlite3_finalize(stmt);
}

void list_owned_accounts(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== List Owned Accounts ===\n");

    const char *sql = "SELECT account_id, date_created, country, phone, balance, type FROM accounts WHERE user_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (!found) printf("Your accounts:\n");
        found = 1;
        int account_id = sqlite3_column_int(stmt, 0);
        const char *date = (const char *)sqlite3_column_text(stmt, 1);
        const char *country = (const char *)sqlite3_column_text(stmt, 2);
        const char *phone = (const char *)sqlite3_column_text(stmt, 3);
        double balance = sqlite3_column_double(stmt, 4);
        const char *type = (const char *)sqlite3_column_text(stmt, 5);

        printf("Account Number: %d | Date: %s | Country: %s | Phone: %s | Balance: $%.2f | Type: %s\n",
               account_id, date, country, phone, balance, type);
    }

    if (!found) {
        printf("No accounts found.\n");
    }

    sqlite3_finalize(stmt);

    
    printf("\nPress Enter to return to the account menu...");
    fflush(stdout);
    getchar(); 
}

void make_transaction(sqlite3 *db, int user_id) {
    int account_id, type_choice;
    double amount, current_balance;
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Make Transaction ===\n");
    printf("Enter account number: ");
    if (scanf("%d", &account_id) != 1) {
        while (getchar() != '\n');
        printf("Invalid account number. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    if (!account_exists_and_owned(db, user_id, account_id)) {
        CLEAR_SCREEN();
        printf("Account number %d not found or not owned by you.\n", account_id);
        PAUSE_DISPLAY();
        return;
    }

    // Check account type and get current balance
    const char *check_sql = "SELECT type, balance FROM accounts WHERE user_id = ? AND account_id = ?;";
    rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, account_id);
    sqlite3_step(stmt);
    const char *type = (const char *)sqlite3_column_text(stmt, 0);
    current_balance = sqlite3_column_double(stmt, 1);
    sqlite3_finalize(stmt);

    if (strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 || strcmp(type, "fixed03") == 0) {
        CLEAR_SCREEN();
        printf("Transactions are not allowed for fixed accounts (type: %s).\n", type);
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
        return;
    }

    printf("Select transaction type:\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("Enter your choice: ");
    if (scanf("%d", &type_choice) != 1) {
        while (getchar() != '\n');
        CLEAR_SCREEN();
        printf("Invalid choice. Please enter a number.\n");
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
        return;
    }
    getchar();

    printf("Enter amount (in dollars): $");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        while (getchar() != '\n');
        CLEAR_SCREEN();
        printf("Invalid amount. Please enter a positive number.\n");
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
        return;
    }
    getchar();

    // Check balance for withdrawal
    if (type_choice == 2) { // Withdraw
        if (amount > current_balance) {
            CLEAR_SCREEN();
            printf("Cannot withdraw $%.2f. The amount is higher than your current balance of $%.2f.\n", 
                   amount, current_balance);
            printf("\nPress Enter to return to the account menu...");
            fflush(stdout);
            getchar();
            return;
        }
    }

    const char *sql = (type_choice == 1) ?
        "UPDATE accounts SET balance = balance + ? WHERE user_id = ? AND account_id = ?;" :
        "UPDATE accounts SET balance = balance - ? WHERE user_id = ? AND account_id = ?;";
    if (type_choice != 1 && type_choice != 2) {
        CLEAR_SCREEN();
        printf("Invalid transaction type. Must be 1 or 2.\n");
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
        return;
    }

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, account_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("%s of $%.2f %s successfully!\n", 
               (type_choice == 1 ? "Deposit" : "Withdrawal"), amount, 
               (type_choice == 1 ? "added" : "withdrawn"));
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to process transaction: %s\n", sqlite3_errmsg(db));
        printf("\nPress Enter to return to the account menu...");
        fflush(stdout);
        getchar();
    }

    sqlite3_finalize(stmt);
}

void transfer_ownership(sqlite3 *db, int user_id) {
    int account_id;
    char new_owner[50];
    sqlite3_stmt *stmt;
    int rc;

    CLEAR_SCREEN();
    printf("=== Transfer Ownership ===\n");
    printf("Enter account ID: ");
    if (scanf("%d", &account_id) != 1) {
        while (getchar() != '\n');
        printf("Invalid account ID. Please enter a number.\n");
        PAUSE_DISPLAY();
        return;
    }
    getchar();

    if (!account_exists_and_owned(db, user_id, account_id)) {
        CLEAR_SCREEN();
        printf("Account ID %d not found or not owned by you.\n", account_id);
        PAUSE_DISPLAY();
        return;
    }

    printf("Enter new owner's username: ");
    fgets(new_owner, sizeof(new_owner), stdin);
    new_owner[strcspn(new_owner, "\n")] = 0;

    // Check if new owner exists
    const char *check_sql = "SELECT id FROM users WHERE name = ?;";
    rc = sqlite3_prepare_v2(db, check_sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }
    sqlite3_bind_text(stmt, 1, new_owner, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        CLEAR_SCREEN();
        printf("User '%s' not found.\n", new_owner);
        PAUSE_DISPLAY();
        sqlite3_finalize(stmt);
        return;
    }
    int new_user_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    // Prevent self-transfer
    if (new_user_id == user_id) {
        CLEAR_SCREEN();
        printf("Error: You cannot transfer the account to yourself.\n");
        PAUSE_DISPLAY();
        return;
    }
    const char *sql = "UPDATE accounts SET user_id = ?, user_name = ? WHERE user_id = ? AND account_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
        return;
    }

    sqlite3_bind_int(stmt, 1, new_user_id);
    sqlite3_bind_text(stmt, 2, new_owner, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);
    sqlite3_bind_int(stmt, 4, account_id);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("Account ID %d transferred to %s successfully!\n", account_id, new_owner);
        PAUSE_DISPLAY();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to transfer account: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY();
    }

    sqlite3_finalize(stmt);
}