"""
User Configuration Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk
import re

class UserScreen:
    def __init__(self, app):
        self.app = app
        self.user_data = {
            "full_name": "",
            "username": "",
            "password": "",
            "confirm_password": "",
            "auto_login": False,
            "admin_user": True
        }
        self.entries = {}
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)  
        content.add_css_class("screen-content")
        content.set_spacing(20)
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Create User Account</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Set up your user account for the new system")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Form container
        form_container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        form_container.set_spacing(20)
        form_container.set_halign(Gtk.Align.CENTER)
        form_container.set_size_request(500, -1)
        
        # User icon
        user_icon = Gtk.Image()
        user_icon.set_from_icon_name("avatar-default")
        user_icon.set_pixel_size(80)
        user_icon.set_halign(Gtk.Align.CENTER)
        user_icon.set_margin_bottom(20)
        form_container.append(user_icon)
        
        # Full name field
        full_name_group = self.create_form_group(
            "Full Name",
            "Enter your full name",
            "full_name"
        )
        form_container.append(full_name_group)
        
        # Username field
        username_group = self.create_form_group(
            "Username",
            "Choose a username for login",
            "username"
        )
        form_container.append(username_group)
        
        # Username help text
        username_help = Gtk.Label()
        username_help.set_text("Username should be lowercase letters, numbers, and dashes only")
        username_help.add_css_class("option-description")
        username_help.set_halign(Gtk.Align.START)
        username_help.set_margin_bottom(10)
        form_container.append(username_help)
        
        # Password field
        password_group = self.create_form_group(
            "Password",
            "Create a secure password",
            "password",
            password=True
        )
        form_container.append(password_group)
        
        # Confirm password field
        confirm_password_group = self.create_form_group(
            "Confirm Password", 
            "Re-enter your password",
            "confirm_password",
            password=True
        )
        form_container.append(confirm_password_group)
        
        # Password strength indicator
        self.password_strength = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        self.password_strength.set_spacing(8)
        self.password_strength.set_margin_bottom(10)
        
        strength_label = Gtk.Label()
        strength_label.set_text("Password Strength:")
        strength_label.add_css_class("form-label")
        strength_label.set_halign(Gtk.Align.START)
        self.password_strength.append(strength_label)
        
        # Strength bars
        self.strength_bars = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        self.strength_bars.set_spacing(4)
        for i in range(4):
            bar = Gtk.Box()
            bar.set_size_request(25, 6)
            bar.add_css_class("strength-bar")
            bar.add_css_class("strength-empty")
            self.strength_bars.append(bar)
        
        self.password_strength.append(self.strength_bars)
        
        self.strength_text = Gtk.Label()
        self.strength_text.add_css_class("option-description")
        self.strength_text.set_halign(Gtk.Align.START)
        self.password_strength.append(self.strength_text)
        
        form_container.append(self.password_strength)
        
        # Options section
        options_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        options_box.set_spacing(12)
        options_box.set_margin_top(20)
        
        # Admin user checkbox
        admin_check = Gtk.CheckButton(label="Make this user an administrator")
        admin_check.set_active(True)
        admin_check.connect("toggled", lambda x: self.set_user_option("admin_user", x.get_active()))
        options_box.append(admin_check)
        
        admin_help = Gtk.Label()
        admin_help.set_text("Administrators can install software and modify system settings")
        admin_help.add_css_class("option-description")
        admin_help.set_halign(Gtk.Align.START)
        admin_help.set_margin_start(30)
        options_box.append(admin_help)
        
        # Auto-login checkbox
        auto_login_check = Gtk.CheckButton(label="Log in automatically")
        auto_login_check.connect("toggled", lambda x: self.set_user_option("auto_login", x.get_active()))
        options_box.append(auto_login_check)
        
        auto_help = Gtk.Label()
        auto_help.set_text("Skip login screen and automatically sign in to this account")
        auto_help.add_css_class("option-description")
        auto_help.set_halign(Gtk.Align.START)
        auto_help.set_margin_start(30)
        options_box.append(auto_help)
        
        form_container.append(options_box)
        content.append(form_container)
        
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
        self.next_btn = Gtk.Button(label="Continue")
        self.next_btn.add_css_class("primary-button")
        self.next_btn.connect("clicked", lambda x: self.validate_and_continue())
        self.next_btn.set_sensitive(False)  # Disabled until form is valid
        nav_box.append(self.next_btn)
        
        content.append(nav_box)
        container.append(content)
        
        return container
    
    def create_form_group(self, label_text, placeholder, field_name, password=False):
        """Create a form input group"""
        group = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        group.add_css_class("form-group")
        group.set_spacing(8)
        
        # Label
        label = Gtk.Label()
        label.set_markup(f"<span weight='bold'>{label_text}</span>")
        label.add_css_class("form-label")
        label.set_halign(Gtk.Align.START)
        group.append(label)
        
        # Entry container (for password toggle button)
        entry_container = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        entry_container.set_spacing(8)
        
        # Entry field
        entry = Gtk.Entry()
        entry.set_placeholder_text(placeholder)
        entry.add_css_class("form-input")
        entry.set_hexpand(True)
        
        if password:
            entry.set_visibility(False)
            entry.set_input_purpose(Gtk.InputPurpose.PASSWORD)
            
            # Password toggle button
            toggle_btn = Gtk.Button()
            toggle_btn.set_icon_name("view-reveal")
            toggle_btn.add_css_class("secondary-button")
            toggle_btn.connect("clicked", lambda x: self.toggle_password_visibility(entry, x))
            entry_container.append(entry)
            entry_container.append(toggle_btn)
        else:
            entry_container.append(entry)
        
        # Connect change events
        entry.connect("changed", lambda x: self.on_field_changed(field_name, x.get_text()))
        
        # Special handling for username field
        if field_name == "username":
            entry.connect("changed", self.on_username_changed)
        elif field_name == "password":
            entry.connect("changed", self.on_password_changed)
        
        self.entries[field_name] = entry
        group.append(entry_container)
        
        return group
    
    def toggle_password_visibility(self, entry, button):
        """Toggle password field visibility"""
        if entry.get_visibility():
            entry.set_visibility(False)
            button.set_icon_name("view-reveal")
        else:
            entry.set_visibility(True)
            button.set_icon_name("view-conceal")
    
    def on_field_changed(self, field_name, value):
        """Handle field value changes"""
        self.user_data[field_name] = value
        self.validate_form()
    
    def on_username_changed(self, entry):
        """Handle username field changes with validation"""
        username = entry.get_text()
        
        # Auto-generate username from full name if username is empty
        if not username and self.user_data.get("full_name"):
            # Generate username from full name
            suggested = self.generate_username(self.user_data["full_name"])
            entry.set_text(suggested)
            return
        
        # Validate username format
        if username and not self.validate_username(username):
            entry.add_css_class("error")
        else:
            entry.remove_css_class("error")
        
        self.user_data["username"] = username
        self.validate_form()
    
    def on_password_changed(self, entry):
        """Handle password changes with strength checking"""
        password = entry.get_text()
        self.user_data["password"] = password
        
        # Update password strength
        self.update_password_strength(password)
        
        # Check password match
        if self.user_data.get("confirm_password"):
            confirm_entry = self.entries["confirm_password"]
            if password != self.user_data["confirm_password"]:
                confirm_entry.add_css_class("error")
            else:
                confirm_entry.remove_css_class("error")
        
        self.validate_form()
    
    def generate_username(self, full_name):
        """Generate username from full name"""
        # Convert to lowercase, remove special chars, replace spaces with dots
        username = re.sub(r'[^a-zA-Z0-9\s]', '', full_name.lower())
        username = re.sub(r'\s+', '.', username.strip())
        return username[:20]  # Limit length
    
    def validate_username(self, username):
        """Validate username format"""
        # Username should be lowercase letters, numbers, dots, dashes only
        pattern = r'^[a-z0-9.-]+$'
        return bool(re.match(pattern, username)) and len(username) >= 3
    
    def update_password_strength(self, password):
        """Update password strength indicator"""
        strength = self.calculate_password_strength(password)
        
        # Clear existing strength classes
        for i, bar in enumerate(self.strength_bars):
            bar.remove_css_class("strength-weak")
            bar.remove_css_class("strength-fair")
            bar.remove_css_class("strength-good")
            bar.remove_css_class("strength-strong")
            bar.add_css_class("strength-empty")
        
        # Set strength bars
        if strength > 0:
            colors = ["strength-weak", "strength-fair", "strength-good", "strength-strong"]
            labels = ["Weak", "Fair", "Good", "Strong"]
            
            for i in range(min(strength, 4)):
                bar = self.strength_bars.get_first_child()
                for _ in range(i):
                    bar = bar.get_next_sibling()
                
                bar.remove_css_class("strength-empty")
                bar.add_css_class(colors[min(strength-1, 3)])
            
            self.strength_text.set_text(labels[min(strength-1, 3)])
        else:
            self.strength_text.set_text("")
    
    def calculate_password_strength(self, password):
        """Calculate password strength (1-4)"""
        if not password:
            return 0
        
        score = 0
        
        # Length check
        if len(password) >= 8:
            score += 1
        if len(password) >= 12:
            score += 1
        
        # Character variety
        if re.search(r'[a-z]', password) and re.search(r'[A-Z]', password):
            score += 1
        if re.search(r'\d', password):
            score += 1
        if re.search(r'[!@#$%^&*(),.?":{}|<>]', password):
            score += 1
        
        return min(score, 4)
    
    def set_user_option(self, option, value):
        """Set user option"""
        self.user_data[option] = value
        print(f"Set {option} to {value}")
    
    def validate_form(self):
        """Validate entire form and enable/disable continue button"""
        valid = True
        
        # Check required fields
        required_fields = ["full_name", "username", "password", "confirm_password"]
        for field in required_fields:
            if not self.user_data.get(field):
                valid = False
                break
        
        # Check username format
        if self.user_data.get("username") and not self.validate_username(self.user_data["username"]):
            valid = False
        
        # Check password match
        if (self.user_data.get("password") and 
            self.user_data.get("confirm_password") and
            self.user_data["password"] != self.user_data["confirm_password"]):
            valid = False
        
        # Check minimum password strength
        if self.user_data.get("password"):
            strength = self.calculate_password_strength(self.user_data["password"])
            if strength < 2:  # Require at least "Fair" strength
                valid = False
        
        self.next_btn.set_sensitive(valid)
    
    def validate_and_continue(self):
        """Final validation before continuing"""
        if self.user_data["password"] != self.user_data["confirm_password"]:
            # Show error dialog
            dialog = Gtk.MessageDialog(
                transient_for=self.app.main_window,
                flags=0,
                message_type=Gtk.MessageType.ERROR,
                buttons=Gtk.ButtonsType.OK,
                text="Passwords do not match"
            )
            dialog.format_secondary_text("Please ensure both password fields contain the same value.")
            dialog.run()
            dialog.destroy()
            return
        
        print(f"User configuration: {self.user_data}")
        self.app.next_screen()
