# Compilador e flags
CC = gcc
# Corrigido -Isrc/menu para -Isrc/MENU
CFLAGS = -Wall -Wextra -Iinclude -Isrc -Isrc/MENU

# Diretórios
SRC_DIR = src
MENU_DIR = $(SRC_DIR)/MENU
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
VPATH = $(SRC_DIR) $(MENU_DIR) $(LIB_DIR)

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(MENU_DIR)/*.c) \
       $(wildcard $(LIB_DIR)/*.c)

OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Nome do executável
TARGET = $(BIN_DIR)/SMASH_TOONS

# Bibliotecas
LIBS = -lraylib -lm -ldl -lGL -lpthread -lrt -lX11 -lcurl

# Regras
.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)