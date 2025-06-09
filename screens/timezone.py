"""
Timezone Selection Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk
import subprocess
import os

class TimezoneScreen:
    def __init__(self, app):
        self.app = app
        self.selected_timezone = None
        self.timezone_buttons = []
        self.search_entry = None
        self.timezone_list = []
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Select Your Timezone</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Choose your timezone to set the correct system time")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Search box
        search_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        search_box.set_spacing(12)
        search_box.set_margin_bottom(20)
        
        search_icon = Gtk.Image()
        search_icon.set_from_icon_name("system-search")
        search_icon.set_pixel_size(20)
        search_box.append(search_icon)
        
        self.search_entry = Gtk.Entry()
        self.search_entry.set_placeholder_text("Search for your city or timezone...")
        self.search_entry.add_css_class("form-input")
        self.search_entry.set_hexpand(True)
        self.search_entry.connect("changed", self.on_search_changed)
        search_box.append(self.search_entry)
        
        content.append(search_box)
        
        # Timezone list
        scrolled = Gtk.ScrolledWindow()
        scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        scrolled.set_min_content_height(350)
        
        self.timezone_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.timezone_box.add_css_class("option-list")
        self.timezone_box.set_spacing(8)
        
        # Load and display timezones
        self.load_timezones()
        self.populate_timezone_list()
        
        scrolled.set_child(self.timezone_box)
        content.append(scrolled)
        
        # Navigation buttons
        nav_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        nav_box.add_css_class("nav-buttons")
        nav_box.set_halign(Gtk.Align.FILL)
        nav_box.set_homogeneous(False)
        
        # Back button
        back_btn = Gtk.Button(label="Back")
        back_btn.add_css_class("secondary-button")
        back_btn.connect("clicked", lambda x: self.app.previous_screen())
        nav_box.append(back_btn)
        
        # Spacer
        spacer = Gtk.Box()
        spacer.set_hexpand(True)
        nav_box.append(spacer)
        
        # Next button
        next_btn = Gtk.Button(label="Continue")
        next_btn.add_css_class("primary-button")
        next_btn.connect("clicked", lambda x: self.app.next_screen())
        nav_box.append(next_btn)
        
        content.append(nav_box)
        container.append(content)
        
        return container
    
    def load_timezones(self):
        """Load available timezones"""
        # Common timezones with their display names
        self.timezone_list = [
            {"zone": "UTC", "display": "UTC (Coordinated Universal Time)", "region": "UTC"},
            {"zone": "America/New_York", "display": "New York (Eastern Time)", "region": "Americas"},
            {"zone": "America/Chicago", "display": "Chicago (Central Time)", "region": "Americas"},
            {"zone": "America/Denver", "display": "Denver (Mountain Time)", "region": "Americas"},
            {"zone": "America/Los_Angeles", "display": "Los Angeles (Pacific Time)", "region": "Americas"},
            {"zone": "America/Toronto", "display": "Toronto (Eastern Time)", "region": "Americas"},
            {"zone": "America/Mexico_City", "display": "Mexico City", "region": "Americas"},
            {"zone": "America/Sao_Paulo", "display": "São Paulo", "region": "Americas"},
            {"zone": "Europe/London", "display": "London (GMT/BST)", "region": "Europe"},
            {"zone": "Europe/Paris", "display": "Paris (CET/CEST)", "region": "Europe"},
            {"zone": "Europe/Berlin", "display": "Berlin (CET/CEST)", "region": "Europe"},
            {"zone": "Europe/Rome", "display": "Rome (CET/CEST)", "region": "Europe"},
            {"zone": "Europe/Madrid", "display": "Madrid (CET/CEST)", "region": "Europe"},
            {"zone": "Europe/Amsterdam", "display": "Amsterdam (CET/CEST)", "region": "Europe"},
            {"zone": "Europe/Moscow", "display": "Moscow (MSK)", "region": "Europe"},
            {"zone": "Asia/Tokyo", "display": "Tokyo (JST)", "region": "Asia"},
            {"zone": "Asia/Shanghai", "display": "Shanghai (CST)", "region": "Asia"},
            {"zone": "Asia/Hong_Kong", "display": "Hong Kong (HKT)", "region": "Asia"},
            {"zone": "Asia/Singapore", "display": "Singapore (SGT)", "region": "Asia"},
            {"zone": "Asia/Seoul", "display": "Seoul (KST)", "region": "Asia"},
            {"zone": "Asia/Dubai", "display": "Dubai (GST)", "region": "Asia"},
            {"zone": "Asia/Kolkata", "display": "Mumbai/Kolkata (IST)", "region": "Asia"},
            {"zone": "Australia/Sydney", "display": "Sydney (AEST/AEDT)", "region": "Australia"},
            {"zone": "Australia/Melbourne", "display": "Melbourne (AEST/AEDT)", "region": "Australia"},
            {"zone": "Australia/Perth", "display": "Perth (AWST)", "region": "Australia"},
            {"zone": "Pacific/Auckland", "display": "Auckland (NZST/NZDT)", "region": "Pacific"},
            {"zone": "Africa/Cairo", "display": "Cairo (EET)", "region": "Africa"},
            {"zone": "Africa/Johannesburg", "display": "Johannesburg (SAST)", "region": "Africa"},
        ]
        
        # Try to detect current timezone
        try:
            result = subprocess.run(['timedatectl', 'show', '--property=Timezone', '--value'], 
                                  capture_output=True, text=True)
            if result.returncode == 0:
                current_tz = result.stdout.strip()
                # Find and select current timezone
                for tz in self.timezone_list:
                    if tz["zone"] == current_tz:
                        self.selected_timezone = current_tz
                        break
        except:
            pass
        
        # Default to UTC if no timezone detected
        if not self.selected_timezone:
            self.selected_timezone = "UTC"
    
    def populate_timezone_list(self, filter_text=""):
        """Populate the timezone list with optional filtering"""
        # Clear existing items
        child = self.timezone_box.get_first_child()
        while child:
            self.timezone_box.remove(child)
            child = self.timezone_box.get_first_child()
        
        self.timezone_buttons = []
        
        # Filter timezones
        filtered_timezones = []
        for tz in self.timezone_list:
            if (filter_text.lower() in tz["display"].lower() or 
                filter_text.lower() in tz["zone"].lower()):
                filtered_timezones.append(tz)
        
        # Group by region
        regions = {}
        for tz in filtered_timezones:
            region = tz["region"]
            if region not in regions:
                regions[region] = []
            regions[region].append(tz)
        
        # Add region headers and timezones
        for region, timezones in regions.items():
            # Region header
            if len(regions) > 1:  # Only show headers if multiple regions
                header = Gtk.Label()
                header.set_markup(f"<span weight='bold' size='small'>{region.upper()}</span>")
                header.set_halign(Gtk.Align.START)
                header.set_margin_top(16)
                header.set_margin_bottom(8)
                header.add_css_class("option-description")
                self.timezone_box.append(header)
            
            # Timezone items
            for tz in timezones:
                tz_item = self.create_timezone_item(tz)
                self.timezone_box.append(tz_item)
                
                # Select current timezone
                if tz["zone"] == self.selected_timezone:
                    self.select_timezone(tz["zone"], tz_item)
    
    def create_timezone_item(self, timezone):
        button = Gtk.Button()
        button.add_css_class("option-item")
        button.set_size_request(-1, 60)
        
        # Content box
        content_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        content_box.set_spacing(16)
        content_box.set_margin_start(16)
        content_box.set_margin_end(16)
        content_box.set_margin_top(12)
        content_box.set_margin_bottom(12)
        
        # Clock icon
        icon = Gtk.Image()
        icon.set_from_icon_name("preferences-system-time")
        icon.set_pixel_size(24)
        icon.add_css_class("option-icon")
        content_box.append(icon)
        
        # Timezone info
        tz_info = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        tz_info.set_valign(Gtk.Align.CENTER)
        
        # Display name
        display_label = Gtk.Label()
        display_label.set_markup(f"<span weight='bold'>{timezone['display']}</span>")
        display_label.set_halign(Gtk.Align.START)
        display_label.add_css_class("option-title")
        tz_info.append(display_label)
        
        # Zone name
        zone_label = Gtk.Label()
        zone_label.set_text(timezone["zone"])
        zone_label.set_halign(Gtk.Align.START)
        zone_label.add_css_class("option-description")
        tz_info.append(zone_label)
        
        content_box.append(tz_info)
        button.set_child(content_box)
        
        # Connect click event
        button.connect("clicked", lambda x: self.select_timezone(timezone["zone"], button))
        self.timezone_buttons.append(button)
        
        return button
    
    def select_timezone(self, timezone, button):
        # Remove selection from all buttons
        for btn in self.timezone_buttons:
            btn.remove_css_class("selected")
        
        # Add selection to clicked button
        button.add_css_class("selected")
        self.selected_timezone = timezone
        
        print(f"Selected timezone: {timezone}")
    
    def on_search_changed(self, entry):
        """Handle search text changes"""
        search_text = entry.get_text()
        self.populate_timezone_list(search_text)
