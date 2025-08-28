CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c11 -pthread

BIN_DIR=.
SRC_DIR=src

all: serial mutex rwlock

serial: $(SRC_DIR)/serial_list.c $(SRC_DIR)/common.h $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/serial $(SRC_DIR)/serial_list.c $(SRC_DIR)/util.c

mutex: $(SRC_DIR)/pthread_mutex_list.c $(SRC_DIR)/common.h $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mutex $(SRC_DIR)/pthread_mutex_list.c $(SRC_DIR)/util.c

rwlock: $(SRC_DIR)/pthread_rwlock_list.c $(SRC_DIR)/common.h $(SRC_DIR)/util.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/rwlock $(SRC_DIR)/pthread_rwlock_list.c $(SRC_DIR)/util.c

clean:
	rm -f serial mutex rwlock
