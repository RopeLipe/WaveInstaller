"""
Language Selection Screen
"""

import gi
gi.require_version('Gtk', '4.0')
from gi.repository import Gtk
import locale

class LanguageScreen:
    def __init__(self, app):
        self.app = app
        self.selected_language = None
        self.language_buttons = []
    
    def create_widget(self):
        # Main container
        container = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        container.add_css_class("screen-container")
        
        # Content area
        content = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        content.add_css_class("screen-content")
        
        # Title
        title = Gtk.Label()
        title.set_markup("<span size='large' weight='bold'>Choose Your Language</span>")
        title.add_css_class("screen-title")
        content.append(title)
        
        # Subtitle
        subtitle = Gtk.Label()
        subtitle.set_text("Select your preferred language for the installation")
        subtitle.add_css_class("screen-subtitle")
        content.append(subtitle)
        
        # Language list
        scrolled = Gtk.ScrolledWindow()
        scrolled.set_policy(Gtk.PolicyType.NEVER, Gtk.PolicyType.AUTOMATIC)
        scrolled.set_min_content_height(400)
        
        lang_box = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        lang_box.add_css_class("option-list")
        lang_box.set_spacing(8)
        
        # Language options
        languages = [
            {"code": "en_US", "name": "English", "native": "English", "flag": "🇺🇸"},
            {"code": "es_ES", "name": "Spanish", "native": "Español", "flag": "🇪🇸"},
            {"code": "fr_FR", "name": "French", "native": "Français", "flag": "🇫🇷"},
            {"code": "de_DE", "name": "German", "native": "Deutsch", "flag": "🇩🇪"},
            {"code": "it_IT", "name": "Italian", "native": "Italiano", "flag": "🇮🇹"},
            {"code": "pt_PT", "name": "Portuguese", "native": "Português", "flag": "🇵🇹"},
            {"code": "ru_RU", "name": "Russian", "native": "Русский", "flag": "🇷🇺"},
            {"code": "zh_CN", "name": "Chinese (Simplified)", "native": "简体中文", "flag": "🇨🇳"},
            {"code": "ja_JP", "name": "Japanese", "native": "日本語", "flag": "🇯🇵"},
            {"code": "ko_KR", "name": "Korean", "native": "한국어", "flag": "🇰🇷"},
            {"code": "ar_SA", "name": "Arabic", "native": "العربية", "flag": "🇸🇦"},
            {"code": "hi_IN", "name": "Hindi", "native": "हिन्दी", "flag": "🇮🇳"},
        ]
        
        # Detect current system language
        try:
            current_locale = locale.getdefaultlocale()[0]
            if current_locale:
                current_lang = current_locale.split('_')[0]
            else:
                current_lang = 'en'
        except:
            current_lang = 'en'
        
        for lang in languages:
            lang_item = self.create_language_item(lang)
            lang_box.append(lang_item)
            
            # Select default language
            if lang["code"].startswith(current_lang) and not self.selected_language:
                self.select_language(lang["code"], lang_item)
        
        # If no language was auto-selected, select English
        if not self.selected_language:
            first_item = lang_box.get_first_child()
            if first_item:
                self.select_language("en_US", first_item)
        
        scrolled.set_child(lang_box)
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
        
        # Next button
        next_btn = Gtk.Button(label="Continue")
        next_btn.add_css_class("primary-button")
        next_btn.connect("clicked", lambda x: self.app.next_screen())
        nav_box.append(next_btn)
        
        content.append(nav_box)
        container.append(content)
        
        return container
    
    def create_language_item(self, lang):
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
        
        # Flag
        flag_label = Gtk.Label()
        flag_label.set_text(lang["flag"])
        flag_label.set_size_request(32, -1)
        content_box.append(flag_label)
        
        # Language info
        lang_info = Gtk.Box(orientation=Gtk.Orientation.VERTICAL)
        lang_info.set_valign(Gtk.Align.CENTER)
        
        # Native name
        native_label = Gtk.Label()
        native_label.set_markup(f"<span weight='bold'>{lang['native']}</span>")
        native_label.set_halign(Gtk.Align.START)
        native_label.add_css_class("option-title")
        lang_info.append(native_label)
        
        # English name
        english_label = Gtk.Label()
        english_label.set_text(lang["name"])
        english_label.set_halign(Gtk.Align.START)
        english_label.add_css_class("option-description")
        lang_info.append(english_label)
        
        content_box.append(lang_info)
        button.set_child(content_box)
        
        # Connect click event
        button.connect("clicked", lambda x: self.select_language(lang["code"], button))
        self.language_buttons.append(button)
        
        return button
    
    def select_language(self, lang_code, button):
        # Remove selection from all buttons
        for btn in self.language_buttons:
            btn.remove_css_class("selected")
        
        # Add selection to clicked button
        button.add_css_class("selected")
        self.selected_language = lang_code
        
        # Here you would typically set the application language
        print(f"Selected language: {lang_code}")
