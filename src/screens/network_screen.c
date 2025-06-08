#include "network_screen.h"
#include "user_screen.h"   // To navigate to the next screen
#include "disk_screen.h"     // To navigate back
#include <gtk/gtk.h>

// Forward declarations for navigation functions
extern void switch_screen(GtkWidget *new_screen);
extern void navigate_to_user_screen(GtkButton *button, gpointer user_data);
extern void navigate_to_disk_screen(GtkButton *button, gpointer user_data);

// Callback to toggle visibility of static IP fields based on DHCP selection
static void dhcp_toggled(GtkCheckButton *check_button, gpointer user_data) {
    GtkWidget *static_ip_grid = GTK_WIDGET(user_data);
    gboolean is_dhcp = gtk_check_button_get_active(check_button);
    gtk_widget_set_visible(static_ip_grid, !is_dhcp);
}

GtkWidget *create_network_screen(void) {
    GtkWidget *box;
    GtkWidget *title_label;
    GtkWidget *interface_label; // Placeholder for interface selection
    GtkWidget *interface_combo_box; // Placeholder
    GtkWidget *dhcp_check_button;
    GtkWidget *static_ip_grid;
    GtkWidget *ip_address_label, *ip_address_entry;
    GtkWidget *subnet_mask_label, *subnet_mask_entry;
    GtkWidget *gateway_label, *gateway_entry;
    GtkWidget *dns_label, *dns_entry;
    GtkWidget *buttons_box;
    GtkWidget *back_button;
    GtkWidget *next_button;

    // Main box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // Align to top, not center, to prevent large empty space if static fields are hidden
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_START); 
    gtk_widget_set_margin_top(box, 30); // Add some top margin
    gtk_widget_add_css_class(box, "network-screen-box");

    title_label = gtk_label_new("Network Configuration");
    gtk_widget_add_css_class(title_label, "title-label");
    gtk_widget_set_margin_bottom(title_label, 20);
    gtk_box_append(GTK_BOX(box), title_label);

    // Placeholder for network interface selection
    interface_label = gtk_label_new("Network Interface:");
    gtk_widget_set_halign(interface_label, GTK_ALIGN_START);
    gtk_widget_set_margin_start(interface_label, 50);
    gtk_widget_set_margin_end(interface_label, 50);
    gtk_box_append(GTK_BOX(box), interface_label);

    interface_combo_box = gtk_combo_box_text_new();
    // In a real app, populate with detected interfaces (e.g., eth0, wlan0)
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface_combo_box), "eth0 (Wired)"); 
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interface_combo_box), "wlan0 (Wireless)");
    gtk_combo_box_set_active(GTK_COMBO_BOX(interface_combo_box), 0);
    gtk_widget_set_margin_start(interface_combo_box, 50);
    gtk_widget_set_margin_end(interface_combo_box, 50);
    gtk_widget_set_margin_bottom(interface_combo_box, 20);
    gtk_box_append(GTK_BOX(box), interface_combo_box);

    // DHCP checkbox
    dhcp_check_button = gtk_check_button_new_with_label("Configure automatically (DHCP)");
    gtk_check_button_set_active(GTK_CHECK_BUTTON(dhcp_check_button), TRUE); // Default to DHCP
    gtk_widget_set_margin_start(dhcp_check_button, 50);
    gtk_widget_set_margin_end(dhcp_check_button, 50);
    gtk_widget_set_margin_bottom(dhcp_check_button, 10);
    gtk_box_append(GTK_BOX(box), dhcp_check_button);

    // Grid for static IP configuration (initially hidden if DHCP is active)
    static_ip_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(static_ip_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(static_ip_grid), 5);
    gtk_widget_set_margin_start(static_ip_grid, 50);
    gtk_widget_set_margin_end(static_ip_grid, 50);
    gtk_widget_set_margin_bottom(static_ip_grid, 20);
    gtk_box_append(GTK_BOX(box), static_ip_grid);

    ip_address_label = gtk_label_new("IP Address:");
    gtk_widget_set_halign(ip_address_label, GTK_ALIGN_START);
    ip_address_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(static_ip_grid), ip_address_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), ip_address_entry, 1, 0, 1, 1);

    subnet_mask_label = gtk_label_new("Subnet Mask:");
    gtk_widget_set_halign(subnet_mask_label, GTK_ALIGN_START);
    subnet_mask_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(static_ip_grid), subnet_mask_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), subnet_mask_entry, 1, 1, 1, 1);

    gateway_label = gtk_label_new("Gateway:");
    gtk_widget_set_halign(gateway_label, GTK_ALIGN_START);
    gateway_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(static_ip_grid), gateway_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), gateway_entry, 1, 2, 1, 1);

    dns_label = gtk_label_new("DNS Server(s):");
    gtk_widget_set_halign(dns_label, GTK_ALIGN_START);
    dns_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(dns_entry), "e.g., 8.8.8.8, 1.1.1.1");
    gtk_grid_attach(GTK_GRID(static_ip_grid), dns_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(static_ip_grid), dns_entry, 1, 3, 1, 1);

    // Connect checkbox toggle to show/hide static IP grid
    g_signal_connect(dhcp_check_button, "toggled", G_CALLBACK(dhcp_toggled), static_ip_grid);
    // Initial visibility based on DHCP state
    dhcp_toggled(GTK_CHECK_BUTTON(dhcp_check_button), static_ip_grid);

    // Box for Back and Next buttons
    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_halign(buttons_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(buttons_box, 20); // Add some space above buttons
    gtk_widget_set_margin_bottom(buttons_box, 20);

    back_button = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_button, "back-button");
    g_signal_connect(back_button, "clicked", G_CALLBACK(navigate_to_disk_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), back_button);

    next_button = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_button, "next-button");
    g_signal_connect(next_button, "clicked", G_CALLBACK(navigate_to_user_screen), NULL);
    gtk_box_append(GTK_BOX(buttons_box), next_button);

    gtk_box_append(GTK_BOX(box), buttons_box);

    return box;
}
