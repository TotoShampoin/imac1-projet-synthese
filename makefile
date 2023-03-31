EXEC = licorri
CC = g++

CFLAGS  = -Wall -O2 -g
LDFLAGS = -lglfw -lGLU -lGL -lm

SRCDIR = src/
INCDIR = include/
OBJDIR = obj/
BINDIR = bin/

SOURCES = $(shell find $(SRC_DIR)/ -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJECTS)
	@mkdir -p $(BIN_DIR)/
	$(CC) -o $(BIN_DIR)/$(EXEC_BIN) $(OBJ_FILES) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INCDIR) $(INC_DIR)

clean:
	rm -rf *~
	rm -rf $(SRC_DIR)/*/*~
	rm -rf $(OBJ_DIR)/
	rm -rf $(BIN_DIR)/*
