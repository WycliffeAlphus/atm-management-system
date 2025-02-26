#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include <openssl/sha.h>
#ifdef _WIN32
#include <windows.h>  // For Sleep on Windows
#else
#include <unistd.h>   // For sleep on Unix-like systems
#endif
#include "../data/db.h"

// Clear the terminal screen
#ifdef _WIN32
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

// Pause display for 2 seconds
#ifdef _WIN32
#define PAUSE_DISPLAY() Sleep(2000)  // Sleep in milliseconds
#else
#define PAUSE_DISPLAY() sleep(2)     // sleep in seconds
#endif

// Authentication functions (auth.c)
int login_user(sqlite3 *db);
void register_user(sqlite3 *db);

// Account management functions (accounts.c)
void account_menu(sqlite3 *db, int user_id);
void create_account(sqlite3 *db, int user_id);

#endif