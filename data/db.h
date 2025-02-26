#ifndef DB_H
#define DB_H

#include "../src/sqlite3.h"

//Initialize the database with tables
int init_database(sqlite3 *db);

#endif