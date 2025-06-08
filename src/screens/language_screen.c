#include "language_screen.h"
#include "timezone_screen.h" // To navigate to the next screen
#include "welcome_screen.h"  // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions (assuming they are in main.c or a shared utility)
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_timezone_screen(GtkButton *button, gpointer user_data);
// We'll also need a way to go back to the welcome screen
extern void navigate_to_welcome_screen(GtkButton *button, gpointer user_data);


GtkWidget *create_language_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *language_combo_box; // Using GtkComboBoxText for simplicity, GtkDropDown is an option for more modern feel
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;

    // Main box container for the screen
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "language-screen-box");

    title_label = gtk_label_new("Select Your Language");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_top(title_label, 20);
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    // Language selection dropdown
    // For a real application, you'd populate this from a list of supported languages
    const char *languages[] = {"English", "Español", "Français", "Deutsch", "日本語"};
    language_combo_box = gtk_combo_box_text_new();
    for (int i = 0; i < G_N_ELEMENTS(languages); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(language_combo_box), languages[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(language_combo_box), 0); // Default to English
    gtk_widget_set_margin_start(language_combo_box, 50);
    gtk_widget_set_margin_end(language_combo_box, 50);
    gtk_widget_set_margin_bottom(language_combo_box, 30);
    gtk_widget_add_css_class(language_combo_box, "language-combo");
    gtk_box_append(GTK_BOX(box), language_combo_box);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10); // 10px spacing
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_welcome_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_timezone_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);

    return box;
}
