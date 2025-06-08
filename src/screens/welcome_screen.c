#include "welcome_screen.h"
#include "language_screen.h" // To navigate to the next screen
#include <gtk/gtk.h>

// Forward declaration for the navigation function, assuming it's in main.c or a shared utility
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_language_screen(GtkButton *button, gpointer user_data);


GtkWidget *create_welcome_screen(void) {
    GtkWidget *grid;
    GtkWidget *title_label;
    GtkWidget *welcome_image;
    GtkWidget *message_label;
    GtkWidget *next_button;
    GtkWidget *box;

    // Main box container for the screen
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "welcome-screen-box"); // For specific styling

    // Optional: Image/Icon (Placeholder)
    // You would use GtkPicture or GtkImage here
    // For example, if you have an icon:
    // welcome_image = gtk_image_new_from_file("assets/icons/welcome_icon.png"); // Update path
    // gtk_widget_set_margin_bottom(welcome_image, 20);
    // gtk_box_append(GTK_BOX(box), welcome_image);
    // For now, let's use a placeholder label for the icon
    welcome_image = gtk_label_new("ICON"); // Placeholder for an icon
    gtk_widget_add_css_class(welcome_image, "welcome-icon"); // CSS class for styling
    gtk_widget_set_margin_bottom(welcome_image, 20);
    gtk_box_append(GTK_BOX(box), welcome_image);


    title_label = gtk_label_new("Welcome to Wave Installer");
    gtk_widget_add_css_class(title_label, "title-label"); // CSS class for styling
    gtk_widget_set_margin_bottom(title_label, 10);
    gtk_box_append(GTK_BOX(box), title_label);

    message_label = gtk_label_new("We're excited to help you set up your new system.\nFollow the steps to configure and install.");
    gtk_label_set_justify(GTK_LABEL(message_label), GTK_JUSTIFY_CENTER);
    gtk_widget_add_css_class(message_label, "message-label"); // CSS class for styling
    gtk_widget_set_margin_bottom(message_label, 30);
    gtk_box_append(GTK_BOX(box), message_label);

    next_button = gtk_button_new_with_label("Get Started");
    gtk_widget_add_css_class(next_button, "next-button"); // CSS class for styling
    gtk_widget_set_halign(next_button, GTK_ALIGN_CENTER);
    // Connect the button to the navigation function (defined in main.c or similar)
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_language_screen), NULL);
    gtk_box_append(GTK_BOX(box), next_button);
    
    // The main container for this screen will be the box.
    // If you need a GtkGrid for more complex layouts within a screen, you can use it here.
    // For this simple welcome screen, a GtkBox is sufficient.
    // grid = gtk_grid_new();
    // gtk_grid_attach(GTK_GRID(grid), box, 0, 0, 1, 1);
    // return grid; // If using grid

    return box; // Return the main container for this screen
}
