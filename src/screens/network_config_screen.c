#include "network_config_screen.h"
#include "../utils/utils.h"

// Placeholder for network detection and configuration logic
// This would typically involve NetworkManager (via D-Bus) or similar system services

static void on_network_interface_selected(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    // TODO: Handle interface selection, show relevant config options (DHCP/Static)
}

static void on_connection_type_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data) {
    // TODO: Show/hide static IP fields based on DHCP/Static selection
    GtkWidget *static_ip_grid = GTK_WIDGET(user_data);
    guint selected = gtk_drop_down_get_selected(dropdown);
    gtk_widget_set_visible(static_ip_grid, selected == 1); // Assuming 0 is DHCP, 1 is Static
}

GtkWidget* create_network_config_screen(NextScreenCallback on_next, PrevScreenCallback on_prev) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_vexpand(box, TRUE);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "content-box");

    GtkWidget *icon = create_image_from_file("network_icon.png", 64, 64);
    if (icon) {
        gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
        gtk_box_append(GTK_BOX(box), icon);
    } else {
        gtk_box_append(GTK_BOX(box), gtk_label_new("(Network Icon)"));
    }

    GtkWidget *title = gtk_label_new("Network Configuration");
    gtk_widget_add_css_class(title, "welcome-subtitle");
    gtk_widget_set_halign(title, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(box), title);

    // Network interface selection (e.g., eth0, wlan0)
    // In a real app, this list would be populated dynamically.
    const char *interfaces[] = {"eth0 (Wired)", "wlan0 (Wireless)", "No Network", NULL};
    GtkWidget *iface_dropdown = gtk_drop_down_new_from_strings(interfaces);
    gtk_widget_set_halign(iface_dropdown, GTK_ALIGN_CENTER);
    // g_signal_connect(iface_dropdown, "notify::selected", G_CALLBACK(on_network_interface_selected), NULL);
    gtk_box_append(GTK_BOX(box), iface_dropdown);

    // Connection type (DHCP/Static)
    GtkWidget *connection_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(connection_grid), 10);
    gtk_widget_set_halign(connection_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(connection_grid, 10);
    gtk_box_append(GTK_BOX(box), connection_grid);

    gtk_grid_attach(GTK_GRID(connection_grid), gtk_label_new("Configure IPv4:"), 0, 0, 1, 1);
    const char *conn_types[] = {"Automatic (DHCP)", "Manual (Static IP)", NULL};
    GtkWidget *conn_type_dropdown = gtk_drop_down_new_from_strings(conn_types);
    gtk_grid_attach(GTK_GRID(connection_grid), conn_type_dropdown, 1, 0, 1, 1);

    // Static IP Configuration Fields (initially hidden)
    GtkWidget *static_ip_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(static_ip_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(static_ip_grid), 5);
    gtk_widget_set_halign(static_ip_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(static_ip_grid, 10);
    gtk_widget_set_visible(static_ip_grid, FALSE);
    gtk_box_append(GTK_BOX(box), static_ip_grid);

    g_signal_connect(conn_type_dropdown, "notify::selected", G_CALLBACK(on_connection_type_changed), static_ip_grid);

    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_label_new("IP Address:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_entry_new(), 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_label_new("Netmask:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_entry_new(), 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_label_new("Gateway:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_entry_new(), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_label_new("DNS Servers:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gtk_entry_new(), 1, 3, 1, 1); // (comma-separated)

    GtkWidget *spacer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_vexpand(spacer, TRUE);
    gtk_box_append(GTK_BOX(box), spacer);

    GtkWidget *nav_box = create_navigation_box(on_next, on_prev, NULL, NULL, "Next");
    gtk_box_append(GTK_BOX(box), nav_box);
    gtk_widget_set_vexpand(nav_box, FALSE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);

    return box;
}
