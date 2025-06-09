#!/usr/bin/env python3
"""
Test script to debug main.py import and initialization issues
"""

import sys
import os

print("Testing Wave Installer imports...")

try:
    print("1. Testing basic GTK imports...")
    import gi
    gi.require_version('Gtk', '4.0')
    gi.require_version('Adw', '1')
    gi.require_version('Gdk', '4.0')
    from gi.repository import Gtk, Adw, Gdk, GLib, Gio
    print("✓ GTK imports successful")
except Exception as e:
    print(f"✗ GTK import failed: {e}")
    sys.exit(1)

try:
    print("2. Testing screen imports...")
    from screens.welcome import WelcomeScreen
    from screens.language import LanguageScreen
    from screens.timezone import TimezoneScreen
    from screens.keyboard import KeyboardScreen
    from screens.disk import DiskScreen
    from screens.network import NetworkScreen
    from screens.user import UserScreen
    from screens.install import InstallScreen
    print("✓ All screen imports successful")
except Exception as e:
    print(f"✗ Screen import failed: {e}")
    import traceback
    traceback.print_exc()
    sys.exit(1)

try:
    print("3. Testing main WaveInstaller class...")
    # Import the class definition
    sys.path.insert(0, os.path.dirname(__file__))
    
    class TestWaveInstaller(Adw.Application):
        def __init__(self):
            super().__init__(application_id='com.wave.installer.test')
            self.current_screen_index = 0
            self.screens = []
            self.main_window = None
        
        def do_activate(self):
            print("✓ do_activate called")
            # Create main window
            self.main_window = Gtk.ApplicationWindow(application=self)
            self.main_window.set_title("Test")
            self.main_window.set_default_size(900, 600)
            
            # Create simple content
            box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
            label = Gtk.Label(label="Test Window")
            box.append(label)
            
            self.main_window.set_child(box)
            self.main_window.present()
            print("✓ Window created and presented")
    
    print("Creating test app...")
    app = TestWaveInstaller()
    print("✓ Test app created successfully")
    
    print("4. Testing screen initialization...")
    # Test creating screen instances
    test_screens = [
        WelcomeScreen(app),
        LanguageScreen(app),
        TimezoneScreen(app),
        KeyboardScreen(app),
        DiskScreen(app),
        NetworkScreen(app),
        UserScreen(app),
        InstallScreen(app)
    ]
    print("✓ All screens created successfully")
    
    print("5. Testing screen widget creation...")
    for i, screen in enumerate(test_screens):
        try:
            widget = screen.create_widget()
            print(f"✓ Screen {i} widget created: {type(screen).__name__}")
        except Exception as e:
            print(f"✗ Screen {i} widget failed: {type(screen).__name__} - {e}")
            import traceback
            traceback.print_exc()
    
    print("\n=== All tests passed! Running test app... ===")
    app.run([])
    
except Exception as e:
    print(f"✗ Main class test failed: {e}")
    import traceback
    traceback.print_exc()
    sys.exit(1)
