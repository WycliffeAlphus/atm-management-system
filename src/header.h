#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "../data/db.h"

int login_user(sqlite3 *db);
void register_user(sqlite3 *db);

#endif