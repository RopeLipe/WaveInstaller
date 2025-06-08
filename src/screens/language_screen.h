#ifndef LANGUAGE_SCREEN_H
#define LANGUAGE_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the language selection screen
GtkWidget* create_language_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // LANGUAGE_SCREEN_H
