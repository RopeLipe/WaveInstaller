#!/bin/bash

# Wave Installer Setup Script
# Run this script to install dependencies and set up the installer

echo "Setting up Wave Installer..."

# Detect distribution
if [ -f /etc/debian_version ]; then
    echo "Detected Debian/Ubuntu system"
    sudo apt update
    sudo apt install -y python3-gi gir1.2-gtk-4.0 gir1.2-adw-1 python3-gi-cairo
elif [ -f /etc/fedora-release ]; then
    echo "Detected Fedora system"
    sudo dnf install -y python3-gobject gtk4-devel libadwaita-devel
elif [ -f /etc/arch-release ]; then
    echo "Detected Arch Linux system"
    sudo pacman -S --noconfirm python-gobject gtk4 libadwaita
else
    echo "Distribution not recognized. Please install GTK4 and Libadwaita manually."
    echo "Required packages: python3-gi, gir1.2-gtk-4.0, gir1.2-adw-1, python3-gi-cairo"
fi

# Make main.py executable
chmod +x main.py

echo "Setup complete! Run './main.py' to start the installer."
