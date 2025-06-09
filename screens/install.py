"""
Installation Progress Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk, GLib
import time
import threading

class InstallScreen:
    def __init__(self, app):
        self.app = app
        self.progress_bar = None
        self.progress_label = None
        self.current_step_label = None
        self.log_buffer = None
        self.log_view = None
        self.installation_complete = False
        self.current_progress = 0
        
        # Installation steps
        self.install_steps = [
            {"name": "Preparing installation", "duration": 2},
            {"name": "Partitioning disk", "duration": 3},
            {"name": "Formatting partitions", "duration": 2},
            {"name": "Installing base system", "duration": 15},
            {"name": "Installing kernel", "duration": 5},
            {"name": "Installing bootloader", "duration": 3},
            {"name": "Configuring system", "duration": 4},
            {"name": "Installing packages", "duration": 8},
            {"name": "Setting up user account", "duration": 2},
            {"name": "Finalizing installation", "duration": 3}
        ]
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        content.set_spacing(30)
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Installing Wave OS</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Please wait while we install your new operating system")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Progress section
        progress_section = self.create_progress_section()
        content.append(progress_section)
        
        # Installation info cards
        info_section = self.create_info_section()
        content.append(info_section)
        
        # Log section (collapsible)
        log_section = self.create_log_section()
        content.append(log_section)
        
        # Navigation buttons (initially hidden)
        self.nav_box = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        self.nav_box.add_css_class("nav-buttons")
        self.nav_box.set_halign(Gtk.Align.FILL)
        self.nav_box.set_homogeneous(False)
        self.nav_box.set_visible(False)
        
        # Reboot button
        reboot_btn = Gtk.Button(label="Reboot Now")
        reboot_btn.add_css_class("primary-button")
        reboot_btn.connect("clicked", lambda x: self.reboot_system())
        
        # Spacer
        spacer = Gtk.Box()
        spacer.set_hexpand(True)
        self.nav_box.append(spacer)
        self.nav_box.append(reboot_btn)
        
        content.append(self.nav_box)
        container.append(content)
        
        # Start installation process
        self.start_installation()
        
        return container
    
    def create_progress_section(self):
        """Create the progress indicator section"""
        section = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        section.set_spacing(20)
        section.add_css_class("card")
        section.set_margin_start(50)
        section.set_margin_end(50)
        
        # Current step
        self.current_step_label = Gtk.Label()
        self.current_step_label.set_markup("<span weight='bold'>Preparing installation...</span>")
        self.current_step_label.add_css_class("option-title")
        section.append(self.current_step_label)
        
        # Progress bar
        self.progress_bar = Gtk.ProgressBar()
        self.progress_bar.add_css_class("progress-bar")
        self.progress_bar.set_size_request(-1, 12)
        section.append(self.progress_bar)
        
        # Progress text
        self.progress_label = Gtk.Label()
        self.progress_label.set_text("0%")
        self.progress_label.add_css_class("option-description")
        section.append(self.progress_label)
        
        return section
    
    def create_info_section(self):
        """Create installation information cards"""
        section = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        section.set_spacing(20)
        section.set_homogeneous(True)
        
        # Time estimate card
        time_card = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        time_card.add_css_class("feature-card")
        time_card.set_spacing(12)
        
        time_icon = Gtk.Image()
        time_icon.set_from_icon_name("preferences-system-time")
        time_icon.set_pixel_size(32)
        time_icon.add_css_class("feature-icon")
        time_card.append(time_icon)
        
        time_title = Gtk.Label()
        time_title.set_markup("<span weight='bold'>Time Remaining</span>")
        time_title.add_css_class("feature-title")
        time_card.append(time_title)
        
        self.time_estimate = Gtk.Label()
        self.time_estimate.set_text("Calculating...")
        self.time_estimate.add_css_class("feature-description")
        time_card.append(self.time_estimate)
        
        section.append(time_card)
        
        # Disk space card
        space_card = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        space_card.add_css_class("feature-card")
        space_card.set_spacing(12)
        
        space_icon = Gtk.Image()
        space_icon.set_from_icon_name("drive-harddisk")
        space_icon.set_pixel_size(32)
        space_icon.add_css_class("feature-icon")
        space_card.append(space_icon)
        
        space_title = Gtk.Label()
        space_title.set_markup("<span weight='bold'>Disk Usage</span>")
        space_title.add_css_class("feature-title")
        space_card.append(space_title)
        
        space_desc = Gtk.Label()
        space_desc.set_text("Installing 4.2GB of 500GB")
        space_desc.add_css_class("feature-description")
        space_card.append(space_desc)
        
        section.append(space_card)
        
        # Features card
        features_card = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        features_card.add_css_class("feature-card")
        features_card.set_spacing(12)
        
        features_icon = Gtk.Image()
        features_icon.set_from_icon_name("applications-system")
        features_icon.set_pixel_size(32)
        features_icon.add_css_class("feature-icon")
        features_card.append(features_icon)
        
        features_title = Gtk.Label()
        features_title.set_markup("<span weight='bold'>What's New</span>")
        features_title.add_css_class("feature-title")
        features_card.append(features_title)
        
        features_desc = Gtk.Label()
        features_desc.set_text("Modern interface\nEnhanced security\nBetter performance")
        features_desc.add_css_class("feature-description")
        features_desc.set_justify(Gtk.Justification.CENTER)
        features_card.append(features_desc)
        
        section.append(features_card)
        
        return section
    
    def create_log_section(self):
        """Create installation log section"""
        section = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        section.set_spacing(12)
        section.set_margin_top(20)
        
        # Log header with toggle
        log_header = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL)
        log_header.set_spacing(8)
        
        log_toggle = Gtk.Button()
        log_toggle.set_icon_name("view-more")
        log_toggle.add_css_class("secondary-button")
        log_toggle.connect("clicked", self.toggle_log_view)
        log_header.append(log_toggle)
        
        log_title = Gtk.Label()
        log_title.set_markup("<span weight='bold'>Installation Log</span>")
        log_title.add_css_class("form-label")
        log_header.append(log_title)
        
        section.append(log_header)
        
        # Log view (initially hidden)
        self.log_scrolled = Gtk.ScrolledWindow()
        self.log_scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        self.log_scrolled.set_min_content_height(150)
        self.log_scrolled.set_visible(False)
        
        self.log_view = Gtk.TextView()
        self.log_view.set_editable(False)
        self.log_view.set_cursor_visible(False)
        self.log_view.add_css_class("form-input")
        self.log_view.set_monospace(True)
        
        self.log_buffer = self.log_view.get_buffer()
        self.log_scrolled.set_child(self.log_view)
        
        section.append(self.log_scrolled)
        
        return section
    
    def toggle_log_view(self, button):
        """Toggle log view visibility"""
        if self.log_scrolled.get_visible():
            self.log_scrolled.set_visible(False)
            button.set_icon_name("view-more")
        else:
            self.log_scrolled.set_visible(True)
            button.set_icon_name("view-less")
            # Scroll to bottom
            mark = self.log_buffer.get_insert()
            self.log_view.scroll_mark_onscreen(mark)
    
    def start_installation(self):
        """Start the installation process"""
        # Calculate total duration
        self.total_duration = sum(step["duration"] for step in self.install_steps)
        self.current_time = 0
        
        # Add initial log entry
        self.add_log_entry("Installation started")
        self.add_log_entry("System information collected")
        
        # Start installation in background thread
        thread = threading.Thread(target=self.run_installation)
        thread.daemon = True
        thread.start()
    
    def run_installation(self):
        """Run installation steps in background"""
        for i, step in enumerate(self.install_steps):
            # Update UI on main thread
            GLib.idle_add(self.update_step, step["name"], i)
            GLib.idle_add(self.add_log_entry, f"Starting: {step['name']}")
            
            # Simulate installation time
            for j in range(step["duration"]):
                time.sleep(1)  # 1 second per unit
                self.current_time += 1
                progress = (self.current_time / self.total_duration) * 100
                remaining_time = self.total_duration - self.current_time
                
                # Update progress on main thread
                GLib.idle_add(self.update_progress, progress, remaining_time)
            
            GLib.idle_add(self.add_log_entry, f"Completed: {step['name']}")
        
        # Installation complete
        GLib.idle_add(self.installation_completed)
    
    def update_step(self, step_name, step_index):
        """Update current installation step"""
        self.current_step_label.set_markup(f"<span weight='bold'>{step_name}</span>")
        return False
    
    def update_progress(self, progress, remaining_time):
        """Update progress bar and labels"""
        self.progress_bar.set_fraction(progress / 100.0)
        self.progress_label.set_text(f"{int(progress)}%")
        
        # Update time estimate
        if remaining_time > 60:
            minutes = remaining_time // 60
            seconds = remaining_time % 60
            time_text = f"{minutes}m {seconds}s"
        else:
            time_text = f"{remaining_time}s"
        
        self.time_estimate.set_text(time_text)
        return False
    
    def add_log_entry(self, message):
        """Add entry to installation log"""
        timestamp = time.strftime("%H:%M:%S")
        log_line = f"[{timestamp}] {message}\n"
        
        # Insert at end of buffer
        end_iter = self.log_buffer.get_end_iter()
        self.log_buffer.insert(end_iter, log_line)
        
        # Auto-scroll if log is visible
        if self.log_scrolled.get_visible():
            mark = self.log_buffer.get_insert()
            self.log_view.scroll_mark_onscreen(mark)
        
        return False
    
    def installation_completed(self):
        """Handle installation completion"""
        self.installation_complete = True
        
        # Update final step
        self.current_step_label.set_markup("<span weight='bold' color='green'>Installation Complete!</span>")
        self.progress_bar.set_fraction(1.0)
        self.progress_label.set_text("100%")
        self.time_estimate.set_text("Complete")
        
        # Add final log entries
        self.add_log_entry("Installation completed successfully")
        self.add_log_entry("System is ready for first boot")
        
        # Show navigation buttons
        self.nav_box.set_visible(True)
        
        # Show completion message
        self.show_completion_dialog()
        
        return False
    
    def show_completion_dialog(self):
        """Show installation completion dialog"""
        dialog = Gtk.MessageDialog(
            transient_for=self.app.main_window,
            flags=0,
            message_type=Gtk.MessageType.INFO,
            buttons=Gtk.ButtonsType.OK,
            text="Installation Complete!"
        )
        dialog.format_secondary_text(
            "Wave OS has been successfully installed on your computer. "
            "Please reboot to start using your new system."
        )
        dialog.run()
        dialog.destroy()
    
    def reboot_system(self):
        """Handle system reboot"""
        dialog = Gtk.MessageDialog(
            transient_for=self.app.main_window,
            flags=0,
            message_type=Gtk.MessageType.QUESTION,
            buttons=Gtk.ButtonsType.YES_NO,
            text="Reboot Now?"
        )
        dialog.format_secondary_text(
            "The installation is complete. Would you like to reboot now to start using Wave OS?"
        )
        
        response = dialog.run()
        dialog.destroy()
        
        if response == Gtk.ResponseType.YES:
            # In a real implementation, this would reboot the system
            print("System would reboot now")
            self.app.quit_installer()
        else:
            print("User chose not to reboot")
