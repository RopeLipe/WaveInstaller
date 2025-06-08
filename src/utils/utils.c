#include "utils.h"

GtkWidget* create_navigation_box(NextScreenCallback next_callback, PrevScreenCallback prev_callback, gpointer user_data_next, gpointer user_data_prev, const char* next_label) {
    GtkWidget *nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(nav_box, GTK_ALIGN_END);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);
    gtk_widget_set_margin_top(nav_box, 20);
    gtk_widget_set_margin_bottom(nav_box, 10);
    gtk_widget_set_margin_start(nav_box, 10);
    gtk_widget_set_margin_end(nav_box, 10);
    gtk_widget_add_css_class(nav_box, "navigation-box");

    if (prev_callback) {
        GtkWidget *prev_button = gtk_button_new_with_label("Previous");
        gtk_widget_add_css_class(prev_button, "nav-button");
        g_signal_connect(prev_button, "clicked", G_CALLBACK(prev_callback), user_data_prev);
        gtk_box_append(GTK_BOX(nav_box), prev_button);
    }

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(nav_box), spacer);


    if (next_callback) {
        const char* button_label = (next_label != NULL) ? next_label : "Next";
        GtkWidget *next_button = gtk_button_new_with_label(button_label);
        gtk_widget_add_css_class(next_button, "nav-button");
        g_signal_connect(next_button, "clicked", G_CALLBACK(next_callback), user_data_next);
        gtk_box_append(GTK_BOX(nav_box), next_button);
    }

    return nav_box;
}

GtkWidget* create_image_from_file(const char* filename, int width, int height) {
    // Construct the full path to the image
    char path[256];
    g_snprintf(path, sizeof(path), "/usr/bin/WaveInstaller/assets/icons/%s", filename);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(path, width, height, NULL);
    if (!pixbuf) {
        g_warning("Failed to load image: %s", path);
        // Return a placeholder or NULL if image loading fails
        GtkWidget *label = gtk_label_new("Image not found");
        return label;
    }
    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    return image;
}
