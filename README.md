# WaveInstaller - C GTK Implementation

A modern Linux installer UI built with C and GTK4/Libadwaita, featuring blue accents, light/dark theme support, rounded elements, and a clean, modern interface.

## Features

- **Modern UI Design**: Clean, modern interface with blue accent colors
- **Theme Support**: Automatic light/dark theme detection and support
- **Rounded Elements**: Consistent rounded corners throughout the interface
- **No Titlebar**: Clean, borderless window design
- **Multi-Screen Flow**: Welcome → Language → Timezone → Keyboard → Disk → Network → User → Install
- **GTK4/Libadwaita**: Built with the latest GTK technologies
- **Custom CSS**: Fully customizable styling with CSS

## Prerequisites

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential pkg-config libgtk-4-dev libadwaita-1-dev
```

### Fedora/RHEL
```bash
sudo dnf install gcc pkg-config gtk4-devel libadwaita-devel
```

### Arch Linux
```bash
sudo pacman -S base-devel pkg-config gtk4 libadwaita
```

## Building

1. Clone or download the project
2. Navigate to the C GTK example directory:
   ```bash
   cd c-gtk-example
   ```

3. Build the application:
   ```bash
   make
   ```

4. Run the application:
   ```bash
   make run
   ```

## Installation

To install system-wide:
```bash
sudo make install
```

This will install:
- Binary to `/usr/local/bin/wave-installer`
- CSS resources to `/usr/local/share/wave-installer/`
- Desktop entry to `/usr/local/share/applications/`

## Development

### Debug Build
```bash
make debug
```

### Clean Build Files
```bash
make clean
```

### Package for Distribution
```bash
make package
```

## Project Structure

```
c-gtk-example/
├── main.c                 # Application entry point
├── wave-installer.c       # Main application implementation
├── wave-installer.h       # Main application header
├── screens/               # Screen implementations
│   ├── welcome-screen.c   # Welcome screen
│   ├── language-screen.c  # Language selection
│   ├── timezone-screen.c  # Timezone selection
│   ├── keyboard-screen.c  # Keyboard layout
│   ├── disk-screen.c      # Disk partitioning
│   ├── network-screen.c   # Network configuration
│   ├── user-screen.c      # User account creation
│   ├── install-screen.c   # Installation progress
│   └── *.h               # Header files for each screen
├── resources/
│   └── style.css         # Custom CSS styling
├── Makefile              # Build configuration
└── README.md             # This file
```

## Architecture

The application follows a modular architecture:

1. **Main Application** (`wave-installer.c`): Manages the application lifecycle, window creation, and screen navigation
2. **Screen Modules**: Each installer step is implemented as a separate module in the `screens/` directory
3. **CSS Styling**: All visual styling is handled through CSS for easy customization
4. **Navigation**: Consistent navigation pattern with Back/Next buttons

## Key Components

### Screen Management
Each screen is implemented as a separate C file with a creation function that returns a GTK widget tree. The main application manages screen transitions and maintains state.

### CSS Integration
The application loads a custom CSS file at startup, providing:
- Blue accent color scheme
- Rounded corners and modern styling
- Dark/light theme support
- Consistent spacing and typography

### Navigation Flow
1. **Welcome**: Introduction and getting started
2. **Language**: System language selection
3. **Timezone**: Timezone and region selection
4. **Keyboard**: Keyboard layout and variant selection
5. **Disk**: Disk selection and partitioning options
6. **Network**: WiFi/Ethernet network configuration
7. **User**: User account and hostname setup
8. **Install**: Installation progress and completion

## Customization

### Styling
Edit `resources/style.css` to customize:
- Colors and themes
- Button styles
- Layout spacing
- Typography

### Adding Screens
1. Create new `.c` and `.h` files in `screens/`
2. Add screen enum to `wave-installer.h`
3. Add screen creation function to main application
4. Update Makefile to include new source files

### Modifying UI
Each screen's UI is built programmatically in its respective `.c` file. GTK4 widgets and CSS classes provide the visual structure.

## Differences from Python Version

- **Performance**: C implementation offers better performance and smaller memory footprint
- **Dependencies**: Only requires GTK4/Libadwaita, no Python runtime needed
- **Distribution**: Compiles to native binary for easier distribution
- **Integration**: Better system integration with native GTK applications

## Contributing

1. Fork the project
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. Please check the main project for license details.
