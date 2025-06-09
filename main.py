#!/usr/bin/env python3
"""
WaveInstaller - Modern Linux Installer
Main application entry point
"""

import gi
gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')
gi.require_version('Gdk', '4.0')
from gi.repository import Gtk, Adw, Gdk, GLib, Gio
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
        # Create main window first
        self.main_window = Gtk.ApplicationWindow(application=self)
        self.main_window.set_title("")
        self.main_window.set_default_size(900, 600)
        self.main_window.set_resizable(False)
        
        # Load custom CSS after window creation
        self.load_css()
        
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
        
        try:
            css_provider.load_from_path(css_path)
            print(f"✓ CSS loaded from: {css_path}")
        except Exception as e:
            print(f"✗ Failed to load CSS from {css_path}: {e}")
            return
        
        Gtk.StyleContext.add_provider_for_display(
            self.main_window.get_display() if self.main_window else Gdk.Display.get_default(),
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
    try:
        app = WaveInstaller()
        return app.run(sys.argv)
    except Exception as e:
        print(f"Error starting Wave Installer: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == '__main__':
    sys.exit(main())
