#ifndef USER_CONFIG_SCREEN_H
#define USER_CONFIG_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the user configuration screen
GtkWidget* create_user_config_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // USER_CONFIG_SCREEN_H
