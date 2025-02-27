#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <openssl/sha.h>
#include <time.h>  
#include <ctype.h> 
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "../data/db.h"

#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#define PAUSE_DISPLAY() Sleep(2000)
#else
#define CLEAR_SCREEN() system("clear")
#define PAUSE_DISPLAY() sleep(2)
#endif

// Authentication functions (auth.c)
int login_user(sqlite3 *db);
void register_user(sqlite3 *db);

// Account menu function (accounts.c)
void account_menu(sqlite3 *db, int user_id);

// Account operations (account_ops.c)
void create_account(sqlite3 *db, int user_id);
void check_account_details(sqlite3 *db, int user_id);
void update_account_info(sqlite3 *db, int user_id);
void remove_account(sqlite3 *db, int user_id);
void list_owned_accounts(sqlite3 *db, int user_id);
void make_transaction(sqlite3 *db, int user_id);
void transfer_ownership(sqlite3 *db, int user_id);

#endif