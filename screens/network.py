"""
Network Configuration Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, GLib
import subprocess
import json

class NetworkScreen:
    def __init__(self, app):
        self.app = app
        self.selected_connection = None
        self.network_cards = []
        self.networks = []
        self.wifi_password_entry = None
        self.connection_type = "ethernet"  # ethernet or wifi
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Network Configuration</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Connect to the internet to download updates during installation")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Connection type tabs
        tab_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        tab_box.set_spacing(8)
        tab_box.set_halign(Gtk.Align.CENTER)
        tab_box.set_margin_bottom(20)
        
        # Ethernet tab
        self.ethernet_tab = Gtk.Button(label="Ethernet")
        self.ethernet_tab.add_css_class("primary-button")
        self.ethernet_tab.connect("clicked", lambda x: self.switch_tab("ethernet"))
        tab_box.append(self.ethernet_tab)
        
        # WiFi tab
        self.wifi_tab = Gtk.Button(label="Wi-Fi")
        self.wifi_tab.add_css_class("secondary-button")
        self.wifi_tab.connect("clicked", lambda x: self.switch_tab("wifi"))
        tab_box.append(self.wifi_tab)
        
        # Skip button
        skip_tab = Gtk.Button(label="Skip")
        skip_tab.add_css_class("secondary-button")
        skip_tab.connect("clicked", lambda x: self.skip_network())
        tab_box.append(skip_tab)
        
        content.append(tab_box)
        
        # Network list container
        self.network_scrolled = Gtk.ScrolledWindow()
        self.network_scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        self.network_scrolled.set_min_content_height(300)
        
        self.network_container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.network_container.add_css_class("option-list")
        self.network_container.set_spacing(12)
        
        self.network_scrolled.set_child(self.network_container)
        content.append(self.network_scrolled)
        
        # WiFi password section (initially hidden)
        self.wifi_password_section = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.wifi_password_section.set_spacing(12)
        self.wifi_password_section.set_margin_top(20)
        self.wifi_password_section.set_visible(False)
        
        password_label = Gtk.Label()
        password_label.set_markup("<span weight='bold'>Wi-Fi Password</span>")
        password_label.set_halign(Gtk.Align.START)
        self.wifi_password_section.append(password_label)
        
        password_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        password_box.set_spacing(12)
        
        self.wifi_password_entry = Gtk.Entry()
        self.wifi_password_entry.set_placeholder_text("Enter Wi-Fi password...")
        self.wifi_password_entry.set_visibility(False)  # Hide password
        self.wifi_password_entry.add_css_class("form-input")
        self.wifi_password_entry.set_hexpand(True)
        password_box.append(self.wifi_password_entry)
        
        # Show/hide password button
        show_password_btn = Gtk.Button()
        show_password_btn.set_icon_name("view-reveal")
        show_password_btn.add_css_class("secondary-button")
        show_password_btn.connect("clicked", self.toggle_password_visibility)
        password_box.append(show_password_btn)
        
        self.wifi_password_section.append(password_box)
        
        # Connection test button
        test_btn = Gtk.Button(label="Test Connection")
        test_btn.add_css_class("secondary-button")
        test_btn.connect("clicked", self.test_connection)
        self.wifi_password_section.append(test_btn)
        
        content.append(self.wifi_password_section)
        
        # Connection status
        self.status_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        self.status_box.set_spacing(12)
        self.status_box.set_margin_top(20)
        self.status_box.set_visible(False)
        
        self.status_icon = Gtk.Image()
        self.status_icon.set_pixel_size(24)
        self.status_box.append(self.status_icon)
        
        self.status_label = Gtk.Label()
        self.status_label.add_css_class("option-title")
        self.status_box.append(self.status_label)
        
        content.append(self.status_box)
        
        # Load initial network data
        self.load_networks()
        
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
        
        # Next button
        next_btn = Gtk.Button(label="Continue")
        next_btn.add_css_class("primary-button")
        next_btn.connect("clicked", lambda x: self.app.next_screen())
        nav_box.append(next_btn)
        
        content.append(nav_box)
        container.append(content)
        
        return container
    
    def switch_tab(self, tab_type):
        """Switch between ethernet and wifi tabs"""
        self.connection_type = tab_type
        
        # Update tab styling
        if tab_type == "ethernet":
            self.ethernet_tab.remove_css_class("secondary-button")
            self.ethernet_tab.add_css_class("primary-button")
            self.wifi_tab.remove_css_class("primary-button")
            self.wifi_tab.add_css_class("secondary-button")
            self.wifi_password_section.set_visible(False)
        else:
            self.wifi_tab.remove_css_class("secondary-button")
            self.wifi_tab.add_css_class("primary-button")
            self.ethernet_tab.remove_css_class("primary-button")
            self.ethernet_tab.add_css_class("secondary-button")
        
        # Reload networks for selected type
        self.load_networks()
    
    def load_networks(self):
        """Load available network connections"""
        if self.connection_type == "ethernet":
            self.networks = [
                {
                    "name": "Ethernet Connection",
                    "type": "ethernet",
                    "status": "connected",
                    "signal": 100,
                    "secured": False,
                    "ip": "192.168.1.100"
                },
                {
                    "name": "Ethernet Connection 2",
                    "type": "ethernet", 
                    "status": "available",
                    "signal": 100,
                    "secured": False,
                    "ip": None
                }
            ]
        else:
            self.networks = [
                {
                    "name": "Home_WiFi_5G",
                    "type": "wifi",
                    "status": "available",
                    "signal": 85,
                    "secured": True,
                    "ip": None
                },
                {
                    "name": "Home_WiFi_2.4G",
                    "type": "wifi",
                    "status": "available", 
                    "signal": 70,
                    "secured": True,
                    "ip": None
                },
                {
                    "name": "Neighbor_WiFi",
                    "type": "wifi",
                    "status": "available",
                    "signal": 45,
                    "secured": True,
                    "ip": None
                },
                {
                    "name": "Public_WiFi",
                    "type": "wifi",
                    "status": "available",
                    "signal": 30,
                    "secured": False,
                    "ip": None
                }
            ]
        
        self.populate_network_list()
    
    def populate_network_list(self):
        """Populate the network list"""
        # Clear existing cards
        child = self.network_container.get_first_child()
        while child:
            self.network_container.remove(child)
            child = self.network_container.get_first_child()
        
        self.network_cards = []
        
        for network in self.networks:
            network_card = self.create_network_card(network)
            self.network_container.append(network_card)
    
    def create_network_card(self, network):
        # Main card button
        card = Gtk.Button()
        card.add_css_class("option-item")
        card.set_size_request(-1, 70)
        
        # Card content
        card_content = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        card_content.set_spacing(16)
        card_content.set_margin_start(16)
        card_content.set_margin_end(16)
        card_content.set_margin_top(12)
        card_content.set_margin_bottom(12)
        
        # Network icon
        icon = Gtk.Image()
        if network["type"] == "wifi":
            if network["signal"] > 70:
                icon.set_from_icon_name("network-wireless-signal-excellent")
            elif network["signal"] > 50:
                icon.set_from_icon_name("network-wireless-signal-good")
            elif network["signal"] > 30:
                icon.set_from_icon_name("network-wireless-signal-ok")
            else:
                icon.set_from_icon_name("network-wireless-signal-weak")
        else:
            icon.set_from_icon_name("network-wired")
        
        icon.set_pixel_size(24)
        icon.add_css_class("option-icon")
        card_content.append(icon)
        
        # Network details
        details_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        details_box.set_valign(Gtk.Align.CENTER)
        
        # Network name
        name_label = Gtk.Label()
        name_label.set_markup(f"<span weight='bold'>{network['name']}</span>")
        name_label.set_halign(Gtk.Align.START)
        name_label.add_css_class("option-title")
        details_box.append(name_label)
        
        # Network info
        info_parts = []
        if network["type"] == "wifi":
            info_parts.append(f"Signal: {network['signal']}%")
            if network["secured"]:
                info_parts.append("Secured")
            else:
                info_parts.append("Open")
        else:
            info_parts.append("Wired connection")
        
        if network["status"] == "connected":
            info_parts.append(f"Connected ({network['ip']})")
        
        info_label = Gtk.Label()
        info_label.set_text(" • ".join(info_parts))
        info_label.set_halign(Gtk.Align.START)
        info_label.add_css_class("option-description")
        details_box.append(info_label)
        
        card_content.append(details_box)
        
        # Spacer
        spacer = Gtk.Box()
        spacer.set_hexpand(True)
        card_content.append(spacer)
        
        # Status indicator
        if network["status"] == "connected":
            status_icon = Gtk.Image()
            status_icon.set_from_icon_name("emblem-ok")
            status_icon.set_pixel_size(20)
            card_content.append(status_icon)
        elif network["secured"] and network["type"] == "wifi":
            lock_icon = Gtk.Image()
            lock_icon.set_from_icon_name("security-high")
            lock_icon.set_pixel_size(20)
            card_content.append(lock_icon)
        
        card.set_child(card_content)
        
        # Connect click event
        card.connect("clicked", lambda x: self.select_network(network, card))
        self.network_cards.append(card)
        
        return card
    
    def select_network(self, network, card):
        # Remove selection from all cards
        for network_card in self.network_cards:
            network_card.remove_css_class("selected")
        
        # Add selection to clicked card
        card.add_css_class("selected")
        self.selected_connection = network
        
        # Show password section for secured WiFi
        if network["type"] == "wifi" and network["secured"] and network["status"] != "connected":
            self.wifi_password_section.set_visible(True)
        else:
            self.wifi_password_section.set_visible(False)
        
        print(f"Selected network: {network['name']}")
    
    def toggle_password_visibility(self, button):
        """Toggle password visibility"""
        if self.wifi_password_entry.get_visibility():
            self.wifi_password_entry.set_visibility(False)
            button.set_icon_name("view-reveal")
        else:
            self.wifi_password_entry.set_visibility(True)
            button.set_icon_name("view-conceal")
    
    def test_connection(self, button):
        """Test network connection"""
        if not self.selected_connection:
            return
        
        # Show connecting status
        self.show_status("network-wireless-acquiring", "Testing connection...", "info")
        
        # Simulate connection test
        GLib.timeout_add(2000, self.connection_test_complete)
    
    def connection_test_complete(self):
        """Complete connection test"""
        # Simulate successful connection
        self.show_status("emblem-ok", "Connected successfully!", "success")
        return False  # Don't repeat timeout
    
    def show_status(self, icon_name, message, status_type):
        """Show connection status"""
        self.status_icon.set_from_icon_name(icon_name)
        self.status_label.set_text(message)
        
        # Remove existing status classes
        self.status_box.remove_css_class("success")
        self.status_box.remove_css_class("error")
        self.status_box.remove_css_class("info")
        
        # Add appropriate status class
        self.status_box.add_css_class(status_type)
        self.status_box.set_visible(True)
    
    def skip_network(self):
        """Skip network configuration"""
        self.selected_connection = None
        self.wifi_password_section.set_visible(False)
        self.status_box.set_visible(False)
        
        # Clear selections
        for card in self.network_cards:
            card.remove_css_class("selected")
        
        print("Skipped network configuration")
