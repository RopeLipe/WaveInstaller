# Wave OS Installer

A modern GTK4-based installer for Wave OS with a clean, rounded UI design.

## Features

- Modern GTK4 interface with custom styling
- Step-by-step installation process
- Language selection
- Timezone configuration
- Keyboard layout selection
- Disk selection with visual cards
- Network configuration (WiFi/Ethernet)
- User account creation
- Real-time installation progress

## Design

- **Blue accents** with light/dark theme support
- **Rounded elements** throughout the interface
- **No title bar** for a modern look
- **Interactive elements** with hover effects
- **Modern icons** using system icon themes
- **Proper UI elements** for each screen (no dropdowns for disk selection)

## Building

### Prerequisites

- GTK4 development libraries
- GCC compiler
- pkg-config

On Ubuntu/Debian:
```bash
sudo apt install libgtk-4-dev build-essential pkg-config
```

On Fedora:
```bash
sudo dnf install gtk4-devel gcc pkg-config
```

### Compilation

```bash
make
```

### Running

```bash
make run
```

For development with GTK Inspector:
```bash
make dev
```

## Project Structure

```
├── main.c              # Application entry point
├── installer.c/.h      # Main window and navigation
├── style.css          # Custom GTK4 CSS styling
├── screens/           # Individual installer screens
│   ├── welcome.c/.h   # Welcome screen
│   ├── language.c/.h  # Language selection
│   ├── timezone.c/.h  # Timezone selection
│   ├── keyboard.c/.h  # Keyboard layout
│   ├── disk.c/.h      # Disk selection with cards
│   ├── network.c/.h   # Network configuration
│   ├── user.c/.h      # User account creation
│   └── install.c/.h   # Installation progress
├── assets/            # Icons and images
└── Makefile          # Build configuration
```

## Screens

1. **Welcome** - Introduction with feature highlights
2. **Language** - Language selection with flags
3. **Timezone** - Timezone selection with city list
4. **Keyboard** - Keyboard layout with preview
5. **Disk** - Visual disk selection cards (no dropdowns)
6. **Network** - WiFi and ethernet configuration
7. **User** - Account creation with avatar
8. **Install** - Progress with real-time updates

## Styling

The installer uses custom CSS to override Adwaita defaults and provide:
- Consistent theming across light/dark modes
- Blue accent colors
- Rounded corners and modern spacing
- Smooth transitions and hover effects
- Proper contrast and accessibility

## Notes

- This is a frontend UI demonstration
- Backend installation logic is not implemented
- Disk detection and actual partitioning would need system integration
- Network configuration would require NetworkManager integration
