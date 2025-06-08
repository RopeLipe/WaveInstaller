#include "user_screen.h"
#include "install_screen.h"  // To navigate to the next screen
#include "network_screen.h"  // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_install_screen(GtkButton *button, gpointer user_data);
extern void navigate_to_network_screen(GtkButton *button, gpointer user_data);

// Callback to check password strength (very basic placeholder)
static void check_password_strength(GtkEditable *editable, gpointer user_data) {
    GtkWidget *strength_label = GTK_WIDGET(user_data);
    const char *password = gtk_editable_get_text(editable);
    if (strlen(password) == 0) {
        gtk_label_set_text(GTK_LABEL(strength_label), "");
    } else if (strlen(password) < 6) {
        gtk_label_set_text(GTK_LABEL(strength_label), "Password is too short");
        gtk_widget_remove_css_class(strength_label, "medium");
        gtk_widget_remove_css_class(strength_label, "strong");
        gtk_widget_add_css_class(strength_label, "weak");
    } else if (strlen(password) < 10) {
        gtk_label_set_text(GTK_LABEL(strength_label), "Password strength: Medium");
        gtk_widget_remove_css_class(strength_label, "weak");
        gtk_widget_remove_css_class(strength_label, "strong");
        gtk_widget_add_css_class(strength_label, "medium");
    } else {
        gtk_label_set_text(GTK_LABEL(strength_label), "Password strength: Strong");
        gtk_widget_remove_css_class(strength_label, "weak");
        gtk_widget_remove_css_class(strength_label, "medium");
        gtk_widget_add_css_class(strength_label, "strong");
    }
}

// Callback to check if passwords match
static void check_passwords_match(GtkEditable *editable, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data); // We'll pass the GtkBuilder
    GtkWidget *password_entry = GTK_WIDGET(gtk_builder_get_object(builder, "password_entry"));
    GtkWidget *confirm_password_entry = GTK_WIDGET(gtk_builder_get_object(builder, "confirm_password_entry"));
    GtkWidget *match_label = GTK_WIDGET(gtk_builder_get_object(builder, "password_match_label"));

    const char *pass1 = gtk_editable_get_text(GTK_EDITABLE(password_entry));
    const char *pass2 = gtk_editable_get_text(GTK_EDITABLE(confirm_password_entry));

    if (strlen(pass2) > 0 && strcmp(pass1, pass2) != 0) {
        gtk_label_set_text(GTK_LABEL(match_label), "Passwords do not match");
        gtk_widget_add_css_class(match_label, "error");
    } else {
        gtk_label_set_text(GTK_LABEL(match_label), "");
        gtk_widget_remove_css_class(match_label, "error");
    }
}

GtkWidget *create_user_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *grid;
    GtkWidget *full_name_label, *full_name_entry;
    GtkWidget *username_label, *username_entry;
    GtkWidget *password_label, *password_entry;
    GtkWidget *confirm_password_label, *confirm_password_entry;
    GtkWidget *password_strength_label;
    GtkWidget *password_match_label;
    GtkWidget *hostname_label, *hostname_entry;
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;
    GtkBuilder *builder; // Using GtkBuilder to easily get widgets by ID

    // Using GtkBuilder to make widget retrieval easier for callbacks
    builder = gtk_builder_new();
    // In a real scenario, you might load this from a UI file, but here we build programmatically
    // and then add widgets to the builder with names.

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_START);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_add_css_class(box, "user-screen-box");

    title_label = gtk_label_new("Create Your User Account");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_widget_set_margin_start(grid, 50);
    gtk_widget_set_margin_end(grid, 50);
    gtk_widget_set_margin_bottom(grid, 20);
    gtk_box_append(GTK_BOX(box), grid);

    full_name_label = gtk_label_new("Full Name:");
    gtk_widget_set_halign(full_name_label, GTK_ALIGN_START);
    full_name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(full_name_entry), "e.g., John Doe");
    gtk_grid_attach(GTK_GRID(grid), full_name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), full_name_entry, 1, 0, 1, 1);

    username_label = gtk_label_new("Username:");
    gtk_widget_set_halign(username_label, GTK_ALIGN_START);
    username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "e.g., jdoe (no spaces, lowercase)");
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 1, 1, 1);

    password_label = gtk_label_new("Password:");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Mask password
    gtk_entry_set_input_purpose(GTK_ENTRY(password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_builder_add_named(builder, "password_entry", G_OBJECT(password_entry)); // Add to builder, use G_OBJECT
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);

    password_strength_label = gtk_label_new("");
    gtk_widget_set_halign(password_strength_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(password_strength_label, "password-strength");
    gtk_grid_attach(GTK_GRID(grid), password_strength_label, 1, 3, 1, 1); // Below password entry
    g_signal_connect(password_entry, "changed", G_CALLBACK(check_password_strength), password_strength_label);

    confirm_password_label = gtk_label_new("Confirm Password:");
    gtk_widget_set_halign(confirm_password_label, GTK_ALIGN_START);
    confirm_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(confirm_password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_builder_add_named(builder, "confirm_password_entry", G_OBJECT(confirm_password_entry)); // Add to builder, use G_OBJECT
    gtk_grid_attach(GTK_GRID(grid), confirm_password_label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), confirm_password_entry, 1, 4, 1, 1);

    password_match_label = gtk_label_new("");
    gtk_widget_set_halign(password_match_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(password_match_label, "password-match");
    gtk_builder_add_named(builder, "password_match_label", G_OBJECT(password_match_label)); // Add to builder, use G_OBJECT
    gtk_grid_attach(GTK_GRID(grid), password_match_label, 1, 5, 1, 1); // Below confirm password
    g_signal_connect(password_entry, "changed", G_CALLBACK(check_passwords_match), builder);
    g_signal_connect(confirm_password_entry, "changed", G_CALLBACK(check_passwords_match), builder);

    hostname_label = gtk_label_new("Computer Name (Hostname):");
    gtk_widget_set_halign(hostname_label, GTK_ALIGN_START);
    hostname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(hostname_entry), "e.g., my-computer");
    gtk_grid_attach(GTK_GRID(grid), hostname_label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), hostname_entry, 1, 6, 1, 1);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(buttons_box, 20);
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_network_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_install_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);
    
    g_object_unref(builder); // Release the builder

    return box;
}
