#include "user.h"

struct _UserScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *fullname_entry;
    GtkWidget *username_entry;
    GtkWidget *password_entry;
    GtkWidget *confirm_entry;
    GtkWidget *avatar_image;
    GtkWidget *auto_login_check;
};

static void user_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(UserScreen, user_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 user_screen_interface_init))

static void on_fullname_changed(GtkEditable *editable, UserScreen *self) {
    const gchar *fullname = gtk_editable_get_text(editable);
    
    // Auto-generate username from full name (simple implementation)
    if (fullname && strlen(fullname) > 0) {
        gchar *username = g_ascii_strdown(fullname, -1);
        // Replace spaces with underscores and remove non-alphanumeric chars
        for (int i = 0; username[i]; i++) {
            if (!g_ascii_isalnum(username[i])) {
                username[i] = '_';
            }
        }
        
        // Set username if it's empty
        if (strlen(gtk_editable_get_text(GTK_EDITABLE(self->username_entry))) == 0) {
            gtk_editable_set_text(GTK_EDITABLE(self->username_entry), username);
        }
        
        g_free(username);
    }
}

static void on_avatar_clicked(GtkButton *button, UserScreen *self) {
    // In a real implementation, this would open a file chooser for avatar selection
    // For now, we'll just cycle through some default avatars
    static int avatar_index = 0;
    const gchar *avatars[] = {
        "avatar-default",
        "avatar-default-symbolic",
        "user-info",
        NULL
    };
    
    avatar_index = (avatar_index + 1) % 3;
    gtk_image_set_from_icon_name(GTK_IMAGE(self->avatar_image), avatars[avatar_index]);
}

static GtkWidget *user_screen_create_widget(InstallerScreen *screen) {
    UserScreen *self = USER_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
    
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    gtk_widget_set_margin_top(self->widget, 48);
    gtk_widget_set_margin_start(self->widget, 32);
    gtk_widget_set_margin_end(self->widget, 32);
    
    // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 40);
    
    GtkWidget *title = gtk_label_new("Create Your Account");
    gtk_widget_add_css_class(title, "welcome-title");
    
    GtkWidget *subtitle = gtk_label_new("Set up your personal account to customize your Wave OS experience.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Main form card
    GtkWidget *form_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(form_card, "user-form-card");
    gtk_widget_set_halign(form_card, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(form_card, 500, -1);
    
    // Form header with user icon
    GtkWidget *form_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(form_header, "form-header");
    gtk_widget_set_halign(form_header, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(form_header, 32);
    
    GtkWidget *user_icon = gtk_image_new_from_icon_name("avatar-default");
    gtk_image_set_pixel_size(GTK_IMAGE(user_icon), 64);
    gtk_widget_add_css_class(user_icon, "user-avatar");
    
    GtkWidget *header_text = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    GtkWidget *header_title = gtk_label_new("Personal Information");
    gtk_widget_add_css_class(header_title, "form-header-title");
    GtkWidget *header_subtitle = gtk_label_new("We'll use this to set up your account");
    gtk_widget_add_css_class(header_subtitle, "form-header-subtitle");
    
    gtk_box_append(GTK_BOX(header_text), header_title);
    gtk_box_append(GTK_BOX(header_text), header_subtitle);
    
    gtk_box_append(GTK_BOX(form_header), user_icon);
    gtk_box_append(GTK_BOX(form_header), header_text);
    
    // Form fields container
    GtkWidget *fields_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_margin_start(fields_container, 32);
    gtk_widget_set_margin_end(fields_container, 32);
    
    // Full name field with icon
    GtkWidget *fullname_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_add_css_class(fullname_group, "form-group");
    
    GtkWidget *fullname_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *name_icon = gtk_image_new_from_icon_name("contact-new");
    gtk_image_set_pixel_size(GTK_IMAGE(name_icon), 16);
    gtk_widget_add_css_class(name_icon, "field-icon");
    
    GtkWidget *fullname_label = gtk_label_new("Full Name");
    gtk_widget_add_css_class(fullname_label, "form-label");
    
    gtk_box_append(GTK_BOX(fullname_label_box), name_icon);
    gtk_box_append(GTK_BOX(fullname_label_box), fullname_label);
    
    self->fullname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->fullname_entry), "Enter your full name");
    gtk_widget_add_css_class(self->fullname_entry, "form-entry");
    g_signal_connect(self->fullname_entry, "changed", G_CALLBACK(on_fullname_changed), self);
    
    gtk_box_append(GTK_BOX(fullname_group), fullname_label_box);
    gtk_box_append(GTK_BOX(fullname_group), self->fullname_entry);
    
    // Username field with icon
    GtkWidget *username_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_add_css_class(username_group, "form-group");
    
    GtkWidget *username_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *user_icon_small = gtk_image_new_from_icon_name("system-users");
    gtk_image_set_pixel_size(GTK_IMAGE(user_icon_small), 16);
    gtk_widget_add_css_class(user_icon_small, "field-icon");
    
    GtkWidget *username_label = gtk_label_new("Username");
    gtk_widget_add_css_class(username_label, "form-label");
    
    gtk_box_append(GTK_BOX(username_label_box), user_icon_small);
    gtk_box_append(GTK_BOX(username_label_box), username_label);
    
    self->username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->username_entry), "Choose a username");
    gtk_widget_add_css_class(self->username_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(username_group), username_label_box);
    gtk_box_append(GTK_BOX(username_group), self->username_entry);
    
    // Password fields in a two-column layout
    GtkWidget *password_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    
    // Password field
    GtkWidget *password_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_add_css_class(password_group, "form-group");
    gtk_widget_set_hexpand(password_group, TRUE);
    
    GtkWidget *password_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *lock_icon = gtk_image_new_from_icon_name("security-high");
    gtk_image_set_pixel_size(GTK_IMAGE(lock_icon), 16);
    gtk_widget_add_css_class(lock_icon, "field-icon");
    
    GtkWidget *password_label = gtk_label_new("Password");
    gtk_widget_add_css_class(password_label, "form-label");
    
    gtk_box_append(GTK_BOX(password_label_box), lock_icon);
    gtk_box_append(GTK_BOX(password_label_box), password_label);
    
    self->password_entry = gtk_password_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->password_entry), "Enter password");
    gtk_widget_add_css_class(self->password_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(password_group), password_label_box);
    gtk_box_append(GTK_BOX(password_group), self->password_entry);
    
    // Confirm password field
    GtkWidget *confirm_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_add_css_class(confirm_group, "form-group");
    gtk_widget_set_hexpand(confirm_group, TRUE);
    
    GtkWidget *confirm_label_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *confirm_icon = gtk_image_new_from_icon_name("security-high");
    gtk_image_set_pixel_size(GTK_IMAGE(confirm_icon), 16);
    gtk_widget_add_css_class(confirm_icon, "field-icon");
    
    GtkWidget *confirm_label = gtk_label_new("Confirm Password");
    gtk_widget_add_css_class(confirm_label, "form-label");
    
    gtk_box_append(GTK_BOX(confirm_label_box), confirm_icon);
    gtk_box_append(GTK_BOX(confirm_label_box), confirm_label);
    
    self->confirm_entry = gtk_password_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->confirm_entry), "Confirm password");
    gtk_widget_add_css_class(self->confirm_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(confirm_group), confirm_label_box);
    gtk_box_append(GTK_BOX(confirm_group), self->confirm_entry);
    
    gtk_box_append(GTK_BOX(password_container), password_group);
    gtk_box_append(GTK_BOX(password_container), confirm_group);
    
    // Options section
    GtkWidget *options_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_add_css_class(options_section, "options-section");
    gtk_widget_set_margin_top(options_section, 24);
    
    GtkWidget *options_header = gtk_label_new("Account Options");
    gtk_widget_add_css_class(options_header, "section-header");
    gtk_widget_set_halign(options_header, GTK_ALIGN_START);
    
    GtkWidget *options_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *auto_login_icon = gtk_image_new_from_icon_name("emblem-default");
    gtk_image_set_pixel_size(GTK_IMAGE(auto_login_icon), 16);
    gtk_widget_add_css_class(auto_login_icon, "option-icon");
    
    self->auto_login_check = gtk_check_button_new_with_label("Log in automatically at startup");
    gtk_widget_add_css_class(self->auto_login_check, "form-checkbox");
    
    gtk_box_append(GTK_BOX(options_box), auto_login_icon);
    gtk_box_append(GTK_BOX(options_box), self->auto_login_check);
    
    gtk_box_append(GTK_BOX(options_section), options_header);
    gtk_box_append(GTK_BOX(options_section), options_box);
    
    // Assemble all form elements
    gtk_box_append(GTK_BOX(fields_container), fullname_group);
    gtk_box_append(GTK_BOX(fields_container), username_group);
    gtk_box_append(GTK_BOX(fields_container), password_container);
    gtk_box_append(GTK_BOX(fields_container), options_section);
    
    gtk_box_append(GTK_BOX(form_card), form_header);
    gtk_box_append(GTK_BOX(form_card), fields_container);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), form_card);
    
    return self->widget;
}

static gboolean user_screen_validate(InstallerScreen *screen) {
    UserScreen *self = USER_SCREEN(screen);
    
    const gchar *fullname = gtk_editable_get_text(GTK_EDITABLE(self->fullname_entry));
    const gchar *username = gtk_editable_get_text(GTK_EDITABLE(self->username_entry));
    const gchar *password = gtk_editable_get_text(GTK_EDITABLE(self->password_entry));
    const gchar *confirm = gtk_editable_get_text(GTK_EDITABLE(self->confirm_entry));
    
    // Basic validation
    if (!fullname || strlen(fullname) == 0) {
        return FALSE;
    }
    
    if (!username || strlen(username) == 0) {
        return FALSE;
    }
    
    if (!password || strlen(password) < 4) {
        return FALSE;
    }
    
    if (g_strcmp0(password, confirm) != 0) {
        return FALSE;
    }
    
    return TRUE;
}

static void user_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = user_screen_create_widget;
    iface->validate = user_screen_validate;
}

static void user_screen_init(UserScreen *self) {
    self->widget = NULL;
}

static void user_screen_class_init(UserScreenClass *klass) {
    // Class initialization
}

UserScreen *user_screen_new(void) {
    return g_object_new(USER_TYPE_SCREEN, NULL);
}
