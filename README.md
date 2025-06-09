# Wave Installer

A modern, GTK4-based installer for Linux distributions with a beautiful, user-friendly interface.

## Features

- **Modern UI**: Clean, rounded interface with blue accents and theme-aware styling
- **No Titlebar**: Frameless window design for a modern appearance
- **Multi-screen Workflow**: Guided installation process through 8 intuitive screens
- **Theme Support**: Automatically adapts to system light/dark theme
- **Custom CSS**: Prevents Adwaita theme conflicts with complete custom styling
- **Interactive Elements**: Proper UI components for each screen type (disk selection uses cards, not dropdowns)
- **Responsive Design**: Works well on different screen sizes

## Screens

1. **Welcome** - Introduction with feature highlights
2. **Language** - Language selection with native names and flags
3. **Timezone** - Searchable timezone selection with regions
4. **Keyboard** - Keyboard layout and variant selection with testing
5. **Disk** - Visual disk selection with usage indicators and partition schemes
6. **Network** - Wi-Fi and Ethernet configuration with connection testing
7. **User** - User account creation with password strength checking
8. **Install** - Real-time installation progress with detailed logging

## Requirements

### System Dependencies (install via package manager)
```bash
# Ubuntu/Debian
sudo apt install python3-gi gir1.2-gtk-4.0 gir1.2-adw-1 python3-gi-cairo

# Fedora
sudo dnf install python3-gobject gtk4-devel libadwaita-devel

# Arch Linux
sudo pacman -S python-gobject gtk4 libadwaita
```

## File Structure

```
WaveInstaller/
├── main.py                 # Main application entry point
├── requirements.txt        # Python dependencies documentation
├── assets/
│   ├── style.css          # Main CSS styling (prevents Adwaita conflicts)
│   ├── additional.css     # Additional CSS for special elements
│   └── icons/             # Application icons (create as needed)
└── screens/               # Individual screen implementations
    ├── welcome.py         # Welcome screen with features
    ├── language.py        # Language selection
    ├── timezone.py        # Timezone configuration
    ├── keyboard.py        # Keyboard layout selection
    ├── disk.py           # Disk partitioning
    ├── network.py        # Network configuration
    ├── user.py           # User account setup
    └── install.py        # Installation progress
```

## Running the Installer

```bash
# Make executable
chmod +x main.py

# Run the installer
python3 main.py
```

## Design Principles

- **User-Centric**: Each screen uses the most appropriate UI elements for its purpose
- **Visual Feedback**: Clear progress indicators, hover effects, and state changes
- **Accessibility**: Proper focus management, keyboard navigation, and reduced motion support
- **Theme Integration**: Respects system theme while maintaining consistent branding
- **Error Prevention**: Input validation and helpful guidance throughout the process

## Customization

The installer is designed to be easily customizable:

- **Colors**: Modify CSS variables in `assets/style.css`
- **Branding**: Replace logos and update text in screen files
- **Steps**: Add or modify installation steps in `screens/install.py`
- **Validation**: Adjust form validation in individual screen files

## Technical Notes

- Built with **GTK4** and **Libadwaita** for modern Linux integration
- Uses **CSS Grid** and **Flexbox** for responsive layouts
- **Threaded installation** process to maintain UI responsiveness
- **Custom CSS** completely overrides Adwaita defaults to prevent conflicts
- **Rounded elements** and **modern typography** throughout

This installer provides a professional, modern experience that users expect from contemporary operating systems while maintaining the flexibility and power of a Linux environment.
