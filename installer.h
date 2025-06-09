#ifndef INSTALLER_H
#define INSTALLER_H

#include <gtk/gtk.h>
#include <glib.h>

// Main installer window
void create_installer_window(GtkApplication *app);

// Page creation functions
GtkWidget* create_welcome_page(void);
GtkWidget* create_language_page(void);
GtkWidget* create_timezone_page(void);
GtkWidget* create_keyboard_page(void);
GtkWidget* create_disk_page(void);
GtkWidget* create_network_page(void);
GtkWidget* create_user_page(void);

// Navigation functions
void navigate_to_page(const char* page_name);
void setup_navigation_buttons(GtkWidget* page, const char* current_page);

// Utility functions
GtkWidget* create_rounded_frame(GtkWidget* child);
GtkWidget* create_disk_card(const char* name, const char* size, const char* type, const char* icon_name);
GtkWidget* create_network_card(const char* name, const char* signal_strength, gboolean is_secure);
void show_wifi_password_dialog(GtkWidget* parent, const char* network_name);
void apply_custom_css(void);

// Global variables
extern GtkWidget* main_window;
extern GtkWidget* main_stack;
extern GtkWidget* navigation_box;

#endif // INSTALLER_H
