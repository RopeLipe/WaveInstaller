#include "user_config_screen.h"
#include "../utils/utils.h"

// TODO: Add validation for fields (e.g., password strength, username format)

GtkWidget* create_user_config_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("user_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(User Icon)"));
    }

    GtkWidget *title = gtk_label_new("Create Your User Account");
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), grid);

    // Full Name
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Your full name:"), 0, 0, 1, 1);
    GtkWidget *fullname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(fullname_entry), "E.g., Ada Lovelace");
    gtk_grid_attach(GTK_GRID(grid), fullname_entry, 1, 0, 1, 1);

    // Username
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Username:"), 0, 1, 1, 1);
    GtkWidget *username_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(username_entry), "E.g., ada (no spaces, lowercase)");
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 1, 1, 1);

    // Password
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Password:"), 0, 2, 1, 1);
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Mask password
    gtk_entry_set_input_purpose(GTK_ENTRY(password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Choose a strong password");
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 2, 1, 1);

    // Confirm Password
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Confirm password:"), 0, 3, 1, 1);
    GtkWidget *confirm_password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(confirm_password_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(confirm_password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_entry_set_placeholder_text(GTK_ENTRY(confirm_password_entry), "Retype your password");
    gtk_grid_attach(GTK_GRID(grid), confirm_password_entry, 1, 3, 1, 1);

    // Hostname
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Computer name (hostname):"), 0, 4, 1, 1);
    GtkWidget *hostname_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(hostname_entry), "E.g., my-computer");
    gtk_grid_attach(GTK_GRID(grid), hostname_entry, 1, 4, 1, 1);

    // Checkbox for automatic login (optional)
    GtkWidget *autologin_check = gtk_check_button_new_with_label("Log in automatically");
    gtk_grid_attach(GTK_GRID(grid), autologin_check, 0, 5, 2, 1); // Span 2 columns

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    return box;
}
