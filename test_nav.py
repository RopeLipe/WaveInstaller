#!/usr/bin/env python3
"""
Test navigation button positioning
"""

import gi
gi.require_version('Gtk', '4.0')
gi.require_version('Adw', '1')
from gi.repository import Gtk, Adw
import sys
import os

class TestNavButtons(Adw.Application):
    def __init__(self):
        super().__init__(application_id='com.test.nav')
        
    def do_activate(self):
        # Load CSS
        css_provider = Gtk.CssProvider()
        css_path = os.path.join(os.path.dirname(__file__), 'assets', 'style.css')
        css_provider.load_from_path(css_path)
        Gtk.StyleContext.add_provider_for_display(
            self.get_display(),
            css_provider,
            Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
        )
        
        # Create main window
        window = Gtk.ApplicationWindow(application=self)
        window.set_title("Navigation Test")
        window.set_default_size(900, 600)
        window.set_resizable(False)
        
        # Create test layout
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        content.set_vexpand(True)
        
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Test Content</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        subtitle = Gtk.Label()
        subtitle.set_text("This tests the navigation button positioning")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        container.append(content)
        
        # Navigation buttons
        nav_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        nav_box.add_css_class("nav-buttons")
        
        back_btn = Gtk.Button(label="Back")
        back_btn.add_css_class("secondary-button")
        nav_box.append(back_btn)
        
        next_btn = Gtk.Button(label="Continue")
        next_btn.add_css_class("primary-button")
        nav_box.append(next_btn)
        
        container.append(nav_box)
        window.set_child(container)
        window.present()

if __name__ == "__main__":
    app = TestNavButtons()
    app.run(sys.argv)
