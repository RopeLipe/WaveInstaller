#!/usr/bin/env python3
"""
Test script for KeyboardScreen to verify variant_box issue is fixed
"""

import gi
gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')
gi.require_version('Gdk', '4.0')
from gi.repository import Gtk, Adw, Gdk, GLib, Gio
import sys
import os

# Add screens directory to path
sys.path.append(os.path.dirname(__file__))

from screens.keyboard import KeyboardScreen

class TestApp(Adw.Application):
    def __init__(self):
        super().__init__(application_id='com.wave.test')
        
    def do_activate(self):
        # Create main window
        self.main_window = Gtk.ApplicationWindow(application=self)
        self.main_window.set_title("Keyboard Test")
        self.main_window.set_default_size(900, 600)
        
        # Create keyboard screen
        keyboard_screen = KeyboardScreen(self)
        widget = keyboard_screen.create_widget()
        
        self.main_window.set_child(widget)
        self.main_window.present()
        
    def next_screen(self):
        print("Next screen called")
        
    def previous_screen(self):
        print("Previous screen called")
        
    def quit_installer(self):
        self.quit()

if __name__ == "__main__":
    app = TestApp()
    app.run(sys.argv)
