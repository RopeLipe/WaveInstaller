# Makefile for WaveInstaller C GTK Application

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 $(shell pkg-config --cflags gtk4 libadwaita-1)
LDFLAGS = $(shell pkg-config --libs gtk4 libadwaita-1)

# Directories
SRCDIR = .
SCREENDIR = screens
BUILDDIR = build
RESOURCEDIR = resources

# Source files
SOURCES = main.c wave-installer.c \
          $(SCREENDIR)/welcome-screen.c \
          $(SCREENDIR)/language-screen.c \
          $(SCREENDIR)/timezone-screen.c \
          $(SCREENDIR)/keyboard-screen.c \
          $(SCREENDIR)/disk-screen.c \
          $(SCREENDIR)/network-screen.c \
          $(SCREENDIR)/user-screen.c \
          $(SCREENDIR)/install-screen.c

# Object files
OBJECTS = $(SOURCES:%.c=$(BUILDDIR)/%.o)

# Target executable
TARGET = $(BUILDDIR)/wave-installer

# Default target
all: $(TARGET)

# Create build directory
$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/$(SCREENDIR)

# Build target
$(TARGET): $(BUILDDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install target
install: $(TARGET)
	install -D $(TARGET) /usr/local/bin/wave-installer
	install -D $(RESOURCEDIR)/style.css /usr/local/share/wave-installer/style.css
	install -D wave-installer.desktop /usr/local/share/applications/wave-installer.desktop

# Clean build files
clean:
	rm -rf $(BUILDDIR)

# Development run
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Package for distribution
package: $(TARGET)
	mkdir -p dist/wave-installer/bin
	mkdir -p dist/wave-installer/share
	cp $(TARGET) dist/wave-installer/bin/
	cp -r $(RESOURCEDIR) dist/wave-installer/share/
	cp README.md dist/wave-installer/
	tar -czf wave-installer-c.tar.gz -C dist wave-installer

.PHONY: all clean install run debug package
