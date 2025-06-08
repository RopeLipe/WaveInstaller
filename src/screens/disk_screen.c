#include "disk_screen.h"
#include "network_screen.h"  // To navigate to the next screen
#include "keyboard_screen.h" // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_network_screen(GtkButton *button, gpointer user_data);
extern void navigate_to_keyboard_screen(GtkButton *button, gpointer user_data);

// Placeholder function to simulate fetching disk information
// In a real application, this would involve system calls or libraries (e.g., udisks2, libblockdev)
static GList *get_available_disks() {
    GList *disks = NULL;
    // Example disks - replace with actual disk detection
    disks = g_list_append(disks, g_strdup("/dev/sda (500 GB SSD)"));
    disks = g_list_append(disks, g_strdup("/dev/sdb (1 TB HDD)"));
    disks = g_list_append(disks, g_strdup("/dev/nvme0n1 (256 GB NVMe)"));
    return disks;
}

GtkWidget *create_disk_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *disk_selection_label;
    GtkWidget *disk_combo_box; // Placeholder for disk list - GtkTreeView or GtkListBox would be better
    GtkWidget *partitioning_label;
    GtkWidget *partition_options_combo_box; // For "Use entire disk", "Manual partitioning"
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;

    // Main box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "disk-screen-box");

    title_label = gtk_label_new("Disk Setup");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_top(title_label, 20);
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    disk_selection_label = gtk_label_new("Select target disk for installation:");
    gtk_widget_set_halign(disk_selection_label, GTK_ALIGN_START);
    gtk_widget_set_margin_start(disk_selection_label, 50);
    gtk_widget_set_margin_end(disk_selection_label, 50);
    gtk_widget_set_margin_bottom(disk_selection_label, 5);
    gtk_box_append(GTK_BOX(box), disk_selection_label);

    // Disk selection dropdown (placeholder)
    disk_combo_box = gtk_combo_box_text_new();
    GList *disks = get_available_disks();
    for (GList *l = disks; l != NULL; l = l->next) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(disk_combo_box), (const char *)l->data);
    }
    g_list_free_full(disks, g_free);
    if (gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(disk_combo_box)) == NULL && g_list_length(disks) > 0) {
         gtk_combo_box_set_active(GTK_COMBO_BOX(disk_combo_box), 0);
    }
    gtk_widget_set_margin_start(disk_combo_box, 50);
    gtk_widget_set_margin_end(disk_combo_box, 50);
    gtk_widget_set_margin_bottom(disk_combo_box, 20);
    gtk_widget_add_css_class(disk_combo_box, "disk-combo");
    gtk_box_append(GTK_BOX(box), disk_combo_box);

    partitioning_label = gtk_label_new("Partitioning options:");
    gtk_widget_set_halign(partitioning_label, GTK_ALIGN_START);
    gtk_widget_set_margin_start(partitioning_label, 50);
    gtk_widget_set_margin_end(partitioning_label, 50);
    gtk_widget_set_margin_bottom(partitioning_label, 5);
    gtk_box_append(GTK_BOX(box), partitioning_label);

    // Partitioning options dropdown
    const char *part_options[] = {"Use entire disk (erase existing data)", "Manual partitioning (advanced)"};
    partition_options_combo_box = gtk_combo_box_text_new();
    for (int i = 0; i < G_N_ELEMENTS(part_options); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(partition_options_combo_box), part_options[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(partition_options_combo_box), 0);
    gtk_widget_set_margin_start(partition_options_combo_box, 50);
    gtk_widget_set_margin_end(partition_options_combo_box, 50);
    gtk_widget_set_margin_bottom(partition_options_combo_box, 30);
    gtk_widget_add_css_class(partition_options_combo_box, "partition-options-combo");
    gtk_box_append(GTK_BOX(box), partition_options_combo_box);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_keyboard_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_network_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);

    return box;
}
