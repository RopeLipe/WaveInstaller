#include "welcome.h"

struct _WelcomeScreen {
    GObject parent_instance;
    GtkWidget *widget;
};

static void welcome_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(WelcomeScreen, welcome_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 welcome_screen_interface_init))

static GtkWidget *welcome_screen_create_widget(InstallerScreen *screen) {
    WelcomeScreen *self = WELCOME_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
      // Main container
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    
    // Welcome container with centering
    GtkWidget *welcome_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(welcome_container, "welcome-container");
    gtk_widget_set_halign(welcome_container, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(welcome_container, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(welcome_container, TRUE);
    gtk_widget_set_vexpand(welcome_container, TRUE);
    
    // Welcome Title
    GtkWidget *title = gtk_label_new("Welcome to Wave OS");
    gtk_widget_add_css_class(title, "welcome-title");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    
    // Subtitle
    GtkWidget *subtitle = gtk_label_new("This installer will guide you through setting up your new operating system.\\n"
                                       "Click Next to get started with the installation process.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_CENTER);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
      // Feature highlights (text only)
    GtkWidget *features_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 48);
    gtk_widget_set_halign(features_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(features_box, 32);
    
    // Feature 1: Easy Setup
    GtkWidget *feature1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget *feature1_label = gtk_label_new("Easy Setup");
    gtk_widget_add_css_class(feature1_label, "text-muted");
    gtk_box_append(GTK_BOX(feature1), feature1_label);
    
    // Feature 2: Modern Interface
    GtkWidget *feature2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget *feature2_label = gtk_label_new("Modern Interface");
    gtk_widget_add_css_class(feature2_label, "text-muted");
    gtk_box_append(GTK_BOX(feature2), feature2_label);
    
    // Feature 3: Secure
    GtkWidget *feature3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget *feature3_label = gtk_label_new("Secure");
    gtk_widget_add_css_class(feature3_label, "text-muted");
    gtk_box_append(GTK_BOX(feature3), feature3_label);
    
    gtk_box_append(GTK_BOX(features_box), feature1);
    gtk_box_append(GTK_BOX(features_box), feature2);
    gtk_box_append(GTK_BOX(features_box), feature3);
      // Add all elements to welcome container
    gtk_box_append(GTK_BOX(welcome_container), title);
    gtk_box_append(GTK_BOX(welcome_container), subtitle);
    gtk_box_append(GTK_BOX(welcome_container), features_box);
    
    gtk_box_append(GTK_BOX(self->widget), welcome_container);
    
    return self->widget;
}

static void welcome_screen_show_screen(InstallerScreen *screen) {
    // Called when screen becomes visible
}

static void welcome_screen_hide_screen(InstallerScreen *screen) {
    // Called when screen becomes hidden
}

static gboolean welcome_screen_validate(InstallerScreen *screen) {
    // Welcome screen is always valid
    return TRUE;
}

static void welcome_screen_next_clicked(InstallerScreen *screen) {
    // Handle next button click if needed
}

static void welcome_screen_back_clicked(InstallerScreen *screen) {
    // Handle back button click if needed
}

static void welcome_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = welcome_screen_create_widget;
    iface->show_screen = welcome_screen_show_screen;
    iface->hide_screen = welcome_screen_hide_screen;
    iface->validate = welcome_screen_validate;
    iface->next_clicked = welcome_screen_next_clicked;
    iface->back_clicked = welcome_screen_back_clicked;
}

static void welcome_screen_init(WelcomeScreen *self) {
    self->widget = NULL;
}

static void welcome_screen_class_init(WelcomeScreenClass *klass) {
    // Class initialization
}

WelcomeScreen *welcome_screen_new(void) {
    return g_object_new(WELCOME_TYPE_SCREEN, NULL);
}
