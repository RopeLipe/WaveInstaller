#include "disk_selection_screen.h"
#include "../utils/utils.h"

// Placeholder for disk detection and partitioning logic
// This would involve platform-specific code or libraries (e.g., libparted, udisks2)

// Example disk structure
typedef struct {
    const char *name;       // e.g., /dev/sda, /dev/nvme0n1
    const char *model;      // e.g., KINGSTON SA400S37
    guint64 size_gb;    // Size in GB
    const char *type;       // e.g., HDD, SSD, NVMe
} DiskInfo;

// Example list of detected disks
static DiskInfo available_disks[] = {
    {"/dev/sda", "KINGSTON SA400S37", 223, "SSD"},
    {"/dev/sdb", "WDC WD10EZEX-00BN5A0", 931, "HDD"},
    {"/dev/nvme0n1", "Samsung SSD 970 EVO Plus 500GB", 465, "NVMe"}
};
static int num_available_disks = sizeof(available_disks) / sizeof(available_disks[0]);

static GtkListItemFactory *factory = NULL;
static GListStore *disk_list_store = NULL;
static GtkWidget *selected_disk_label = NULL; // To show which disk is selected

static void setup_list_item_cb(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget *icon = create_image_from_file("drive_icon.png", 32, 32); // Placeholder icon
    GtkWidget *label_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);

    GtkWidget *name_label = gtk_label_new("");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    GtkWidget *details_label = gtk_label_new("");
    gtk_widget_set_halign(details_label, GTK_ALIGN_START);
    gtk_label_set_attributes(GTK_LABEL(details_label), pango_attr_list_new()); // To allow markup if needed
    gtk_widget_add_css_class(details_label, "caption-label"); // Smaller text for details

    if(icon) gtk_box_append(GTK_BOX(box), icon);
    else gtk_box_append(GTK_BOX(box), gtk_label_new("[icon]"));

    gtk_box_append(GTK_BOX(label_box), name_label);
    gtk_box_append(GTK_BOX(label_box), details_label);
    gtk_box_append(GTK_BOX(box), label_box);

    gtk_list_item_set_child(list_item, box);
}

static void bind_list_item_cb(GtkListItemFactory *factory, GtkListItem *list_item, gpointer user_data) {
    GtkWidget *box = gtk_list_item_get_child(list_item);
    GList *children = gtk_widget_get_first_child(box) ? gtk_widget_get_children(gtk_widget_get_first_child(gtk_widget_get_next_sibling(gtk_widget_get_first_child(box)))) : NULL;
    if (!children) return;

    GtkWidget *name_label = g_list_nth_data(children, 0);
    GtkWidget *details_label = g_list_nth_data(children, 1);
    g_list_free(children);

    DiskInfo *disk = g_list_model_get_item(G_LIST_MODEL(disk_list_store), gtk_list_item_get_position(list_item));

    if (disk && name_label && details_label) {
        gtk_label_set_text(GTK_LABEL(name_label), disk->name);
        char details[128];
        g_snprintf(details, sizeof(details), "%s - %lu GB (%s)", disk->model, disk->size_gb, disk->type);
        gtk_label_set_text(GTK_LABEL(details_label), details);
    }
}

static void on_disk_selected(GtkListView *list_view, GtkListItem *item, gpointer user_data) {
    if (item == NULL) return; // Item can be NULL if selection is cleared
    guint position = gtk_list_item_get_position(item);
    DiskInfo *disk = g_list_model_get_item(G_LIST_MODEL(disk_list_store), position);
    if (disk && selected_disk_label) {
        char label_text[256];
        g_snprintf(label_text, sizeof(label_text), "Selected: %s (%s, %lu GB)", disk->name, disk->model, disk->size_gb);
        gtk_label_set_text(GTK_LABEL(selected_disk_label), label_text);
    }
    // TODO: Store the selected disk for the installation process
}

GtkWidget* create_disk_selection_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_vexpand(main_box, TRUE);
    gtk_widget_set_hexpand(main_box, TRUE);
    gtk_widget_set_halign(main_box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(main_box, GTK_ALIGN_FILL);
    gtk_widget_add_css_class(main_box, "content-box");
    gtk_widget_set_margin_top(main_box, 10);
    gtk_widget_set_margin_bottom(main_box, 10);
    gtk_widget_set_margin_start(main_box, 20);
    gtk_widget_set_margin_end(main_box, 20);

    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    GtkWidget *icon = create_image_from_file("disk_icon.png", 48, 48);
    if (icon) gtk_box_append(GTK_BOX(title_box), icon);
    else gtk_box_append(GTK_BOX(title_box), gtk_label_new("(Disk Icon)"));

    GtkWidget *title = gtk_label_new("Select Installation Disk");
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(main_box), title_box);

    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scrolled_window, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_add_css_class(scrolled_window, "rounded-list"); // For CSS styling if needed

    disk_list_store = g_list_store_new(G_TYPE_POINTER); // Store pointers to DiskInfo (or GObjects)
    for (int i = 0; i < num_available_disks; ++i) {
        g_list_store_append(disk_list_store, &available_disks[i]);
    }

    factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_list_item_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_list_item_cb), NULL);

    GtkNoSelection *selection_model = gtk_single_selection_new(G_LIST_MODEL(disk_list_store));
    GtkWidget *list_view = gtk_list_view_new(GTK_SELECTION_MODEL(selection_model), factory);
    gtk_widget_add_css_class(list_view, "disk-list-view"); // For specific styling
    // g_signal_connect(selection_model, "selection-changed", G_CALLBACK(on_disk_selected_new), list_view);
    // For GtkListView, item activation or selection change is handled differently.
    // We can listen to "activate" on the list_view or selection_model's "selection-changed"
    // For simplicity, let's assume single click selection triggers an action or updates a label.
    // A more robust way is to use selection_model "selection-changed" signal.
    // For now, we will use a label to show the selected disk, updated via a button or other means.

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), list_view);
    gtk_box_append(GTK_BOX(main_box), scrolled_window);

    selected_disk_label = gtk_label_new("No disk selected.");
    gtk_widget_set_halign(selected_disk_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(selected_disk_label, 10);
    gtk_box_append(GTK_BOX(main_box), selected_disk_label);

    // TODO: Add controls for partitioning options (e.g., "Use entire disk", "Manual partitioning")
    // This would typically involve radio buttons or a GtkExpander for advanced options.
    GtkWidget *options_label = gtk_label_new("Partitioning options will appear here based on selection.");
    gtk_widget_set_halign(options_label, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(main_box), options_label);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(main_box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    // Connect selection changed signal
    g_signal_connect(selection_model, "notify::selected-item", G_CALLBACK(on_disk_selected), selected_disk_label);


    return main_box;
}
