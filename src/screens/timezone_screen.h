#ifndef TIMEZONE_SCREEN_H
#define TIMEZONE_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the timezone selection screen
GtkWidget* create_timezone_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // TIMEZONE_SCREEN_H
