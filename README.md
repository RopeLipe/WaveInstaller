# Wave Installer

A modern GTK4-based Linux installer with a clean, fixed-size interface supporting both light and dark themes.

## Features

- **Modern UI**: Clean, rounded corners, blue accent color
- **Fixed Size Window**: 900x600 pixels, no titlebar/decorations
- **Theme Support**: Both light and dark theme compatibility
- **Complete Installation Flow**: 7 pages covering all installation steps
- **Interactive Elements**: Searchable dropdowns, selectable cards, password strength indicator

## Installation Pages

1. **Welcome** - Introduction with feature list
2. **Language Selection** - Searchable language dropdown
3. **Timezone Selection** - Searchable timezone dropdown with current time display
4. **Keyboard Layout** - Layout selection with preview and test area
5. **Disk Selection** - Selectable disk cards with size/type information
6. **Network Configuration** - Wi-Fi toggle, network cards, password dialog
7. **User Account Creation** - User form with password strength indicator

## Project Structure

```
WaveInstaller/
├── main.c              # Application entry point
├── installer.h         # Main header with function declarations
├── installer.c         # Main window and navigation logic
├── css.c              # CSS loading functionality
├── style.css          # External stylesheet
├── pages/             # Individual page implementations
│   ├── welcome.c
│   ├── language.c
│   ├── timezone.c
│   ├── keyboard.c
│   ├── disk.c
│   ├── network.c
│   └── user.c
└── Makefile           # Build configuration
```

## Dependencies

- GTK4 development libraries
- GLib development libraries
- GCC compiler

### Ubuntu/Debian:
```bash
sudo apt install libgtk-4-dev libglib2.0-dev gcc make
```

### Fedora:
```bash
sudo dnf install gtk4-devel glib2-devel gcc make
```

### Arch Linux:
```bash
sudo pacman -S gtk4 glib2 gcc make
```

## Building

1. **Clone/Download** the project
2. **Navigate** to the project directory
3. **Build** using make:

```bash
make
```

### Build Options

- `make` - Standard build
- `make debug` - Build with debug symbols
- `make clean` - Clean build files
- `make run` - Build and run
- `make install` - Install to /usr/local/bin

## CSS Styling

The application loads its styles from `style.css`. The CSS file path is currently hardcoded to:
```
/home/l/WaveInstaller/style.css
```

To change the CSS path, edit the `css_file_path` variable in `css.c`:

```c
const char* css_file_path = "/path/to/your/style.css";
```

If the CSS file is not found, the application will fall back to minimal inline styles and continue running.

## Customization

### Changing Colors
Edit `style.css` and modify the color values. The primary accent color is `#0066cc` (blue).

### Adding New Pages
1. Create a new `.c` file in the `pages/` directory
2. Add the function declaration to `installer.h`
3. Add the page to the stack in `installer.c`
4. Update the navigation logic
5. Add the new file to the Makefile

### Window Size
Modify the window size in `installer.c`:
```c
gtk_window_set_default_size(GTK_WINDOW(main_window), WIDTH, HEIGHT);
```

## Notes

- This is a **frontend-only** implementation with no actual installation logic
- All backend functionality is simulated with placeholder data
- The window is fixed-size and unresizable by design
- Navigation between pages uses smooth slide transitions
- All UI elements support both light and dark GTK themes

## Development

The code follows GTK4 best practices:
- Uses native GTK4 widgets wherever possible
- Implements proper memory management with GObject reference counting
- Supports theme-aware styling through CSS
- Uses appropriate GTK containers for responsive layouts
