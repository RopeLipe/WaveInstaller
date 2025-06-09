CC = gcc
CFLAGS = -Wall -Wextra -std=c99 $(shell pkg-config --cflags gtk4)
LIBS = $(shell pkg-config --libs gtk4)
TARGET = wave-installer
SRCDIR = .
PAGEDIR = pages

# Source files
SOURCES = main.c installer.c css.c \
          $(PAGEDIR)/welcome.c \
          $(PAGEDIR)/language.c \
          $(PAGEDIR)/timezone.c \
          $(PAGEDIR)/keyboard.c \
          $(PAGEDIR)/disk.c \
          $(PAGEDIR)/network.c \
          $(PAGEDIR)/user.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install target (optional)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# Run the application
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Dependencies
main.o: main.c installer.h
installer.o: installer.c installer.h
css.o: css.c installer.h
$(PAGEDIR)/welcome.o: $(PAGEDIR)/welcome.c installer.h
$(PAGEDIR)/language.o: $(PAGEDIR)/language.c installer.h
$(PAGEDIR)/timezone.o: $(PAGEDIR)/timezone.c installer.h
$(PAGEDIR)/keyboard.o: $(PAGEDIR)/keyboard.c installer.h
$(PAGEDIR)/disk.o: $(PAGEDIR)/disk.c installer.h
$(PAGEDIR)/network.o: $(PAGEDIR)/network.c installer.h
$(PAGEDIR)/user.o: $(PAGEDIR)/user.c installer.h

.PHONY: all clean install run debug
