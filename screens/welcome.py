"""
Welcome Screen - First screen of the installer
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, GdkPixbuf
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
        
        # Logo
        logo = Gtk.Image()
        logo.add_css_class("welcome-logo")
        try:
            logo_path = os.path.join(os.path.dirname(__file__), '..', 'assets', 'icons', 'logo.png')
            if os.path.exists(logo_path):
                logo.set_from_file(logo_path)
            else:
                logo.set_from_icon_name("application-x-executable")
                logo.set_pixel_size(120)
        except:
            logo.set_from_icon_name("application-x-executable")
            logo.set_pixel_size(120)
        
        content.append(logo)
        
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
        
        # Feature cards
        features_grid = Gtk.Grid()
        features_grid.add_css_class("feature-grid")
        features_grid.set_column_spacing(20)
        features_grid.set_row_spacing(20)
        features_grid.set_column_homogeneous(True)
        
        features = [
            {
                "icon": "preferences-system",
                "title": "Modern Interface",
                "description": "Clean and intuitive design that adapts to your system theme"
            },
            {
                "icon": "security-high",
                "title": "Secure Setup",
                "description": "Built-in security features and encrypted installation options"
            },
            {
                "icon": "applications-utilities",
                "title": "Easy Configuration",
                "description": "Guided setup process with intelligent defaults"
            }
        ]
        
        for i, feature in enumerate(features):
            card = self.create_feature_card(feature)
            features_grid.attach(card, i % 3, i // 3, 1, 1)
        
        content.append(features_grid)
        
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
        
        # Spacer
        spacer = Gtk.Box()
        spacer.set_hexpand(True)
        nav_box.append(spacer)
        
        # Next button
        next_btn = Gtk.Button(label="Get Started")
        next_btn.add_css_class("primary-button")
        next_btn.connect("clicked", lambda x: self.app.next_screen())
        nav_box.append(next_btn)
        
        content.append(nav_box)
        container.append(content)
        
        return container
    
    def create_feature_card(self, feature):
        card = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        card.add_css_class("feature-card")
        card.set_spacing(12)
        
        # Icon
        icon = Gtk.Image()
        icon.set_from_icon_name(feature["icon"])
        icon.set_pixel_size(48)
        icon.add_css_class("feature-icon")
        card.append(icon)
        
        # Title
        title = Gtk.Label()
        title.set_markup(f"<span weight='bold'>{feature['title']}</span>")
        title.add_css_class("feature-title")
        card.append(title)
        
        # Description
        desc = Gtk.Label()
        desc.set_text(feature["description"])
        desc.add_css_class("feature-description")
        desc.set_wrap(True)
        desc.set_justify(Gtk.Justification.CENTER)
        card.append(desc)
        
        return card
