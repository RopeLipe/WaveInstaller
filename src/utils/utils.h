#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>

// Callback function types for navigation
typedef void (*NextScreenCallback)(GtkWidget*, gpointer);
typedef void (*PrevScreenCallback)(GtkWidget*, gpointer);

// Function to create a standard navigation box with Next and Previous buttons
GtkWidget* create_navigation_box(NextScreenCallback next_callback, PrevScreenCallback prev_callback, gpointer user_data_next, gpointer user_data_prev, const char* next_label);

// Function to load an image and create a GtkImage widget
GtkWidget* create_image_from_file(const char* filename, int width, int height);

// Placeholder for other utility functions, e.g.:
// - System theme detection (though GTK4 CSS handles prefers-color-scheme)
// - Functions to create styled labels or buttons if needed frequently

#endif // UTILS_H
