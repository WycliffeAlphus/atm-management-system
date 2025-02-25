CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lsqlite3 -lcrypto  
SRC = src/main.c src/auth.c src/accounts.c data/db.c
OBJ = $(SRC:.c=.o)
EXEC = atm

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)