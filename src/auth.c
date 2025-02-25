#include "header.h"

void register_user(sqlite3 *db) {
    char name[50], password[50];
    unsigned char hash[SHA256_DIGEST_LENGTH]; // 32 bytes for SHA-256
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1]; // 64 chars + null terminator
    sqlite3_stmt *stmt;
    int rc, i;

    // Get user input
    printf("Enter username: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Hash the password using SHA-256
    SHA256((unsigned char *)password, strlen(password), hash);

    // Convert hash to hexadecimal string
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = 0; // Null terminate

    // Prepare the INSERT statement
    const char *sql = "INSERT INTO users (name, password) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind parameters (name and hashed password)
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_hex, -1, SQLITE_STATIC);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("User %s registered successfully!\n", name);
    } else if (rc == SQLITE_CONSTRAINT) {
        printf("Error: Username '%s' already exists. Please choose a different name.\n", name);
    } else {
        fprintf(stderr, "Failed to register user: %s\n", sqlite3_errmsg(db));
    }

    // Clean up
    sqlite3_finalize(stmt);
}

int login_user(sqlite3 *db) {
    char name[50], password[50];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    sqlite3_stmt *stmt;
    int user_id = -1;
    int rc, i;

    // Get user input
    printf("Enter username: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    // Hash the input password
    SHA256((unsigned char *)password, strlen(password), hash);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = 0;

    // Prepare the SELECT statement
    const char *sql = "SELECT id FROM users WHERE name = ? AND password = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Bind parameters (name and hashed password)
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_hex, -1, SQLITE_STATIC);

    // Execute and fetch result
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0); // Get user ID
        printf("Login successful for user: %s (ID: %d)\n", name, user_id);
    } else if (rc == SQLITE_DONE) {
        printf("Login failed: Incorrect username or password.\n");
    } else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
    }

    // Clean up
    sqlite3_finalize(stmt);
    return user_id;
}