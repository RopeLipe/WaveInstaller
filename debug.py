#!/usr/bin/env python3
"""
Diagnostic script to test Wave Installer startup
"""

import sys
import os
import traceback

print("Starting Wave Installer diagnostic...")

try:
    print("1. Testing GTK imports...")
    import gi
    gi.require_version('Gtk', '4.0')
    gi.require_version('Adw', '1')
    gi.require_version('Gdk', '4.0')
    from gi.repository import Gtk, Adw, Gdk, GLib, Gio
    print("✓ GTK imports successful")
    
    print("2. Testing screen imports...")
    
    # Test each screen import individually
    try:
        from screens.welcome import WelcomeScreen
        print("✓ WelcomeScreen imported")
    except Exception as e:
        print(f"✗ WelcomeScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.language import LanguageScreen
        print("✓ LanguageScreen imported")
    except Exception as e:
        print(f"✗ LanguageScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.timezone import TimezoneScreen
        print("✓ TimezoneScreen imported")
    except Exception as e:
        print(f"✗ TimezoneScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.keyboard import KeyboardScreen
        print("✓ KeyboardScreen imported")
    except Exception as e:
        print(f"✗ KeyboardScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.disk import DiskScreen
        print("✓ DiskScreen imported")
    except Exception as e:
        print(f"✗ DiskScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.network import NetworkScreen
        print("✓ NetworkScreen imported")
    except Exception as e:
        print(f"✗ NetworkScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.user import UserScreen
        print("✓ UserScreen imported")
    except Exception as e:
        print(f"✗ UserScreen failed: {e}")
        traceback.print_exc()
    
    try:
        from screens.install import InstallScreen
        print("✓ InstallScreen imported")
    except Exception as e:
        print(f"✗ InstallScreen failed: {e}")
        traceback.print_exc()
    
    print("3. Testing application creation...")
    
    class TestApp(Adw.Application):
        def __init__(self):
            super().__init__(application_id='com.wave.test')
            print("✓ Application initialized")
        
        def do_activate(self):
            print("✓ Application activated")
            
            # Create simple window
            window = Gtk.ApplicationWindow(application=self)
            window.set_title("Test")
            window.set_default_size(400, 300)
            
            label = Gtk.Label(label="Test successful!")
            window.set_child(label)
            window.present()
            print("✓ Window created and presented")
    
    print("4. Running test application...")
    app = TestApp()
    exit_code = app.run(sys.argv)
    print(f"Application exited with code: {exit_code}")
    
except Exception as e:
    print(f"✗ Critical error: {e}")
    traceback.print_exc()
    sys.exit(1)
