CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
PKGCONFIG = pkg-config
GTK_CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
GTK_LIBS = $(shell $(PKGCONFIG) --libs gtk4)

SRCDIR = .
SCREENDIR = screens
BUILDDIR = build

SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/installer.c \
          $(SCREENDIR)/welcome.c \
          $(SCREENDIR)/language.c \
          $(SCREENDIR)/timezone.c \
          $(SCREENDIR)/keyboard.c \
          $(SCREENDIR)/disk.c \
          $(SCREENDIR)/network.c \
          $(SCREENDIR)/user.c \
          $(SCREENDIR)/install.c

OBJECTS = $(SOURCES:%.c=$(BUILDDIR)/%.o)
TARGET = wave-installer

.PHONY: all clean install

all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/$(SCREENDIR)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(GTK_LIBS)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
	rm -f $(TARGET)

install: $(TARGET)
	install -D $(TARGET) /usr/local/bin/$(TARGET)
	install -D style.css /usr/local/share/$(TARGET)/style.css
	cp -r assets /usr/local/share/$(TARGET)/

run: $(TARGET)
	./$(TARGET)

# Development helpers
dev: $(TARGET)
	GTK_DEBUG=interactive ./$(TARGET)

format:
	clang-format -i $(SOURCES) $(SRCDIR)/*.h $(SCREENDIR)/*.h

.SECONDARY: $(OBJECTS)
