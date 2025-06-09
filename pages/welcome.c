#include "installer.h"

GtkWidget* create_welcome_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_CENTER);
    
    // Logo or icon area
    GtkWidget* logo_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(logo_box, GTK_ALIGN_CENTER);
    
    // Main icon
    GtkWidget* icon = gtk_image_new_from_icon_name("system-software-install");
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 96);
    gtk_widget_add_css_class(icon, "welcome-icon");
    gtk_box_append(GTK_BOX(logo_box), icon);
    
    // Welcome title
    GtkWidget* title = gtk_label_new("Welcome to Wave Installer");
    gtk_widget_add_css_class(title, "welcome-title");
    gtk_box_append(GTK_BOX(logo_box), title);
    
    // Subtitle
    GtkWidget* subtitle = gtk_label_new("This installer will guide you through the process of installing Wave OS on your computer.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    gtk_label_set_max_width_chars(GTK_LABEL(subtitle), 60);
    gtk_box_append(GTK_BOX(logo_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), logo_box);
    
    // Features list
    GtkWidget* features_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(features_box, GTK_ALIGN_CENTER);
    
    // Feature items
    const char* features[] = {
        "Modern and intuitive interface",
        "Secure installation process",
        "Automatic hardware detection",
        "Multiple language support"
    };
    
    for (int i = 0; i < 4; i++) {
        GtkWidget* feature_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        gtk_widget_set_halign(feature_box, GTK_ALIGN_START);
        
        GtkWidget* check_icon = gtk_image_new_from_icon_name("emblem-ok-symbolic");
        gtk_image_set_pixel_size(GTK_IMAGE(check_icon), 16);
        gtk_widget_add_css_class(check_icon, "feature-check");
        gtk_box_append(GTK_BOX(feature_box), check_icon);
        
        GtkWidget* feature_label = gtk_label_new(features[i]);
        gtk_widget_add_css_class(feature_label, "feature-text");
        gtk_box_append(GTK_BOX(feature_box), feature_label);
        
        gtk_box_append(GTK_BOX(features_box), feature_box);
    }
    
    gtk_box_append(GTK_BOX(page), features_box);
    
    return page;
}
