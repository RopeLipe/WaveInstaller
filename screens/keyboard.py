"""
Keyboard Layout Selection Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk
import subprocess

class KeyboardScreen:
    def __init__(self, app):
        self.app = app
        self.selected_layout = None
        self.selected_variant = None
        self.layout_buttons = []
        self.variant_buttons = []
        self.test_entry = None
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Keyboard Layout</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Select your keyboard layout and test it below")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Two column layout
        columns = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        columns.set_spacing(30)
        columns.set_homogeneous(True)
        
        # Left column - Keyboard layouts
        left_column = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        left_column.set_spacing(16)
        
        layout_label = Gtk.Label()
        layout_label.set_markup("<span weight='bold'>Keyboard Layout</span>")
        layout_label.set_halign(Gtk.Align.START)
        left_column.append(layout_label)
        
        # Layout list
        layout_scrolled = Gtk.ScrolledWindow()
        layout_scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        layout_scrolled.set_min_content_height(300)
        
        self.layout_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.layout_box.add_css_class("option-list")
        self.layout_box.set_spacing(8)
        
        # Load keyboard layouts
        self.load_keyboard_layouts()
        
        layout_scrolled.set_child(self.layout_box)
        left_column.append(layout_scrolled)
        
        columns.append(left_column)
        
        # Right column - Variants and test
        right_column = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        right_column.set_spacing(16)
        
        variant_label = Gtk.Label()
        variant_label.set_markup("<span weight='bold'>Layout Variant</span>")
        variant_label.set_halign(Gtk.Align.START)
        right_column.append(variant_label)
        
        # Variant list
        variant_scrolled = Gtk.ScrolledWindow()
        variant_scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        variant_scrolled.set_min_content_height(200)
        
        self.variant_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.variant_box.add_css_class("option-list")
        self.variant_box.set_spacing(8)
        
        variant_scrolled.set_child(self.variant_box)
        right_column.append(variant_scrolled)
        
        # Test area
        test_label = Gtk.Label()
        test_label.set_markup("<span weight='bold'>Test Your Keyboard</span>")
        test_label.set_halign(Gtk.Align.START)
        test_label.set_margin_top(20)
        right_column.append(test_label)
        
        self.test_entry = Gtk.Entry()
        self.test_entry.set_placeholder_text("Type here to test your keyboard layout...")
        self.test_entry.add_css_class("form-input")
        self.test_entry.set_size_request(-1, 60)
        right_column.append(self.test_entry)
        
        test_info = Gtk.Label()
        test_info.set_text("Try typing special characters and symbols")
        test_info.add_css_class("option-description")
        test_info.set_halign(Gtk.Align.START)
        right_column.append(test_info)
        
        columns.append(right_column)
        content.append(columns)
        
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
    
    def load_keyboard_layouts(self):
        """Load available keyboard layouts"""
        layouts = [
            {"code": "us", "name": "English (US)", "flag": "🇺🇸"},
            {"code": "gb", "name": "English (UK)", "flag": "🇬🇧"},
            {"code": "de", "name": "German", "flag": "🇩🇪"},
            {"code": "fr", "name": "French", "flag": "🇫🇷"},
            {"code": "es", "name": "Spanish", "flag": "🇪🇸"},
            {"code": "it", "name": "Italian", "flag": "🇮🇹"},
            {"code": "pt", "name": "Portuguese", "flag": "🇵🇹"},
            {"code": "ru", "name": "Russian", "flag": "🇷🇺"},
            {"code": "jp", "name": "Japanese", "flag": "🇯🇵"},
            {"code": "kr", "name": "Korean", "flag": "🇰🇷"},
            {"code": "cn", "name": "Chinese", "flag": "🇨🇳"},
            {"code": "ar", "name": "Arabic", "flag": "🇸🇦"},
            {"code": "dvorak", "name": "Dvorak", "flag": "⌨️"},
            {"code": "colemak", "name": "Colemak", "flag": "⌨️"},
        ]
        
        for layout in layouts:
            layout_item = self.create_layout_item(layout)
            self.layout_box.append(layout_item)
        
        # Select US layout by default
        if self.layout_buttons:
            self.select_layout("us", self.layout_buttons[0])
    
    def create_layout_item(self, layout):
        button = Gtk.Button()
        button.add_css_class("option-item")
        button.set_size_request(-1, 50)
        
        # Content box
        content_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        content_box.set_spacing(12)
        content_box.set_margin_start(16)
        content_box.set_margin_end(16)
        content_box.set_margin_top(8)
        content_box.set_margin_bottom(8)
        
        # Flag/Icon
        flag_label = Gtk.Label()
        flag_label.set_text(layout["flag"])
        flag_label.set_size_request(24, -1)
        content_box.append(flag_label)
        
        # Layout name
        name_label = Gtk.Label()
        name_label.set_text(layout["name"])
        name_label.set_halign(Gtk.Align.START)
        name_label.add_css_class("option-title")
        content_box.append(name_label)
        
        button.set_child(content_box)
        
        # Connect click event
        button.connect("clicked", lambda x: self.select_layout(layout["code"], button))
        self.layout_buttons.append(button)
        
        return button
    
    def select_layout(self, layout_code, button):
        # Remove selection from all layout buttons
        for btn in self.layout_buttons:
            btn.remove_css_class("selected")
        
        # Add selection to clicked button
        button.add_css_class("selected")
        self.selected_layout = layout_code
        
        # Load variants for selected layout
        self.load_layout_variants(layout_code)
        
        print(f"Selected keyboard layout: {layout_code}")
    
    def load_layout_variants(self, layout_code):
        """Load variants for the selected layout"""
        # Clear existing variants
        child = self.variant_box.get_first_child()
        while child:
            self.variant_box.remove(child)
            child = self.variant_box.get_first_child()
        
        self.variant_buttons = []
        
        # Layout-specific variants
        variants = {
            "us": [
                {"code": "", "name": "Default"},
                {"code": "intl", "name": "International"},
                {"code": "altgr-intl", "name": "International (AltGr)"},
                {"code": "dvorak", "name": "Dvorak"},
                {"code": "colemak", "name": "Colemak"}
            ],
            "gb": [
                {"code": "", "name": "Default"},
                {"code": "extd", "name": "Extended"},
                {"code": "intl", "name": "International"}
            ],
            "de": [
                {"code": "", "name": "Default"},
                {"code": "nodeadkeys", "name": "No dead keys"},
                {"code": "neo", "name": "Neo"}
            ],
            "fr": [
                {"code": "", "name": "Default"},
                {"code": "nodeadkeys", "name": "No dead keys"},
                {"code": "bepo", "name": "BÉPO"}
            ]
        }
        
        # Get variants for current layout or default
        layout_variants = variants.get(layout_code, [{"code": "", "name": "Default"}])
        
        for variant in layout_variants:
            variant_item = self.create_variant_item(variant)
            self.variant_box.append(variant_item)
        
        # Select default variant
        if self.variant_buttons:
            self.select_variant("", self.variant_buttons[0])
    
    def create_variant_item(self, variant):
        button = Gtk.Button()
        button.add_css_class("option-item")
        button.set_size_request(-1, 40)
        
        # Content box
        content_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        content_box.set_spacing(12)
        content_box.set_margin_start(16)
        content_box.set_margin_end(16)
        content_box.set_margin_top(6)
        content_box.set_margin_bottom(6)
        
        # Variant name
        name_label = Gtk.Label()
        name_label.set_text(variant["name"])
        name_label.set_halign(Gtk.Align.START)
        name_label.add_css_class("option-title")
        content_box.append(name_label)
        
        button.set_child(content_box)
        
        # Connect click event
        button.connect("clicked", lambda x: self.select_variant(variant["code"], button))
        self.variant_buttons.append(button)
        
        return button
    
    def select_variant(self, variant_code, button):
        # Remove selection from all variant buttons
        for btn in self.variant_buttons:
            btn.remove_css_class("selected")
        
        # Add selection to clicked button
        button.add_css_class("selected")
        self.selected_variant = variant_code
        
        # Apply keyboard layout (would be implemented with setxkbmap or similar)
        self.apply_keyboard_layout()
        
        print(f"Selected keyboard variant: {variant_code}")
    
    def apply_keyboard_layout(self):
        """Apply the selected keyboard layout"""
        if self.selected_layout:
            try:
                # This would set the keyboard layout in a real implementation
                cmd = ["setxkbmap", self.selected_layout]
                if self.selected_variant:
                    cmd.extend(["-variant", self.selected_variant])
                
                # In a real implementation, you would run this command
                # subprocess.run(cmd, check=True)
                print(f"Would apply keyboard layout: {' '.join(cmd)}")
            except Exception as e:
                print(f"Error applying keyboard layout: {e}")
