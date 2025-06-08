#include "installer.h"

struct _InstallerWindow {
    GtkApplicationWindow parent_instance;
    
    GtkWidget *stack;
    GtkWidget *header_bar;
    GtkWidget *back_button;
    GtkWidget *next_button;
    GtkWidget *step_indicator;
    GtkWidget *navigation_box;
    GtkWidget *step_circles[8];
    
    InstallerScreen *current_screen;
    ScreenType current_screen_type;
    
    // Screen instances
    InstallerScreen *welcome_screen;
    InstallerScreen *language_screen;
    InstallerScreen *timezone_screen;
    InstallerScreen *keyboard_screen;
    InstallerScreen *disk_screen;
    InstallerScreen *network_screen;
    InstallerScreen *user_screen;
    InstallerScreen *install_screen;
};

G_DEFINE_FINAL_TYPE(InstallerWindow, installer_window, GTK_TYPE_APPLICATION_WINDOW)

// Base screen interface implementation
G_DEFINE_INTERFACE(InstallerScreen, installer_screen, G_TYPE_OBJECT)

static void installer_screen_default_init(InstallerScreenInterface *iface) {
    // Default implementations can go here
}

GtkWidget *installer_screen_create_widget(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    g_return_val_if_fail(iface->create_widget != NULL, NULL);
    return iface->create_widget(self);
}

void installer_screen_show_screen(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    if (iface->show_screen)
        iface->show_screen(self);
}

void installer_screen_hide_screen(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    if (iface->hide_screen)
        iface->hide_screen(self);
}

gboolean installer_screen_validate(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    if (iface->validate)
        return iface->validate(self);
    return TRUE;
}

void installer_screen_next_clicked(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    if (iface->next_clicked)
        iface->next_clicked(self);
}

void installer_screen_back_clicked(InstallerScreen *self) {
    InstallerScreenInterface *iface = INSTALLER_SCREEN_GET_IFACE(self);
    if (iface->back_clicked)
        iface->back_clicked(self);
}

static void update_step_indicator(InstallerWindow *self) {
    // Update step circles to show current progress
    for (int i = 0; i < 8; i++) {
        GtkWidget *circle = self->step_circles[i];
        gtk_widget_remove_css_class(circle, "current");
        gtk_widget_remove_css_class(circle, "completed");
        
        if (i < self->current_screen_type) {
            gtk_widget_add_css_class(circle, "completed");
        } else if (i == self->current_screen_type) {
            gtk_widget_add_css_class(circle, "current");
        }
    }
}

static void on_back_clicked(GtkButton *button, InstallerWindow *self) {
    if (self->current_screen_type > SCREEN_WELCOME) {
        installer_screen_back_clicked(self->current_screen);
        
        // Navigate to previous screen
        ScreenType prev_screen = self->current_screen_type - 1;
        
        switch (prev_screen) {
            case SCREEN_WELCOME:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "welcome");
                self->current_screen = self->welcome_screen;
                break;
            case SCREEN_LANGUAGE:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "language");
                self->current_screen = self->language_screen;
                break;
            case SCREEN_TIMEZONE:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "timezone");
                self->current_screen = self->timezone_screen;
                break;
            case SCREEN_KEYBOARD:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "keyboard");
                self->current_screen = self->keyboard_screen;
                break;
            case SCREEN_DISK:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "disk");
                self->current_screen = self->disk_screen;
                break;
            case SCREEN_NETWORK:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "network");
                self->current_screen = self->network_screen;
                break;
            case SCREEN_USER:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "user");
                self->current_screen = self->user_screen;
                break;
            default:
                break;        }
        
        self->current_screen_type = prev_screen;
        update_step_indicator(self);
        
        // Update button visibility
        gtk_widget_set_visible(self->back_button, self->current_screen_type > SCREEN_WELCOME);
    }
}

static void on_next_clicked(GtkButton *button, InstallerWindow *self) {
    if (!installer_screen_validate(self->current_screen)) {
        return;
    }
    
    installer_screen_next_clicked(self->current_screen);
    
    if (self->current_screen_type < SCREEN_INSTALL) {
        // Navigate to next screen
        ScreenType next_screen = self->current_screen_type + 1;
        
        switch (next_screen) {
            case SCREEN_LANGUAGE:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "language");
                self->current_screen = self->language_screen;
                break;
            case SCREEN_TIMEZONE:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "timezone");
                self->current_screen = self->timezone_screen;
                break;
            case SCREEN_KEYBOARD:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "keyboard");
                self->current_screen = self->keyboard_screen;
                break;
            case SCREEN_DISK:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "disk");
                self->current_screen = self->disk_screen;
                break;            case SCREEN_NETWORK:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "network");
                self->current_screen = self->network_screen;
                break;
            case SCREEN_USER:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "user");
                self->current_screen = self->user_screen;
                break;
            case SCREEN_INSTALL:
                gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "install");
                self->current_screen = self->install_screen;
                gtk_widget_set_visible(self->next_button, FALSE);
                break;
            default:
                break;        }
        
        self->current_screen_type = next_screen;
        update_step_indicator(self);
        
        // Update button visibility
        gtk_widget_set_visible(self->back_button, self->current_screen_type > SCREEN_WELCOME);
    }
}

static void installer_window_init(InstallerWindow *self) {
    // Set window properties
    gtk_window_set_title(GTK_WINDOW(self), "Wave OS Installer");
    gtk_window_set_default_size(GTK_WINDOW(self), 900, 650);
    gtk_window_set_resizable(GTK_WINDOW(self), FALSE);
    gtk_window_set_decorated(GTK_WINDOW(self), FALSE);
    
    // Create header bar with step indicator
    self->header_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(self->header_bar, "header-bar");
    
    // Circular step indicator
    self->step_indicator = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(self->step_indicator, "step-indicator");
    gtk_widget_set_halign(self->step_indicator, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(self->step_indicator, TRUE);
    
    // Create step circles
    const gchar *step_names[] = {"Welcome", "Language", "Timezone", "Keyboard", "Disk", "Network", "User", "Install"};
    for (int i = 0; i < 8; i++) {
        GtkWidget *step_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        
        GtkWidget *step_circle = gtk_label_new(g_strdup_printf("%d", i + 1));
        gtk_widget_add_css_class(step_circle, "step-circle");
        if (i == 0) gtk_widget_add_css_class(step_circle, "current");
        gtk_widget_set_size_request(step_circle, 32, 32);
        
        // Store reference to circle for updates
        self->step_circles[i] = step_circle;
        
        gtk_box_append(GTK_BOX(step_container), step_circle);
        gtk_box_append(GTK_BOX(self->step_indicator), step_container);
        
        // Add connector line (except for last step)
        if (i < 7) {
            GtkWidget *connector = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_widget_add_css_class(connector, "step-connector");
            gtk_widget_set_hexpand(connector, TRUE);
            gtk_widget_set_size_request(connector, 40, 2);
            gtk_box_append(GTK_BOX(self->step_indicator), connector);
        }
    }
    
    gtk_box_append(GTK_BOX(self->header_bar), self->step_indicator);
    
    // Create main stack
    self->stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(self->stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(self->stack), 300);
    
    // Create screens
    self->welcome_screen = INSTALLER_SCREEN(welcome_screen_new());
    self->language_screen = INSTALLER_SCREEN(language_screen_new());
    self->timezone_screen = INSTALLER_SCREEN(timezone_screen_new());
    self->keyboard_screen = INSTALLER_SCREEN(keyboard_screen_new());
    self->disk_screen = INSTALLER_SCREEN(disk_screen_new());
    self->network_screen = INSTALLER_SCREEN(network_screen_new());
    self->user_screen = INSTALLER_SCREEN(user_screen_new());
    self->install_screen = INSTALLER_SCREEN(install_screen_new());
    
    // Add screens to stack
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->welcome_screen), "welcome");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->language_screen), "language");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->timezone_screen), "timezone");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->keyboard_screen), "keyboard");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->disk_screen), "disk");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->network_screen), "network");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->user_screen), "user");
    gtk_stack_add_named(GTK_STACK(self->stack), 
                       installer_screen_create_widget(self->install_screen), "install");
    
    // Set initial screen
    self->current_screen = self->welcome_screen;
    self->current_screen_type = SCREEN_WELCOME;
    gtk_stack_set_visible_child_name(GTK_STACK(self->stack), "welcome");
    
    // Create navigation buttons    self->back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(self->back_button, "nav-button");
    gtk_widget_add_css_class(self->back_button, "back-button");
    gtk_widget_set_visible(self->back_button, FALSE);
    g_signal_connect(self->back_button, "clicked", G_CALLBACK(on_back_clicked), self);
      self->next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(self->next_button, "nav-button");
    gtk_widget_add_css_class(self->next_button, "next-button");
    g_signal_connect(self->next_button, "clicked", G_CALLBACK(on_next_clicked), self);
    
    // Create navigation box (bottom right)
    self->navigation_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(self->navigation_box, "navigation-box");
    gtk_widget_set_halign(self->navigation_box, GTK_ALIGN_END);
    gtk_widget_set_valign(self->navigation_box, GTK_ALIGN_END);
    gtk_widget_set_margin_end(self->navigation_box, 24);
    gtk_widget_set_margin_bottom(self->navigation_box, 24);
    
    gtk_box_append(GTK_BOX(self->navigation_box), self->back_button);
    gtk_box_append(GTK_BOX(self->navigation_box), self->next_button);
    
    // Create main layout with overlay for navigation
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append(GTK_BOX(main_box), self->header_bar);
    gtk_box_append(GTK_BOX(main_box), self->stack);
    
    // Create overlay to position navigation buttons
    GtkWidget *overlay = gtk_overlay_new();
    gtk_overlay_set_child(GTK_OVERLAY(overlay), main_box);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), self->navigation_box);
    
    gtk_window_set_child(GTK_WINDOW(self), overlay);
}

static void installer_window_class_init(InstallerWindowClass *klass) {
    // Class initialization
}

InstallerWindow *installer_window_new(GtkApplication *app) {
    return g_object_new(INSTALLER_TYPE_WINDOW, "application", app, NULL);
}
