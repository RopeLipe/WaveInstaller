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
      // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 32);
    gtk_widget_set_margin_top(title_box, 24);
    
    GtkWidget *title = gtk_label_new("Create Your Account");
    gtk_widget_add_css_class(title, "welcome-title");
    
    GtkWidget *subtitle = gtk_label_new("Set up your user account to personalize your Wave OS experience.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
      // Form container
    GtkWidget *form_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(form_container, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(form_container, "user-form");
    
    // Full name field
    GtkWidget *fullname_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(fullname_group, "form-group");
    
    GtkWidget *fullname_label = gtk_label_new("Full Name");
    gtk_widget_add_css_class(fullname_label, "form-label");
    gtk_widget_set_halign(fullname_label, GTK_ALIGN_START);
    
    self->fullname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->fullname_entry), "Enter your full name");
    gtk_widget_add_css_class(self->fullname_entry, "form-entry");
    g_signal_connect(self->fullname_entry, "changed", G_CALLBACK(on_fullname_changed), self);
    
    gtk_box_append(GTK_BOX(fullname_group), fullname_label);
    gtk_box_append(GTK_BOX(fullname_group), self->fullname_entry);
    
    // Username field
    GtkWidget *username_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(username_group, "form-group");
    
    GtkWidget *username_label = gtk_label_new("Username");
    gtk_widget_add_css_class(username_label, "form-label");
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);
    
    self->username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->username_entry), "Enter username");
    gtk_widget_add_css_class(self->username_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(username_group), username_label);
    gtk_box_append(GTK_BOX(username_group), self->username_entry);
    
    // Password field
    GtkWidget *password_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(password_group, "form-group");
    
    GtkWidget *password_label = gtk_label_new("Password");
    gtk_widget_add_css_class(password_label, "form-label");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
      self->password_entry = gtk_password_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->password_entry), "Enter password");
    gtk_widget_add_css_class(self->password_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(password_group), password_label);
    gtk_box_append(GTK_BOX(password_group), self->password_entry);
    
    // Confirm password field
    GtkWidget *confirm_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(confirm_group, "form-group");
    
    GtkWidget *confirm_label = gtk_label_new("Confirm Password");
    gtk_widget_add_css_class(confirm_label, "form-label");
    gtk_widget_set_halign(confirm_label, GTK_ALIGN_START);
      self->confirm_entry = gtk_password_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->confirm_entry), "Confirm password");
    gtk_widget_add_css_class(self->confirm_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(confirm_group), confirm_label);
    gtk_box_append(GTK_BOX(confirm_group), self->confirm_entry);
    
    // Options
    GtkWidget *options_group = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_add_css_class(options_group, "form-group");
    gtk_widget_set_margin_top(options_group, 16);
    
    self->auto_login_check = gtk_check_button_new_with_label("Log in automatically");
    
    gtk_box_append(GTK_BOX(options_group), self->auto_login_check);
      // Add all sections to form
    gtk_box_append(GTK_BOX(form_container), fullname_group);
    gtk_box_append(GTK_BOX(form_container), username_group);
    gtk_box_append(GTK_BOX(form_container), password_group);
    gtk_box_append(GTK_BOX(form_container), confirm_group);
    gtk_box_append(GTK_BOX(form_container), options_group);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), form_container);
    
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
