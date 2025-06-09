#!/usr/bin/env python3
"""
WaveInstaller - Modern Linux Installer
Main application entry point
"""

import gi
gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')
from gi.repository import Gtk, Adw, GLib, Gio
import sys
import os

# Import all screens
from screens.welcome import WelcomeScreen
from screens.language import LanguageScreen
from screens.timezone import TimezoneScreen
from screens.keyboard import KeyboardScreen
from screens.disk import DiskScreen
from screens.network import NetworkScreen
from screens.user import UserScreen
from screens.install import InstallScreen

class WaveInstaller(Adw.Application):
    def __init__(self):
        super().__init__(application_id='com.wave.installer')
        self.current_screen_index = 0
        self.screens = []
        self.main_window = None
        
    def do_activate(self):
        # Load custom CSS
        self.load_css()
        
        # Create main window
        self.main_window = Gtk.ApplicationWindow(application=self)
        self.main_window.set_title("Wave Installer")
        self.main_window.set_default_size(1000, 700)
        self.main_window.set_resizable(False)
        
        # Remove titlebar
        self.main_window.set_titlebar(Gtk.HeaderBar())
        headerbar = self.main_window.get_titlebar()
        headerbar.set_show_title_buttons(False)
        headerbar.add_css_class("hidden-headerbar")
        
        # Create main container
        self.main_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.main_window.set_child(self.main_box)
        
        # Initialize screens
        self.init_screens()
        
        # Show first screen
        self.show_screen(0)
        
        self.main_window.present()
    
    def load_css(self):
        """Load custom CSS styling"""
        css_provider = Gtk.CssProvider()
        css_path = os.path.join(os.path.dirname(__file__), 'assets', 'style.css')
        css_provider.load_from_path(css_path)
        
        Gtk.StyleContext.add_provider_for_display(
            self.main_window.get_display() if self.main_window else Gtk.gdk.Display.get_default(),
            css_provider,
            Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
        )
    
    def init_screens(self):
        """Initialize all installer screens"""
        self.screens = [
            WelcomeScreen(self),
            LanguageScreen(self),
            TimezoneScreen(self),
            KeyboardScreen(self),
            DiskScreen(self),
            NetworkScreen(self),
            UserScreen(self),
            InstallScreen(self)
        ]
    
    def show_screen(self, index):
        """Show a specific screen"""
        if 0 <= index < len(self.screens):
            # Clear current content
            child = self.main_box.get_first_child()
            if child:
                self.main_box.remove(child)
            
            # Add new screen
            self.current_screen_index = index
            screen_widget = self.screens[index].create_widget()
            self.main_box.append(screen_widget)
    
    def next_screen(self):
        """Navigate to next screen"""
        if self.current_screen_index < len(self.screens) - 1:
            self.show_screen(self.current_screen_index + 1)
    
    def previous_screen(self):
        """Navigate to previous screen"""
        if self.current_screen_index > 0:
            self.show_screen(self.current_screen_index - 1)
    
    def quit_installer(self):
        """Quit the installer"""
        self.quit()

def main():
    app = WaveInstaller()
    return app.run(sys.argv)

if __name__ == '__main__':
    main()
