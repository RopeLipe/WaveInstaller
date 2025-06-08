#include "welcome_screen.h"
#include "../utils/utils.h" // For create_image_from_file and create_navigation_box

GtkWidget* create_welcome_screen(NextScreenCallback on_next) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    // Add a placeholder icon (replace 'welcome_icon.png' with your actual icon file)
    GtkWidget *icon = create_image_from_file("wave_logo.png", 128, 128); // Example size
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        GtkWidget *icon_placeholder = gtk_label_new("(Icon)");
        gtk_widget_set_halign(icon_placeholder, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon_placeholder);
    }

    GtkWidget *title = gtk_label_new("Welcome to Wave Installer");
    gtk_widget_add_css_class(title, "welcome-title");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *subtitle = gtk_label_new("We'll guide you through the setup process.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), subtitle);

    // Navigation
    GtkWidget *nav_box = create_navigation_box(on_next, NULL, NULL, NULL, "Get Started");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE); // Don't let nav_box expand vertically
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END); // Align to bottom


    return box;
}
