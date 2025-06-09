#include "../installer.h"

static GtkWidget* fullname_entry = NULL;
static GtkWidget* username_entry = NULL;
static GtkWidget* hostname_entry = NULL;
static GtkWidget* password_entry = NULL;
static GtkWidget* confirm_password_entry = NULL;
static GtkWidget* password_strength_bar = NULL;
static GtkWidget* password_strength_label = NULL;

static void update_password_strength(GtkEntry* entry, gpointer user_data) {
    const char* password = gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
    int length = strlen(password);
    
    // Simple password strength calculation
    int strength = 0;
    gboolean has_lower = FALSE, has_upper = FALSE, has_digit = FALSE, has_special = FALSE;
    
    for (int i = 0; i < length; i++) {
        char c = password[i];
        if (c >= 'a' && c <= 'z') has_lower = TRUE;
        else if (c >= 'A' && c <= 'Z') has_upper = TRUE;
        else if (c >= '0' && c <= '9') has_digit = TRUE;
        else has_special = TRUE;
    }
    
    if (length >= 8) strength++;
    if (has_lower) strength++;
    if (has_upper) strength++;
    if (has_digit) strength++;
    if (has_special) strength++;
    
    const char* strength_text[] = {"Very Weak", "Weak", "Fair", "Good", "Strong"};
    const char* strength_class[] = {"very-weak", "weak", "fair", "good", "strong"};
    
    // Remove all strength classes
    for (int i = 0; i < 5; i++) {
        gtk_widget_remove_css_class(password_strength_bar, strength_class[i]);
    }
    
    int strength_index = (strength > 4) ? 4 : strength;
    gtk_widget_add_css_class(password_strength_bar, strength_class[strength_index]);
    gtk_label_set_text(GTK_LABEL(password_strength_label), strength_text[strength_index]);
    
    // Update progress bar value (0.0 to 1.0)
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(password_strength_bar), strength / 5.0);
}

static void on_fullname_changed(GtkEntry* entry, gpointer user_data) {
    const char* fullname = gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
    
    // Auto-generate username from full name
    if (strlen(fullname) > 0) {
        char* username = g_ascii_strdown(fullname, -1);
        // Replace spaces with nothing and remove special characters
        char* clean_username = g_strdup("");
        for (int i = 0; username[i]; i++) {
            if ((username[i] >= 'a' && username[i] <= 'z') || 
                (username[i] >= '0' && username[i] <= '9')) {
                char* temp = g_strdup_printf("%s%c", clean_username, username[i]);
                g_free(clean_username);
                clean_username = temp;
            }
        }
        
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(username_entry), clean_username, -1);
        g_free(username);
        g_free(clean_username);
    }
}

static void on_username_changed(GtkEntry* entry, gpointer user_data) {
    const char* username = gtk_entry_buffer_get_text(gtk_entry_get_buffer(entry));
    
    // Auto-generate hostname
    if (strlen(username) > 0) {
        char* hostname = g_strdup_printf("%s-desktop", username);
        gtk_entry_buffer_set_text(gtk_entry_get_buffer(hostname_entry), hostname, -1);
        g_free(hostname);
    }
}

GtkWidget* create_user_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Create User Account");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Set up your user account to access the system after installation.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);
    
    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(content_box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(content_box, 500, -1);
    
    // User form in a grid
    GtkWidget* form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 16);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 12);
    gtk_widget_add_css_class(form_grid, "user-form");
    
    int row = 0;
    
    // Full Name
    GtkWidget* fullname_label = gtk_label_new("Full Name:");
    gtk_widget_add_css_class(fullname_label, "field-label");
    gtk_widget_set_halign(fullname_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), fullname_label, 0, row, 1, 1);
    
    fullname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fullname_entry), "Enter your full name");
    gtk_widget_add_css_class(fullname_entry, "user-entry");
    gtk_widget_set_hexpand(fullname_entry, TRUE);
    g_signal_connect(fullname_entry, "changed", G_CALLBACK(on_fullname_changed), NULL);
    gtk_grid_attach(GTK_GRID(form_grid), fullname_entry, 1, row, 1, 1);
    row++;
    
    // Username
    GtkWidget* username_label = gtk_label_new("Username:");
    gtk_widget_add_css_class(username_label, "field-label");
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), username_label, 0, row, 1, 1);
    
    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "Enter username");
    gtk_widget_add_css_class(username_entry, "user-entry");
    gtk_widget_set_hexpand(username_entry, TRUE);
    g_signal_connect(username_entry, "changed", G_CALLBACK(on_username_changed), NULL);
    gtk_grid_attach(GTK_GRID(form_grid), username_entry, 1, row, 1, 1);
    row++;
    
    // Hostname
    GtkWidget* hostname_label = gtk_label_new("Computer Name:");
    gtk_widget_add_css_class(hostname_label, "field-label");
    gtk_widget_set_halign(hostname_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), hostname_label, 0, row, 1, 1);
    
    hostname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(hostname_entry), "Enter computer name");
    gtk_widget_add_css_class(hostname_entry, "user-entry");
    gtk_widget_set_hexpand(hostname_entry, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), hostname_entry, 1, row, 1, 1);
    row++;
    
    // Password
    GtkWidget* password_label = gtk_label_new("Password:");
    gtk_widget_add_css_class(password_label, "field-label");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), password_label, 0, row, 1, 1);
    
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter password");
    gtk_widget_add_css_class(password_entry, "password-entry");
    gtk_widget_set_hexpand(password_entry, TRUE);
    g_signal_connect(password_entry, "changed", G_CALLBACK(update_password_strength), NULL);
    gtk_grid_attach(GTK_GRID(form_grid), password_entry, 1, row, 1, 1);
    row++;
    
    // Password strength indicator
    GtkWidget* strength_label = gtk_label_new("Password Strength:");
    gtk_widget_add_css_class(strength_label, "field-label");
    gtk_widget_set_halign(strength_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), strength_label, 0, row, 1, 1);
    
    GtkWidget* strength_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    
    password_strength_bar = gtk_progress_bar_new();
    gtk_widget_add_css_class(password_strength_bar, "password-strength");
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(password_strength_bar), 0.0);
    gtk_box_append(GTK_BOX(strength_box), password_strength_bar);
    
    password_strength_label = gtk_label_new("Very Weak");
    gtk_widget_add_css_class(password_strength_label, "strength-text");
    gtk_widget_set_halign(password_strength_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(strength_box), password_strength_label);
    
    gtk_grid_attach(GTK_GRID(form_grid), strength_box, 1, row, 1, 1);
    row++;
    
    // Confirm Password
    GtkWidget* confirm_label = gtk_label_new("Confirm Password:");
    gtk_widget_add_css_class(confirm_label, "field-label");
    gtk_widget_set_halign(confirm_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), confirm_label, 0, row, 1, 1);
    
    confirm_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(confirm_password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Confirm password");
    gtk_widget_add_css_class(confirm_password_entry, "password-entry");
    gtk_widget_set_hexpand(confirm_password_entry, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), confirm_password_entry, 1, row, 1, 1);
    row++;
    
    gtk_box_append(GTK_BOX(content_box), form_grid);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    // Additional options
    GtkWidget* options_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(options_frame, "options-frame");
    
    GtkWidget* options_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(options_box, 16);
    gtk_widget_set_margin_bottom(options_box, 16);
    gtk_widget_set_margin_start(options_box, 16);
    gtk_widget_set_margin_end(options_box, 16);
    
    GtkWidget* admin_check = gtk_check_button_new_with_label("Add this user to administrators group");
    gtk_widget_add_css_class(admin_check, "admin-check");
    gtk_check_button_set_active(GTK_CHECK_BUTTON(admin_check), TRUE);
    gtk_box_append(GTK_BOX(options_box), admin_check);
    
    GtkWidget* autologin_check = gtk_check_button_new_with_label("Log in automatically");
    gtk_widget_add_css_class(autologin_check, "autologin-check");
    gtk_box_append(GTK_BOX(options_box), autologin_check);
    
    gtk_frame_set_child(GTK_FRAME(options_frame), options_box);
    gtk_box_append(GTK_BOX(page), options_frame);
    
    // Password requirements info
    GtkWidget* info_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(info_frame, "info-frame");
    
    GtkWidget* info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(info_box, 16);
    gtk_widget_set_margin_bottom(info_box, 16);
    gtk_widget_set_margin_start(info_box, 16);
    gtk_widget_set_margin_end(info_box, 16);
    
    GtkWidget* info_icon = gtk_image_new_from_icon_name("dialog-information-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(info_icon), 16);
    gtk_widget_add_css_class(info_icon, "info-icon");
    gtk_box_append(GTK_BOX(info_box), info_icon);
    
    GtkWidget* info_text = gtk_label_new("Use a strong password with at least 8 characters, including uppercase, lowercase, numbers, and special characters.");
    gtk_widget_add_css_class(info_text, "info-text");
    gtk_label_set_wrap(GTK_LABEL(info_text), TRUE);
    gtk_box_append(GTK_BOX(info_box), info_text);
    
    gtk_frame_set_child(GTK_FRAME(info_frame), info_box);
    gtk_box_append(GTK_BOX(page), info_frame);
    
    return page;
}
