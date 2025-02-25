#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../data/db.h"

// Authentication functions (auth.c)
int login_user(sqlite3 *db);
void register_user(sqlite3 *db);

// Account management functions (accounts.c)
void account_menu(sqlite3 *db, int user_id);

#endif