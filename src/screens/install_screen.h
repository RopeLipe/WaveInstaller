#ifndef INSTALL_SCREEN_H
#define INSTALL_SCREEN_H

#include <gtk/gtk.h>

// Function to create the installation progress screen
GtkWidget* create_install_screen(GtkApplication *app); // Pass app to allow closing on finish/error

#endif // INSTALL_SCREEN_H
