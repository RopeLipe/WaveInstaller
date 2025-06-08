#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For NextScreenCallback

// Function to create the welcome screen
GtkWidget* create_welcome_screen(NextScreenCallback on_next);

#endif // WELCOME_SCREEN_H
