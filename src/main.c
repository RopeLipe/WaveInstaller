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

// CSS content as a C string
const char *css_data =
"/* styles.css */\\n"
"\\n"
"/* General window styling */\\n"
"window {\\n"
"    background-color: #f0f0f0; /* Light grey background */\\n"
"    font-family: sans-serif;\\n"
"    color: #333333; /* Default text color for light theme */\\n"
"}\\n"
"\\n"
"/* Dark theme specific styling */\\n"
"window:dark {\\n"
"    background-color: #2c2c2c; /* Dark grey background */\\n"
"    color: #e0e0e0; /* Default text color for dark theme */\\n"
"}\\n"
"\\n"
"/* --- General Widget Styling --- */\\n"
"\\n"
"button {\\n"
"    border-radius: 8px; /* Rounded corners for buttons */\\n"
"    background-color: #0078d4; /* Blue accent */\\n"
"    color: white;\\n"
"    padding: 10px 18px;\\n"
"    font-size: 1em;\\n"
"    border: none;\\n"
"    min-width: 100px;\\n"
"    transition: background-color 0.2s ease-in-out;\\n"
"}\\n"
"\\n"
"button:hover {\\n"
"    background-color: #005a9e; /* Darker blue on hover */\\n"
"}\\n"
"\\n"
"button:disabled {\\n"
"    background-color: #777777;\\n"
"    color: #aaaaaa;\\n"
"}\\n"
"\\n"
"/* Specific button types */\\n"
"/* .next-button { */\\n"
"    /* Already covered by general button, can add specifics if needed */\\n"
"/* } */\\n"
"\\n"
".back-button {\\n"
"    background-color: #6c757d; /* A slightly different shade for back buttons */\\n"
"}\\n"
".back-button:hover {\\n"
"    background-color: #5a6268;\\n"
"}\\n"
"\\n"
"label {\\n"
"    font-size: 1em;\\n"
"}\\n"
"\\n"
"window:dark label {\\n"
"    color: #e0e0e0;\\n"
"}\\n"
"\\n"
".title-label {\\n"
"    font-size: 1.8em;\\n"
"    font-weight: bold;\\n"
"    margin-bottom: 15px;\\n"
"}\\n"
"\\n"
".message-label {\\n"
"    font-size: 1.1em;\\n"
"    margin-bottom: 20px;\\n"
"}\\n"
"\\n"
"entry, textview { /* Style text inputs and text views */\\n"
"    border-radius: 5px;\\n"
"    padding: 8px;\\n"
"    border: 1px solid #cccccc;\\n"
"    font-size: 1em;\\n"
"    background-color: #ffffff;\\n"
"    color: #333333;\\n"
"}\\n"
"entry:focus, textview:focus {\\n"
"    border-color: #0078d4; /* Blue border on focus */\\n"
"    box-shadow: 0 0 0 2px rgba(0, 120, 212, 0.3);\\n"
"}\\n"
"\\n"
"window:dark entry, window:dark textview {\\n"
"    background-color: #3a3a3a;\\n"
"    color: #e0e0e0;\\n"
"    border: 1px solid #555555;\\n"
"}\\n"
"window:dark entry:focus, window:dark textview:focus {\\n"
"    border-color: #0078d4;\\n"
"    box-shadow: 0 0 0 2px rgba(0, 120, 212, 0.4);\\n"
"}\\n"
"\\n"
"\\n"
"combobox text, combobox button, menubutton { /* Style combo boxes and dropdowns */\\n"
"    border-radius: 5px;\\n"
"    padding: 8px;\\n"
"    border: 1px solid #cccccc;\\n"
"    font-size: 1em;\\n"
"    background-color: #ffffff; /* Ensure consistent background */\\n"
"}\\n"
"combobox > box > button, menubutton > button { /* Target the button part specifically for consistent styling */\\n"
"    padding: 6px 10px; /* Adjust padding for the button inside combobox */\\n"
"    border-radius: 5px;\\n"
"}\\n"
"\\n"
"\\n"
"window:dark combobox text, window:dark combobox button, window:dark menubutton {\\n"
"    background-color: #3a3a3a;\\n"
"    color: #e0e0e0;\\n"
"    border: 1px solid #555555;\\n"
"}\\n"
"\\n"
"checkbutton > label { /* Style checkbox labels */\\n"
"    font-size: 1em;\\n"
"}\\n"
"\\n"
"progressbar {\\n"
"    border-radius: 5px;\\n"
"    min-height: 20px;\\n"
"}\\n"
"progressbar > trough {\\n"
"    background-color: #e0e0e0;\\n"
"    border-radius: 5px;\\n"
"}\\n"
"progressbar > trough > progress {\\n"
"    background-image: none; /* Remove default GTK styling */\\n"
"    background-color: #0078d4; /* Blue accent for progress */\\n"
"    border-radius: 5px;\\n"
"}\\n"
"window:dark progressbar > trough {\\n"
"    background-color: #505050;\\n"
"}\\n"
"\\n"
"\\n"
"/* --- Screen Specific Containers (mostly for padding/alignment if needed) --- */\\n"
".welcome-screen-box,\\n"
".language-screen-box,\\n"
".timezone-screen-box,\\n"
".keyboard-screen-box,\\n"
".disk-screen-box,\\n"
".network-screen-box,\\n"
".user-screen-box,\\n"
".install-screen-box {\\n"
"    padding: 20px;\\n"
"    /* These boxes are already centered by halign/valign in C code,\\n"
"       so specific styling here might be minimal unless unique spacing is needed. */\\n"
"}\\n"
"\\n"
"/* --- Widget Specific Styling --- */\\n"
"\\n"
".welcome-icon {\\n"
"    font-size: 3em; /* Placeholder styling for the ICON label */\\n"
"    color: #0078d4;\\n"
"    /* Replace with actual image styling when you have icons */\\n"
"    /* e.g., -gtk-icon-source: url(\\"assets/icons/welcome_icon.png\\"); */\\n"
"    /* min-width: 64px; */\\n"
"    /* min-height: 64px; */\\n"
"}\\n"
"\\n"
".language-combo,\\n"
".timezone-combo,\\n"
".keyboard-layout-combo,\\n"
".keyboard-variant-combo,\\n"
".disk-combo,\\n"
".partition-options-combo {\\n"
"    min-width: 250px; /* Ensure dropdowns have a decent width */\\n"
"}\\n"
"\\n"
".keyboard-test-entry {\\n"
"    min-width: 300px;\\n"
"}\\n"
"\\n"
".password-strength {\\n"
"    font-size: 0.9em;\\n"
"    margin-top: -5px; /* Adjust spacing a bit */\\n"
"    min-height: 1.5em; /* Reserve space to prevent layout jumps */\\n"
"}\\n"
".password-strength.weak {\\n"
"    color: #d9534f; /* Red for weak */\\n"
"}\\n"
".password-strength.medium {\\n"
"    color: #f0ad4e; /* Orange for medium */\\n"
"}\\n"
".password-strength.strong {\\n"
"    color: #5cb85c; /* Green for strong */\\n"
"}\\n"
"window:dark .password-strength.weak { color: #ff7f7f; }\\n"
"window:dark .password-strength.medium { color: #ffcc66; }\\n"
"window:dark .password-strength.strong { color: #88dd88; }\\n"
"\\n"
"\\n"
".password-match {\\n"
"    font-size: 0.9em;\\n"
"    margin-top: -5px;\\n"
"    min-height: 1.5em;\\n"
"}\\n"
".password-match.error {\\n"
"    color: #d9534f; /* Red for mismatch */\\n"
"}\\n"
"window:dark .password-match.error { color: #ff7f7f; }\\n"
;

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

    // Load CSS from string
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GError *error = NULL;

    gtk_css_provider_load_from_string(css_provider, css_data);

    // The following error check is for gtk_css_provider_load_from_file,
    // but gtk_css_provider_load_from_string itself doesn't directly return an error.
    // However, it's good practice to keep error handling for the provider itself.
    // If gtk_css_provider_load_from_string fails (e.g. due to parsing errors in the string),
    // it will print warnings to the console, but the application might still run.
    // For more robust error checking with load_from_string, one might need to
    // connect to GtkCssProvider::parsing-error signal if available and needed.

    // For simplicity, we'll assume the string is valid.
    // If there were an error object to check from load_from_string, it would be like this:
    /*
    if (error) {
        g_warning("Failed to load CSS from string: %s", error->message);
        g_error_free(error);
    } else {
    */
        gtk_style_context_add_provider_for_display(
            gdk_display_get_default(),
            GTK_STYLE_PROVIDER(css_provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
        g_message("CSS loaded from string successfully."); // Optional: success message
    /*
    }
    */
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
