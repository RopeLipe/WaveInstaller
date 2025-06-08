#ifndef NETWORK_CONFIG_SCREEN_H
#define NETWORK_CONFIG_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the network configuration screen
GtkWidget* create_network_config_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // NETWORK_CONFIG_SCREEN_H
