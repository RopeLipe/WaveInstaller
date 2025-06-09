/* user-screen.c - User account creation screen implementation */
#include "user-screen.h"
#include "../wave-installer.h"
#include <string.h>

static char *full_name = NULL;
static char *username = NULL;
static char *password = NULL;
static char *confirm_password = NULL;
static char *hostname = NULL;
static gboolean auto_login = FALSE;

static void
on_entry_changed(GtkEditable *editable, gpointer user_data) {
    const char *field = (const char *)user_data;
    const char *text = gtk_editable_get_text(editable);
    
    if (strcmp(field, "full_name") == 0) {
        if (full_name) g_free(full_name);
        full_name = g_strdup(text);
    } else if (strcmp(field, "username") == 0) {
        if (username) g_free(username);
        username = g_strdup(text);
    } else if (strcmp(field, "password") == 0) {
        if (password) g_free(password);
        password = g_strdup(text);
    } else if (strcmp(field, "confirm_password") == 0) {
        if (confirm_password) g_free(confirm_password);
        confirm_password = g_strdup(text);
    } else if (strcmp(field, "hostname") == 0) {
        if (hostname) g_free(hostname);
        hostname = g_strdup(text);
    }
}

static void
on_auto_login_toggled(GtkCheckButton *button, gpointer user_data) {
    auto_login = gtk_check_button_get_active(button);
}

static gboolean
validate_form(void) {
    return (full_name && strlen(full_name) > 0) &&
           (username && strlen(username) > 0) &&
           (password && strlen(password) > 0) &&
           (confirm_password && strlen(confirm_password) > 0) &&
           (strcmp(password, confirm_password) == 0) &&
           (hostname && strlen(hostname) > 0);
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    if (validate_form()) {
        wave_installer_application_next_screen(app);
    }
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

void
user_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *form_grid;
    GtkWidget *full_name_label, *full_name_entry, *username_label, *username_entry;
    GtkWidget *password_label, *password_entry, *confirm_label, *confirm_entry;
    GtkWidget *hostname_label, *hostname_entry, *auto_login_check;
    GtkWidget *nav_box, *back_btn, *next_btn;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Create User Account");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Set up your user account and computer name");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Form grid */
    form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 16);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 16);
    gtk_widget_add_css_class(form_grid, "form-grid");
    gtk_widget_set_hexpand(form_grid, TRUE);
    gtk_widget_set_halign(form_grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(content), form_grid);
    
    int row = 0;
    
    /* Full Name */
    full_name_label = gtk_label_new("Full Name:");
    gtk_widget_add_css_class(full_name_label, "form-label");
    gtk_widget_set_halign(full_name_label, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(form_grid), full_name_label, 0, row, 1, 1);
    
    full_name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(full_name_entry), "Enter your full name");
    gtk_widget_add_css_class(full_name_entry, "form-input");
    gtk_widget_set_hexpand(full_name_entry, TRUE);
    g_signal_connect(full_name_entry, "changed", G_CALLBACK(on_entry_changed), "full_name");
    gtk_grid_attach(GTK_GRID(form_grid), full_name_entry, 1, row++, 1, 1);
    
    /* Username */
    username_label = gtk_label_new("Username:");
    gtk_widget_add_css_class(username_label, "form-label");
    gtk_widget_set_halign(username_label, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(form_grid), username_label, 0, row, 1, 1);
    
    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter username (lowercase)");
    gtk_widget_add_css_class(username_entry, "form-input");
    gtk_widget_set_hexpand(username_entry, TRUE);
    g_signal_connect(username_entry, "changed", G_CALLBACK(on_entry_changed), "username");
    gtk_grid_attach(GTK_GRID(form_grid), username_entry, 1, row++, 1, 1);
    
    /* Password */
    password_label = gtk_label_new("Password:");
    gtk_widget_add_css_class(password_label, "form-label");
    gtk_widget_set_halign(password_label, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(form_grid), password_label, 0, row, 1, 1);
    
    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter a secure password");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_widget_add_css_class(password_entry, "form-input");
    gtk_widget_set_hexpand(password_entry, TRUE);
    g_signal_connect(password_entry, "changed", G_CALLBACK(on_entry_changed), "password");
    gtk_grid_attach(GTK_GRID(form_grid), password_entry, 1, row++, 1, 1);
    
    /* Confirm Password */
    confirm_label = gtk_label_new("Confirm Password:");
    gtk_widget_add_css_class(confirm_label, "form-label");
    gtk_widget_set_halign(confirm_label, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(form_grid), confirm_label, 0, row, 1, 1);
    
    confirm_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_entry), "Confirm your password");
    gtk_entry_set_visibility(GTK_ENTRY(confirm_entry), FALSE);
    gtk_widget_add_css_class(confirm_entry, "form-input");
    gtk_widget_set_hexpand(confirm_entry, TRUE);
    g_signal_connect(confirm_entry, "changed", G_CALLBACK(on_entry_changed), "confirm_password");
    gtk_grid_attach(GTK_GRID(form_grid), confirm_entry, 1, row++, 1, 1);
    
    /* Hostname */
    hostname_label = gtk_label_new("Computer Name:");
    gtk_widget_add_css_class(hostname_label, "form-label");
    gtk_widget_set_halign(hostname_label, GTK_ALIGN_END);
    gtk_grid_attach(GTK_GRID(form_grid), hostname_label, 0, row, 1, 1);
      hostname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(hostname_entry), "Enter computer name");
    gtk_editable_set_text(GTK_EDITABLE(hostname_entry), "wave-linux");
    gtk_widget_add_css_class(hostname_entry, "form-input");
    gtk_widget_set_hexpand(hostname_entry, TRUE);
    g_signal_connect(hostname_entry, "changed", G_CALLBACK(on_entry_changed), "hostname");
    gtk_grid_attach(GTK_GRID(form_grid), hostname_entry, 1, row++, 1, 1);
    
    /* Auto-login checkbox */
    auto_login_check = gtk_check_button_new_with_label("Log in automatically");
    gtk_widget_add_css_class(auto_login_check, "form-checkbox");
    gtk_widget_set_margin_top(auto_login_check, 20);
    g_signal_connect(auto_login_check, "toggled", G_CALLBACK(on_auto_login_toggled), app);
    gtk_grid_attach(GTK_GRID(form_grid), auto_login_check, 1, row++, 1, 1);
    
    /* Navigation buttons */
    nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(nav_box, "nav-buttons");
    gtk_widget_set_halign(nav_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(container), nav_box);
    
    /* Back button */
    back_btn = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_btn, "secondary-button");
    g_signal_connect(back_btn, "clicked", G_CALLBACK(on_back_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), back_btn);
    
    /* Next button */
    next_btn = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_btn, "primary-button");
    g_signal_connect(next_btn, "clicked", G_CALLBACK(on_next_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), next_btn);
    
    /* Store widget */
    app->screens[SCREEN_USER].widget = container;
    
    /* Initialize hostname */
    hostname = g_strdup("wave-linux");
}
