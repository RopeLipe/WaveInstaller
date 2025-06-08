#include "keyboard_screen.h"
#include "disk_screen.h"     // To navigate to the next screen
#include "timezone_screen.h" // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_disk_screen(GtkButton *button, gpointer user_data);
extern void navigate_to_timezone_screen(GtkButton *button, gpointer user_data);

GtkWidget *create_keyboard_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *layout_combo_box; // Using GtkComboBoxText for keyboard layouts
    GtkWidget *variant_combo_box; // Optional: for layout variants (e.g., Dvorak, Colemak for US layout)
    GtkWidget *test_entry; // A text entry to test the keyboard layout
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;

    // Main box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "keyboard-screen-box");

    title_label = gtk_label_new("Select Keyboard Layout");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_top(title_label, 20);
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    // Keyboard layout selection
    // In a real installer, this list would be populated from system data (e.g., XKB)
    const char *layouts[] = {"US English", "UK English", "German", "French", "Spanish"};
    layout_combo_box = gtk_combo_box_text_new();
    for (int i = 0; i < G_N_ELEMENTS(layouts); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(layout_combo_box), layouts[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(layout_combo_box), 0); // Default to US English
    gtk_widget_set_margin_start(layout_combo_box, 50);
    gtk_widget_set_margin_end(layout_combo_box, 50);
    gtk_widget_set_margin_bottom(layout_combo_box, 10);
    gtk_widget_add_css_class(layout_combo_box, "keyboard-layout-combo");
    gtk_box_append(GTK_BOX(box), layout_combo_box);

    // Optional: Keyboard variant selection (can be hidden or shown based on selected layout)
    // For simplicity, let's add a placeholder. A real implementation would be more dynamic.
    variant_combo_box = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(variant_combo_box), "Default");
    // Add other variants as needed, e.g., gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(variant_combo_box), "Dvorak");
    gtk_combo_box_set_active(GTK_COMBO_BOX(variant_combo_box), 0);
    gtk_widget_set_margin_start(variant_combo_box, 50);
    gtk_widget_set_margin_end(variant_combo_box, 50);
    gtk_widget_set_margin_bottom(variant_combo_box, 20);
    gtk_widget_add_css_class(variant_combo_box, "keyboard-variant-combo");
    gtk_box_append(GTK_BOX(box), variant_combo_box);

    // Text entry to test keyboard layout
    test_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(test_entry), "Type here to test your keyboard layout...");
    gtk_widget_set_margin_start(test_entry, 50);
    gtk_widget_set_margin_end(test_entry, 50);
    gtk_widget_set_margin_bottom(test_entry, 30);
    gtk_widget_add_css_class(test_entry, "keyboard-test-entry");
    gtk_box_append(GTK_BOX(box), test_entry);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_timezone_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_disk_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);

    return box;
}
