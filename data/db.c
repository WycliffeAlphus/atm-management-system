#include "db.h"
#include <stdio.h>

// init_database creates the tables if they don't exist
int init_database(sqlite3 *db){

    char *err_msg = 0;
    const char *sql = 
       "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT UNIQUE NOT NULL,"
            "password TEXT NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS accounts ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "user_id INTEGER,"
            "user_name TEXT,"
            "account_id INTEGER,"
            "date_created TEXT,"
            "country TEXT,"
            "phone TEXT,"
            "balance REAL,"
            "type TEXT,"
            "FOREIGN KEY (user_id) REFERENCES users(id)"
        ");";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK){
        fprintf(stderr, "SQL error:%s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }    
    return 0;
}