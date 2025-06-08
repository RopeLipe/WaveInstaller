#include <gtk/gtk.h>
#include "welcome_screen.h"
#include "language_screen.h"
#include "timezone_screen.h"
#include "keyboard_screen.h"
#include "disk_screen.h"
#include "network_screen.h"
#include "user_screen.h"
#include "install_screen.h"

static GtkWidget *main_window;
static GtkWidget *main_stack;

// Function to switch screens
static void switch_screen(GtkWidget *new_screen) {
    GtkWidget *current_screen = gtk_stack_get_visible_child(GTK_STACK(main_stack));
    if (current_screen) {
        gtk_stack_remove(GTK_STACK(main_stack), current_screen);
    }
    gtk_stack_add_named(GTK_STACK(main_stack), new_screen, "current_screen");
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "current_screen");
}

// Placeholder navigation functions (to be implemented in each screen's C file)
void navigate_to_language_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_language_screen());
}

void navigate_to_timezone_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_timezone_screen());
}

void navigate_to_welcome_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_welcome_screen());
}

void navigate_to_keyboard_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_keyboard_screen());
}

void navigate_to_disk_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_disk_screen());
}

void navigate_to_network_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_network_screen());
}

void navigate_to_user_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_user_screen());
}

void navigate_to_install_screen(GtkButton *button, gpointer user_data) {
    switch_screen(create_install_screen());
}

// Add other navigation functions similarly...

static void activate(GtkApplication *app, gpointer user_data) {
    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Wave Installer");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
    gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE); // No title bar

    // Create the stack to hold different screens
    main_stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(main_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_window_set_child(GTK_WINDOW(main_window), main_stack);

    // Load CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GFile *css_file = g_file_new_for_path("css/styles.css"); // Create a GFile object for the CSS path
    GError *error = NULL;

    gtk_css_provider_load_from_file(css_provider, css_file, &error); // Load CSS using GFile
    g_object_unref(css_file); // Release the GFile object

    if (error) {
        g_warning("Failed to load CSS file 'css/styles.css': %s", error->message);
        g_error_free(error);
    } else {
        gtk_style_context_add_provider_for_display(
            gdk_display_get_default(),
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
        g_message("CSS file 'css/styles.css' loaded successfully."); // Optional: success message
    }
    g_object_unref(css_provider);


    // Start with the welcome screen
    switch_screen(create_welcome_screen());

    gtk_widget_show(main_window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.wave.waveinstaller", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
