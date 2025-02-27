#include "header.h"

// register_user registers a new user
void register_user(sqlite3 *db) {
    char name[50], password[50];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    sqlite3_stmt *stmt;
    int rc, i;

    CLEAR_SCREEN();
    printf("=== Register ===\n");
    printf("Enter username: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    SHA256((unsigned char *)password, strlen(password), hash);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = 0;

    const char *sql = "INSERT INTO users (name, password) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY(); // Show error
        return;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_hex, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("User %s registered successfully!\n", name);
        PAUSE_DISPLAY(); 
    } else if (rc == SQLITE_CONSTRAINT) {
        CLEAR_SCREEN();
        printf("Error: Username '%s' already exists. Please choose a different name.\n", name);
        PAUSE_DISPLAY();
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to register user: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY(); 
    }

    sqlite3_finalize(stmt);
}


// login_user logs in a user
int login_user(sqlite3 *db) {
    char name[50], password[50];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
    sqlite3_stmt *stmt;
    int user_id = -1;
    int rc, i;

    CLEAR_SCREEN();
    printf("=== Login ===\n");
    printf("Enter username: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    SHA256((unsigned char *)password, strlen(password), hash);
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[SHA256_DIGEST_LENGTH * 2] = 0;

    const char *sql = "SELECT id FROM users WHERE name = ? AND password = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY(); 
        return -1;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash_hex, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        user_id = sqlite3_column_int(stmt, 0);
        CLEAR_SCREEN();
        printf("Login successful for user: %s (ID: %d)\n", name, user_id);
        PAUSE_DISPLAY(); 
    } else if (rc == SQLITE_DONE) {
        CLEAR_SCREEN();
        printf("Login failed: Incorrect username or password.\n");
        PAUSE_DISPLAY(); 
    } else {
        CLEAR_SCREEN();
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        PAUSE_DISPLAY(); 
    }

    sqlite3_finalize(stmt);
    return user_id;
}