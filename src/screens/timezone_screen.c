#include "timezone_screen.h"
#include "../utils/utils.h"

// Placeholder for actual timezone data and logic
// In a real app, you'd populate this from system data or a library
const char *regions[] = {"Africa", "America", "Antarctica", "Asia", "Atlantic", "Australia", "Europe", "Indian", "Pacific", NULL};
const char *america_cities[] = {"New York", "Los Angeles", "Chicago", "Denver", "Anchorage", "Phoenix", "Boise", "Detroit", "Honolulu", "Indianapolis", "Juneau", "Louisville", "Menominee", "Mexico City", "Toronto", "Vancouver", NULL }; // Example

static GtkWidget *city_dropdown;

static void on_region_selected(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    guint selected_index = gtk_drop_down_get_selected(dropdown);
    // For simplicity, we only update for "America". A real app needs a full mapping.
    if (selected_index == 1) { // Assuming "America" is at index 1
        gtk_drop_down_set_model(GTK_DROP_DOWN(city_dropdown), G_LIST_MODEL(gtk_string_list_new(america_cities)));
    } else {
        const char* empty_cities[] = {"Select a region first", NULL};
        gtk_drop_down_set_model(GTK_DROP_DOWN(city_dropdown), G_LIST_MODEL(gtk_string_list_new(empty_cities)));
    }
    gtk_drop_down_set_selected(GTK_DROP_DOWN(city_dropdown), 0);
}

GtkWidget* create_timezone_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("timezone_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(Timezone Icon)"));
    }

    GtkWidget *title = gtk_label_new("Select Your Timezone");
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), grid);

    GtkWidget *region_label = gtk_label_new("Region:");
    gtk_widget_set_halign(region_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), region_label, 0, 0, 1, 1);

    GtkWidget *region_dropdown = gtk_drop_down_new_from_strings(regions);
    g_signal_connect(region_dropdown, "notify::selected", G_CALLBACK(on_region_selected), NULL);
    gtk_grid_attach(GTK_GRID(grid), region_dropdown, 1, 0, 1, 1);

    GtkWidget *city_label = gtk_label_new("City/Area:");
    gtk_widget_set_halign(city_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(grid), city_label, 0, 1, 1, 1);

    const char* initial_cities[] = {"Select a region first", NULL};
    city_dropdown = gtk_drop_down_new_from_strings(initial_cities);
    gtk_grid_attach(GTK_GRID(grid), city_dropdown, 1, 1, 1, 1);
    // TODO: Add signal handler for city selection if needed

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    return box;
}
