# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc -Isrc/menu

# Diretórios
SRC_DIR = src
MENU_DIR = $(SRC_DIR)/MENU
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(MENU_DIR)/*.c) \
       $(wildcard $(LIB_DIR)/*.c)

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Nome do executável
TARGET = $(BIN_DIR)/SMASH_TOONS

# Bibliotecas
LIBS = -lraylib -lm -ldl -lGL -lpthread -lrt -lX11

# Regras
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(MENU_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
