#include <gtk/gtk.h>
#include "screens/welcome_screen.h"
#include "screens/language_screen.h"
#include "screens/timezone_screen.h"
#include "screens/keyboard_screen.h"
#include "screens/disk_selection_screen.h"
#include "screens/network_config_screen.h"
#include "screens/user_config_screen.h"
#include "screens/install_screen.h"
#include "utils/utils.h"

static GtkWidget *stack;
static GtkApplication *app;

static void on_next_screen(GtkWidget *widget, gpointer user_data) {
    const char *current_screen_name = gtk_stack_get_visible_child_name(GTK_STACK(stack));
    if (g_strcmp0(current_screen_name, "welcome") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "language");
    } else if (g_strcmp0(current_screen_name, "language") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "timezone");
    } else if (g_strcmp0(current_screen_name, "timezone") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "keyboard");
    } else if (g_strcmp0(current_screen_name, "keyboard") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "disk_selection");
    } else if (g_strcmp0(current_screen_name, "disk_selection") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "network_config");
    } else if (g_strcmp0(current_screen_name, "network_config") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "user_config");
    } else if (g_strcmp0(current_screen_name, "user_config") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "install");
    }
}

static void on_prev_screen(GtkWidget *widget, gpointer user_data) {
    const char *current_screen_name = gtk_stack_get_visible_child_name(GTK_STACK(stack));
    if (g_strcmp0(current_screen_name, "language") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "welcome");
    } else if (g_strcmp0(current_screen_name, "timezone") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "language");
    } else if (g_strcmp0(current_screen_name, "keyboard") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "timezone");
    } else if (g_strcmp0(current_screen_name, "disk_selection") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "keyboard");
    } else if (g_strcmp0(current_screen_name, "network_config") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "disk_selection");
    } else if (g_strcmp0(current_screen_name, "user_config") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "network_config");
    } else if (g_strcmp0(current_screen_name, "install") == 0) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "user_config");
    }
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *navigation_box;
    GtkWidget *prev_button;
    GtkWidget *next_button;

    // Create the main window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Wave Installer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // No title bar

    // Apply CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "/usr/bin/WaveInstaller/assets/css/style.css");
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(main_box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(main_box, GTK_ALIGN_CENTER);
    gtk_window_set_child(GTK_WINDOW(window), main_box);

    // Create the stack for screens
    stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_box_append(GTK_BOX(main_box), stack);

    // Create and add screens
    GtkWidget *welcome_screen = create_welcome_screen(on_next_screen);
    gtk_stack_add_named(GTK_STACK(stack), welcome_screen, "welcome");

    GtkWidget *language_screen = create_language_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), language_screen, "language");

    GtkWidget *timezone_screen = create_timezone_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), timezone_screen, "timezone");

    GtkWidget *keyboard_screen = create_keyboard_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), keyboard_screen, "keyboard");

    GtkWidget *disk_selection_screen = create_disk_selection_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), disk_selection_screen, "disk_selection");

    GtkWidget *network_config_screen = create_network_config_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), network_config_screen, "network_config");

    GtkWidget *user_config_screen = create_user_config_screen(on_next_screen, on_prev_screen);
    gtk_stack_add_named(GTK_STACK(stack), user_config_screen, "user_config");

    GtkWidget *install_screen = create_install_screen(app); // app is passed to allow closing
    gtk_stack_add_named(GTK_STACK(stack), install_screen, "install");

    // Navigation buttons (will be part of each screen's create function for better placement)
    // This is a simplified navigation for now.
    // Proper navigation will be integrated into each screen module.

    gtk_widget_set_visible(window, TRUE);
}

int main(int argc, char **argv) {
    app = gtk_application_new("com.example.waveinstaller", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
