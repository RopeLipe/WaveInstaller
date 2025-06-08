#include "timezone_screen.h"
#include "keyboard_screen.h" // To navigate to the next screen
#include "language_screen.h" // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_keyboard_screen(GtkButton *button, gpointer user_data);
extern void navigate_to_language_screen(GtkButton *button, gpointer user_data);

GtkWidget *create_timezone_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *timezone_combo_box; // Using GtkComboBoxText for now
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;

    // Main box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "timezone-screen-box");

    title_label = gtk_label_new("Select Your Timezone");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_top(title_label, 20);
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    // Timezone selection dropdown
    // In a real app, this would be a more comprehensive list, possibly grouped by region
    // or using a map-based selector.
    const char *timezones[] = {
        "America/New_York (UTC-4)",
        "Europe/London (UTC+1)",
        "Asia/Tokyo (UTC+9)",
        "Australia/Sydney (UTC+10)",
        "America/Los_Angeles (UTC-7)"
    };
    timezone_combo_box = gtk_combo_box_text_new();
    for (int i = 0; i < G_N_ELEMENTS(timezones); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(timezone_combo_box), timezones[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(timezone_combo_box), 0); // Default to first timezone
    gtk_widget_set_margin_start(timezone_combo_box, 50);
    gtk_widget_set_margin_end(timezone_combo_box, 50);
    gtk_widget_set_margin_bottom(timezone_combo_box, 30);
    gtk_widget_add_css_class(timezone_combo_box, "timezone-combo");
    gtk_box_append(GTK_BOX(box), timezone_combo_box);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_language_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_keyboard_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);

    return box;
}
