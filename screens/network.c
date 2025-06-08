#include "network.h"

struct _NetworkScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *wifi_listbox;
    GtkWidget *ethernet_toggle;
    gboolean network_configured;
};

typedef struct {
    const gchar *ssid;
    gint strength;
    gboolean secured;
    gboolean connected;
} WifiNetwork;

static const WifiNetwork wifi_networks[] = {
    {"HomeNetwork_5G", 85, TRUE, FALSE},
    {"Office_WiFi", 70, TRUE, FALSE},
    {"Guest", 60, FALSE, FALSE},
    {"Neighbor_WiFi", 45, TRUE, FALSE},
    {NULL, 0, FALSE, FALSE}
};

static void network_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(NetworkScreen, network_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 network_screen_interface_init))

static void on_ethernet_toggled(GtkSwitch *switch_widget, gpointer user_data) {
    NetworkScreen *self = NETWORK_SCREEN(user_data);
    self->network_configured = gtk_switch_get_active(switch_widget);
}

static void on_wifi_selected(GtkListBox *listbox, GtkListBoxRow *row, NetworkScreen *self) {
    if (row) {
        // In a real implementation, this would show a password dialog for secured networks
        self->network_configured = TRUE;
    }
}

static GtkWidget *create_wifi_row(const WifiNetwork *wifi) {
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_start(row, 16);
    gtk_widget_set_margin_end(row, 16);
    gtk_widget_set_margin_top(row, 12);
    gtk_widget_set_margin_bottom(row, 12);
    
    // WiFi signal icon
    const gchar *signal_icon;
    if (wifi->strength >= 75) signal_icon = "network-wireless-signal-excellent";
    else if (wifi->strength >= 50) signal_icon = "network-wireless-signal-good";
    else if (wifi->strength >= 25) signal_icon = "network-wireless-signal-ok";
    else signal_icon = "network-wireless-signal-weak";
    
    GtkWidget *wifi_icon = gtk_image_new_from_icon_name(signal_icon);
    gtk_image_set_pixel_size(GTK_IMAGE(wifi_icon), 24);
    gtk_widget_add_css_class(wifi_icon, "wifi-icon");
    
    // Network info
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_hexpand(info_box, TRUE);
    
    GtkWidget *ssid_label = gtk_label_new(wifi->ssid);
    gtk_widget_set_halign(ssid_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(ssid_label, "wifi-ssid");
    
    // Network type (secured/open)
    GtkWidget *type_label = gtk_label_new(wifi->secured ? "Secured" : "Open");
    gtk_widget_set_halign(type_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(type_label, "wifi-type");
    
    gtk_box_append(GTK_BOX(info_box), ssid_label);
    gtk_box_append(GTK_BOX(info_box), type_label);
      // Security icon for secured networks
    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    if (wifi->secured) {
        GtkWidget *lock_icon = gtk_image_new_from_icon_name("network-wireless-encrypted");
        gtk_image_set_pixel_size(GTK_IMAGE(lock_icon), 16);
        gtk_widget_add_css_class(lock_icon, "security-icon");
        gtk_box_append(GTK_BOX(right_box), lock_icon);
    }
    
    gtk_box_append(GTK_BOX(row), wifi_icon);
    gtk_box_append(GTK_BOX(row), info_box);
    gtk_box_append(GTK_BOX(row), right_box);
    
    return row;
}

static GtkWidget *network_screen_create_widget(InstallerScreen *screen) {
    NetworkScreen *self = NETWORK_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
      self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    gtk_widget_set_margin_top(self->widget, 48);
    gtk_widget_set_margin_start(self->widget, 32);
    gtk_widget_set_margin_end(self->widget, 32);
      // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 32);
    gtk_widget_set_margin_top(title_box, 24);
    
    GtkWidget *title = gtk_label_new("Network Configuration");
    gtk_widget_add_css_class(title, "welcome-title");
      GtkWidget *subtitle = gtk_label_new("Connect to the internet to download updates during installation.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Network options container
    GtkWidget *network_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_halign(network_container, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(network_container, 600, -1);
    
    // Ethernet card
    GtkWidget *ethernet_card = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(ethernet_card, "network-card");
    
    GtkWidget *ethernet_icon = gtk_image_new_from_icon_name("network-wired");
    gtk_image_set_pixel_size(GTK_IMAGE(ethernet_icon), 32);
    
    GtkWidget *ethernet_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_hexpand(ethernet_info, TRUE);
    
    GtkWidget *ethernet_title = gtk_label_new("Wired Connection");
    gtk_widget_set_halign(ethernet_title, GTK_ALIGN_START);
    
    GtkWidget *ethernet_desc = gtk_label_new("Use ethernet cable for internet connection");
    gtk_widget_set_halign(ethernet_desc, GTK_ALIGN_START);
    gtk_widget_add_css_class(ethernet_desc, "text-muted");
    
    gtk_box_append(GTK_BOX(ethernet_info), ethernet_title);
    gtk_box_append(GTK_BOX(ethernet_info), ethernet_desc);
    
    self->ethernet_toggle = gtk_switch_new();
    gtk_widget_set_valign(self->ethernet_toggle, GTK_ALIGN_CENTER);
    g_signal_connect(self->ethernet_toggle, "notify::active", 
                    G_CALLBACK(on_ethernet_toggled), self);
    
    gtk_box_append(GTK_BOX(ethernet_card), ethernet_icon);
    gtk_box_append(GTK_BOX(ethernet_card), ethernet_info);
    gtk_box_append(GTK_BOX(ethernet_card), self->ethernet_toggle);
    
    // WiFi section
    GtkWidget *wifi_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    
    GtkWidget *wifi_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *wifi_icon = gtk_image_new_from_icon_name("network-wireless");
    gtk_image_set_pixel_size(GTK_IMAGE(wifi_icon), 24);
    GtkWidget *wifi_title = gtk_label_new("WiFi Networks");
    gtk_box_append(GTK_BOX(wifi_header), wifi_icon);
    gtk_box_append(GTK_BOX(wifi_header), wifi_title);
    
    GtkWidget *wifi_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wifi_scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(wifi_scrolled, -1, 250);
    gtk_widget_add_css_class(wifi_scrolled, "wifi-list");
    
    self->wifi_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(self->wifi_listbox), GTK_SELECTION_SINGLE);
    
    for (int i = 0; wifi_networks[i].ssid != NULL; i++) {
        GtkWidget *row_widget = create_wifi_row(&wifi_networks[i]);
        GtkWidget *list_row = gtk_list_box_row_new();
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(list_row), row_widget);
        gtk_list_box_append(GTK_LIST_BOX(self->wifi_listbox), list_row);
    }
    
    g_signal_connect(self->wifi_listbox, "row-selected", 
                    G_CALLBACK(on_wifi_selected), self);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(wifi_scrolled), self->wifi_listbox);
    
    gtk_box_append(GTK_BOX(wifi_section), wifi_header);
    gtk_box_append(GTK_BOX(wifi_section), wifi_scrolled);
      gtk_box_append(GTK_BOX(network_container), ethernet_card);
    gtk_box_append(GTK_BOX(network_container), wifi_section);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), network_container);
    
    return self->widget;
}

static gboolean network_screen_validate(InstallerScreen *screen) {
    // Network configuration is optional, always return TRUE
    return TRUE;
}

static void network_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = network_screen_create_widget;
    iface->validate = network_screen_validate;
}

static void network_screen_init(NetworkScreen *self) {
    self->widget = NULL;
    self->network_configured = FALSE;
}

static void network_screen_class_init(NetworkScreenClass *klass) {
    // Class initialization
}

NetworkScreen *network_screen_new(void) {
    return g_object_new(NETWORK_TYPE_SCREEN, NULL);
}
