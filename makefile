EXEC = imac-corridor
CC = g++

CFLAGS  = -Wall -O2 -g
LDFLAGS = -lglfw -lGLU -lGL -lopenal -lsndfile

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

SOURCES = $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS = $(patsubst $(SRCDIR)%.cpp,$(OBJDIR)%.o, $(SOURCES))

all: $(BINDIR)/$(EXEC)

$(BINDIR)/$(EXEC): $(OBJECTS)
	mkdir -p $(BINDIR)/
	mkdir -p $(BINDIR)/assets
	cp -r assets/* $(BINDIR)/assets
	$(CC) -o $(BINDIR)/$(EXEC) $(OBJECTS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) -I$(INCDIR)

clean:
	rm -rf *~
	rm -rf $(OBJDIR)/
	rm -rf $(BINDIR)/*
