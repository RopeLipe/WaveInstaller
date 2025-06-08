#include "language_screen.h"
#include "../utils/utils.h"

static void on_language_selected(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    // TODO: Store selected language
    // guint selected_index = gtk_drop_down_get_selected(dropdown);
    // const char *selected_lang_code = gtk_string_list_get_string(GTK_STRING_LIST(gtk_drop_down_get_model(dropdown)), selected_index);
    // g_print("Selected language: %s\n", selected_lang_code);
}

GtkWidget* create_language_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("language_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(Language Icon)"));
    }

    GtkWidget *title = gtk_label_new("Select Your Language");
    gtk_widget_add_css_class(title, "welcome-subtitle"); // Re-use for consistent subtitle styling
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    // Example languages (replace with actual list)
    const char *languages[] = {"English (US)", "Español", "Français", "Deutsch", NULL};
    GtkWidget *language_dropdown = gtk_drop_down_new_from_strings(languages);
    gtk_widget_set_halign(language_dropdown, GTK_ALIGN_CENTER);
    // g_signal_connect(language_dropdown, "notify::selected", G_CALLBACK(on_language_selected), NULL);
    gtk_box_append(GTK_BOX(box), language_dropdown);

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    return box;
}
