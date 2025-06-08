#include "keyboard_screen.h"
#include "../utils/utils.h"

// Placeholder for actual keyboard layout data and logic
const char *keyboard_layouts[] = {"English (US)", "English (UK)", "Spanish", "French (AZERTY)", "German (QWERTZ)", NULL};

GtkWidget* create_keyboard_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("keyboard_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(Keyboard Icon)"));
    }

    GtkWidget *title = gtk_label_new("Select Keyboard Layout");
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    // Using a GtkListView for a more scrollable/searchable list if layouts are many
    // For simplicity with a few items, GtkDropDown is also fine.
    // Here, we'll use a GtkDropDown as it's simpler for this example.
    GtkWidget *layout_dropdown = gtk_drop_down_new_from_strings(keyboard_layouts);
    gtk_widget_set_halign(layout_dropdown, GTK_ALIGN_CENTER);
    // TODO: Add signal handler for layout selection
    gtk_box_append(GTK_BOX(box), layout_dropdown);

    // Test area
    GtkWidget *test_label = gtk_label_new("Test your keyboard layout here:");
    gtk_widget_set_halign(test_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(test_label, 20);
    gtk_box_append(GTK_BOX(box), test_label);

    GtkWidget *test_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(test_entry), "Type here...");
    gtk_widget_set_halign(test_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(test_entry, 300, -1); // Set a reasonable width
    gtk_box_append(GTK_BOX(box), test_entry);

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    return box;
}
