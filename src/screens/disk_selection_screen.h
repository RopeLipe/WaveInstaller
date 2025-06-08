#ifndef DISK_SELECTION_SCREEN_H
#define DISK_SELECTION_SCREEN_H

#include <gtk/gtk.h>
#include "../utils/utils.h" // For Navigation Callbacks

// Function to create the disk selection screen
GtkWidget* create_disk_selection_screen(NextScreenCallback on_next, PrevScreenCallback on_prev);

#endif // DISK_SELECTION_SCREEN_H
