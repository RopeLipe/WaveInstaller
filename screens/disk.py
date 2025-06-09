"""
Disk Selection Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, GLib
import subprocess
import json

class DiskScreen:
    def __init__(self, app):
        self.app = app
        self.selected_disk = None
        self.selected_partition_scheme = "auto"
        self.disk_cards = []
        self.disks = []
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Disk Selection</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Choose where to install the system. All data on the selected disk will be erased.")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Warning box
        warning_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        warning_box.add_css_class("card")
        warning_box.set_spacing(12)
        warning_box.set_margin_bottom(20)
        warning_box.get_style_context().add_class("warning")
        
        warning_icon = Gtk.Image()
        warning_icon.set_from_icon_name("dialog-warning")
        warning_icon.set_pixel_size(24)
        warning_box.append(warning_icon)
        
        warning_text = Gtk.Label()
        warning_text.set_markup("<span weight='bold'>Warning:</span> This will permanently erase all data on the selected disk.")
        warning_text.set_wrap(True)
        warning_box.append(warning_text)
        
        content.append(warning_box)
        
        # Disk selection area
        disk_scrolled = Gtk.ScrolledWindow()
        disk_scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        disk_scrolled.set_min_content_height(300)
        
        self.disk_container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.disk_container.add_css_class("disk-grid")
        self.disk_container.set_spacing(16)
        
        # Load and display disks
        self.load_disks()
        
        disk_scrolled.set_child(self.disk_container)
        content.append(disk_scrolled)
        
        # Partition scheme options
        scheme_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        scheme_box.set_spacing(12)
        scheme_box.set_margin_top(20)
        
        scheme_label = Gtk.Label()
        scheme_label.set_markup("<span weight='bold'>Partition Scheme</span>")
        scheme_label.set_halign(Gtk.Align.START)
        scheme_box.append(scheme_label)
        
        # Partition scheme radio buttons
        self.scheme_auto = Gtk.CheckButton(label="Automatic (Recommended)")
        self.scheme_auto.set_active(True)
        self.scheme_auto.connect("toggled", lambda x: self.set_partition_scheme("auto"))
        scheme_box.append(self.scheme_auto)
        
        self.scheme_manual = Gtk.CheckButton(label="Manual partitioning")
        self.scheme_manual.set_group(self.scheme_auto)
        self.scheme_manual.connect("toggled", lambda x: self.set_partition_scheme("manual"))
        scheme_box.append(self.scheme_manual)
        
        scheme_desc = Gtk.Label()
        scheme_desc.set_text("Automatic partitioning will create optimal partitions for your system")
        scheme_desc.add_css_class("option-description")
        scheme_desc.set_halign(Gtk.Align.START)
        scheme_box.append(scheme_desc)
        
        content.append(scheme_box)
        
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
        next_btn.connect("clicked", lambda x: self.app.next_screen())        nav_box.append(next_btn)
        
        container.append(content)
        container.append(nav_box)
        
        return container
    
    def load_disks(self):
        """Load available disk drives"""
        # Mock disk data for demonstration
        # In a real implementation, you would use lsblk, fdisk, or similar
        self.disks = [
            {
                "device": "/dev/sda",
                "model": "Samsung SSD 970 EVO Plus",
                "size": "1TB",
                "size_bytes": 1000204886016,
                "type": "SSD",
                "interface": "NVMe",
                "used_space": 0.3,
                "partitions": []
            },
            {
                "device": "/dev/sdb", 
                "model": "Western Digital Blue",
                "size": "2TB",
                "size_bytes": 2000398934016,
                "type": "HDD",
                "interface": "SATA",
                "used_space": 0.6,
                "partitions": [
                    {"name": "/dev/sdb1", "size": "500GB", "type": "ntfs", "mount": "/mnt/windows"},
                    {"name": "/dev/sdb2", "size": "1.5TB", "type": "ext4", "mount": "/mnt/data"}
                ]
            },
            {
                "device": "/dev/sdc",
                "model": "Kingston DataTraveler",
                "size": "32GB", 
                "size_bytes": 32017047552,
                "type": "USB",
                "interface": "USB 3.0",
                "used_space": 0.1,
                "partitions": []
            }
        ]
        
        self.populate_disk_list()
    
    def populate_disk_list(self):
        """Populate the disk selection list"""
        # Clear existing cards
        child = self.disk_container.get_first_child()
        while child:
            self.disk_container.remove(child)
            child = self.disk_container.get_first_child()
        
        self.disk_cards = []
        
        for disk in self.disks:
            disk_card = self.create_disk_card(disk)
            self.disk_container.append(disk_card)
    
    def create_disk_card(self, disk):
        # Main card button
        card = Gtk.Button()
        card.add_css_class("disk-card")
        card.set_size_request(-1, 120)
        
        # Card content
        card_content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        card_content.set_spacing(12)
        card_content.set_margin_start(20)
        card_content.set_margin_end(20)
        card_content.set_margin_top(16)
        card_content.set_margin_bottom(16)
        
        # Disk info header
        info_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        info_box.set_spacing(16)
        info_box.add_css_class("disk-info")
        
        # Disk icon
        icon = Gtk.Image()
        if disk["type"] == "SSD":
            icon.set_from_icon_name("drive-harddisk-solidstate")
        elif disk["type"] == "USB":
            icon.set_from_icon_name("drive-removable-media")
        else:
            icon.set_from_icon_name("drive-harddisk")
        icon.set_pixel_size(32)
        icon.add_css_class("disk-icon")
        info_box.append(icon)
        
        # Disk details
        details_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        details_box.add_css_class("disk-details")
        details_box.set_valign(Gtk.Align.CENTER)
        
        # Model and device
        model_label = Gtk.Label()
        model_label.set_markup(f"<span weight='bold'>{disk['model']}</span>")
        model_label.set_halign(Gtk.Align.START)
        details_box.append(model_label)
        
        device_label = Gtk.Label()
        device_label.set_text(f"{disk['device']} • {disk['size']} • {disk['interface']}")
        device_label.set_halign(Gtk.Align.START)
        device_label.add_css_class("option-description")
        details_box.append(device_label)
        
        info_box.append(details_box)
        
        # Spacer
        spacer = Gtk.Box()
        spacer.set_hexpand(True)
        info_box.append(spacer)
        
        # Type badge
        type_label = Gtk.Label()
        type_label.set_text(disk["type"])
        type_label.add_css_class("option-description")
        info_box.append(type_label)
        
        card_content.append(info_box)
        
        # Usage bar
        if disk["used_space"] > 0:
            usage_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
            usage_box.set_spacing(4)
            
            usage_label = Gtk.Label()
            usage_label.set_text(f"Currently {int(disk['used_space'] * 100)}% used")
            usage_label.set_halign(Gtk.Align.START)
            usage_label.add_css_class("option-description")
            usage_box.append(usage_label)
            
            # Progress bar
            progress_container = Gtk.Box()
            progress_container.add_css_class("disk-bar")
            progress_container.set_size_request(-1, 8)
            
            progress_bar = Gtk.Box()
            progress_bar.add_css_class("disk-usage")
            progress_bar.set_size_request(int(300 * disk["used_space"]), -1)
            progress_container.append(progress_bar)
            
            usage_box.append(progress_container)
            card_content.append(usage_box)
        
        # Partition info
        if disk["partitions"]:
            part_label = Gtk.Label()
            part_count = len(disk["partitions"])
            part_label.set_text(f"{part_count} partition{'s' if part_count != 1 else ''} found")
            part_label.set_halign(Gtk.Align.START)
            part_label.add_css_class("option-description")
            card_content.append(part_label)
        
        card.set_child(card_content)
        
        # Connect click event
        card.connect("clicked", lambda x: self.select_disk(disk["device"], card))
        self.disk_cards.append(card)
        
        return card
    
    def select_disk(self, device, card):
        # Remove selection from all cards
        for disk_card in self.disk_cards:
            disk_card.remove_css_class("selected")
        
        # Add selection to clicked card
        card.add_css_class("selected")
        self.selected_disk = device
        
        print(f"Selected disk: {device}")
    
    def set_partition_scheme(self, scheme):
        self.selected_partition_scheme = scheme
        print(f"Selected partition scheme: {scheme}")
    
    def format_size(self, bytes_size):
        """Format byte size to human readable format"""
        for unit in ['B', 'KB', 'MB', 'GB', 'TB']:
            if bytes_size < 1024.0:
                return f"{bytes_size:.1f} {unit}"
            bytes_size /= 1024.0
        return f"{bytes_size:.1f} PB"
