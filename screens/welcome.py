"""
Welcome Screen - First screen of the installer
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk
import os

class WelcomeScreen:
    def __init__(self, app):
        self.app = app
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
          # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        content.set_valign(Gtk.Align.CENTER)
        content.set_vexpand(True)
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Welcome to Wave Installer</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Let's get your system set up quickly and easily")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        container.append(content)
        
        # Navigation buttons
        nav_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        nav_box.add_css_class("nav-buttons")
        nav_box.set_halign(Gtk.Align.FILL)
        nav_box.set_homogeneous(False)
        
        # Quit button
        quit_btn = Gtk.Button(label="Quit")
        quit_btn.add_css_class("secondary-button")
        quit_btn.connect("clicked", lambda x: self.app.quit_installer())
        nav_box.append(quit_btn)
        
        # Next button
        next_btn = Gtk.Button(label="Get Started")
        next_btn.add_css_class("primary-button")
        next_btn.connect("clicked", lambda x: self.app.next_screen())
        nav_box.append(next_btn)
        
        container.append(nav_box)
        
        return container
