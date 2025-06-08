#ifndef KEYBOARD_SCREEN_H
#define KEYBOARD_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the keyboard layout selection screen
GtkWidget* create_keyboard_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // KEYBOARD_SCREEN_H
