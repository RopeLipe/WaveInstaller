/* welcome-screen.c - Welcome screen implementation */
#include "welcome-screen.h"

static void on_quit_clicked(GtkButton *button, WaveInstallerApplication *app);
static void on_get_started_clicked(GtkButton *button, WaveInstallerApplication *app);

GtkWidget *
welcome_screen_create(WaveInstallerApplication *app) {
    /* Main container */
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_widget_set_valign(content, GTK_ALIGN_CENTER);
    gtk_widget_set_vexpand(content, TRUE);
    
    /* Title */
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), 
                        "<span size='large' weight='bold'>Welcome to Wave Installer</span>");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    GtkWidget *subtitle = gtk_label_new("Let's get your system set up quickly and easily");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    gtk_box_append(GTK_BOX(container), content);
    
    /* Navigation buttons */
    GtkWidget *nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(nav_box, "nav-buttons");
    gtk_widget_set_halign(nav_box, GTK_ALIGN_FILL);
    
    /* Quit button */
    GtkWidget *quit_btn = gtk_button_new_with_label("Quit");
    gtk_widget_add_css_class(quit_btn, "secondary-button");
    g_signal_connect(quit_btn, "clicked", G_CALLBACK(on_quit_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), quit_btn);
    
    /* Get Started button */
    GtkWidget *next_btn = gtk_button_new_with_label("Get Started");
    gtk_widget_add_css_class(next_btn, "primary-button");
    g_signal_connect(next_btn, "clicked", G_CALLBACK(on_get_started_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), next_btn);
    
    gtk_box_append(GTK_BOX(container), nav_box);
    
    /* Store reference for the app */
    app->welcome_screen = container;
    app->screens[SCREEN_WELCOME].widget = container;
    
    return container;
}

static void
on_quit_clicked(GtkButton *button, WaveInstallerApplication *app) {
    wave_installer_application_quit_installer(app);
}

static void
on_get_started_clicked(GtkButton *button, WaveInstallerApplication *app) {
    wave_installer_application_next_screen(app);
}
