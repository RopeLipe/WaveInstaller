/* network-screen.c - Network configuration screen implementation */
#include "network-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *ssid;
    const char *security;
    int strength;
} WifiNetwork;

static WifiNetwork sample_networks[] = {
    {"MyHomeWiFi", "WPA2", 85},
    {"Office-Network", "WPA3", 70},
    {"Guest-WiFi", "Open", 60},
    {"Neighbor-WiFi", "WPA2", 45},
    {NULL, NULL, 0}
};

static gboolean use_wifi = FALSE;
static char *selected_network = NULL;
static char *wifi_password = NULL;

static void
on_connection_type_changed(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *type = (const char *)g_object_get_data(G_OBJECT(button), "connection-type");
        use_wifi = (strcmp(type, "wifi") == 0);
        
        /* Show/hide WiFi list */
        GtkWidget *wifi_section = (GtkWidget *)g_object_get_data(G_OBJECT(button), "wifi-section");
        if (wifi_section) {
            gtk_widget_set_visible(wifi_section, use_wifi);
        }
    }
}

static void
on_network_selected(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *ssid = (const char *)g_object_get_data(G_OBJECT(button), "ssid");
        if (selected_network) g_free(selected_network);
        selected_network = g_strdup(ssid);
    }
}

static void
on_password_changed(GtkEditable *editable, gpointer user_data) {
    const char *text = gtk_editable_get_text(editable);
    if (wifi_password) g_free(wifi_password);
    wifi_password = g_strdup(text);
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    /* For WiFi, check if network is selected and password is provided if needed */
    if (use_wifi && selected_network) {
        /* Check if password is needed (non-open networks) */
        gboolean needs_password = TRUE;
        for (int i = 0; sample_networks[i].ssid != NULL; i++) {
            if (strcmp(sample_networks[i].ssid, selected_network) == 0) {
                needs_password = (strcmp(sample_networks[i].security, "Open") != 0);
                break;
            }
        }
        
        if (needs_password && (!wifi_password || strlen(wifi_password) == 0)) {
            return; /* Password required but not provided */
        }
    }
    
    wave_installer_application_next_screen(app);
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

static void
on_skip_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_next_screen(app);
}

void
network_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *connection_box, *connection_label;
    GtkWidget *ethernet_btn, *wifi_btn, *wifi_section, *wifi_scrolled, *wifi_list;
    GtkWidget *password_box, *password_label, *password_entry, *nav_box, *back_btn, *next_btn, *skip_btn;
    GSList *connection_group = NULL, *network_group = NULL;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Network Configuration");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Connect to the internet to download updates during installation");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Connection type selection */
    connection_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_box_append(GTK_BOX(content), connection_box);
    
    connection_label = gtk_label_new("Connection Type");
    gtk_widget_add_css_class(connection_label, "section-title");
    gtk_widget_set_halign(connection_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(connection_box), connection_label);
    
    /* Ethernet option */
    ethernet_btn = gtk_toggle_button_new_with_label("🌐 Wired Connection (Ethernet)");
    gtk_widget_add_css_class(ethernet_btn, "option-item");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ethernet_btn), TRUE);
    g_object_set_data(G_OBJECT(ethernet_btn), "connection-type", "ethernet");
    gtk_box_append(GTK_BOX(connection_box), ethernet_btn);
    
    /* WiFi option */
    wifi_btn = gtk_toggle_button_new_with_label("📶 Wireless Connection (WiFi)");
    gtk_widget_add_css_class(wifi_btn, "option-item");
    gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(wifi_btn), GTK_TOGGLE_BUTTON(ethernet_btn));
    g_object_set_data(G_OBJECT(wifi_btn), "connection-type", "wifi");
    gtk_box_append(GTK_BOX(connection_box), wifi_btn);
    
    /* WiFi section (initially hidden) */
    wifi_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(wifi_section, 20);
    gtk_widget_set_visible(wifi_section, FALSE);
    gtk_box_append(GTK_BOX(content), wifi_section);
    
    /* WiFi networks list */
    GtkWidget *wifi_label = gtk_label_new("Available Networks");
    gtk_widget_add_css_class(wifi_label, "section-title");
    gtk_widget_set_halign(wifi_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(wifi_section), wifi_label);
    
    wifi_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wifi_scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(wifi_scrolled), 200);
    gtk_widget_set_vexpand(wifi_scrolled, TRUE);
    gtk_box_append(GTK_BOX(wifi_section), wifi_scrolled);
    
    wifi_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(wifi_list, "option-list");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(wifi_scrolled), wifi_list);
    
    /* Add WiFi networks */
    for (int i = 0; sample_networks[i].ssid != NULL; i++) {
        GtkWidget *network_item = gtk_toggle_button_new();
        gtk_widget_add_css_class(network_item, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(network_item), 
                                   network_group ? GTK_TOGGLE_BUTTON(network_group->data) : NULL);
        if (!network_group) network_group = g_slist_append(network_group, network_item);
        
        /* Network content */
        GtkWidget *network_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
        gtk_toggle_button_set_child(GTK_TOGGLE_BUTTON(network_item), network_content);
        
        /* Network info */
        GtkWidget *network_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        gtk_widget_set_hexpand(network_info, TRUE);
        gtk_box_append(GTK_BOX(network_content), network_info);
        
        /* SSID */
        GtkWidget *ssid_label = gtk_label_new(sample_networks[i].ssid);
        gtk_widget_add_css_class(ssid_label, "option-title");
        gtk_widget_set_halign(ssid_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(network_info), ssid_label);
        
        /* Security */
        GtkWidget *security_label = gtk_label_new(sample_networks[i].security);
        gtk_widget_add_css_class(security_label, "option-subtitle");
        gtk_widget_set_halign(security_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(network_info), security_label);
        
        /* Signal strength */
        char strength_text[32];
        snprintf(strength_text, sizeof(strength_text), "%d%%", sample_networks[i].strength);
        GtkWidget *strength_label = gtk_label_new(strength_text);
        gtk_widget_add_css_class(strength_label, "signal-strength");
        gtk_box_append(GTK_BOX(network_content), strength_label);
        
        /* Store SSID */
        g_object_set_data(G_OBJECT(network_item), "ssid", (gpointer)sample_networks[i].ssid);
        
        /* Connect signal */
        g_signal_connect(network_item, "toggled", G_CALLBACK(on_network_selected), app);
        
        gtk_box_append(GTK_BOX(wifi_list), network_item);
    }
    
    /* WiFi password entry */
    password_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(password_box, 16);
    gtk_box_append(GTK_BOX(wifi_section), password_box);
    
    password_label = gtk_label_new("Network Password:");
    gtk_widget_add_css_class(password_label, "form-label");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(password_box), password_label);
    
    password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter network password...");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_widget_add_css_class(password_entry, "form-input");
    g_signal_connect(password_entry, "changed", G_CALLBACK(on_password_changed), app);
    gtk_box_append(GTK_BOX(password_box), password_entry);
    
    /* Set up connection type change handlers */
    g_object_set_data(G_OBJECT(ethernet_btn), "wifi-section", wifi_section);
    g_object_set_data(G_OBJECT(wifi_btn), "wifi-section", wifi_section);
    g_signal_connect(ethernet_btn, "toggled", G_CALLBACK(on_connection_type_changed), app);
    g_signal_connect(wifi_btn, "toggled", G_CALLBACK(on_connection_type_changed), app);
    
    /* Navigation buttons */
    nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(nav_box, "nav-buttons");
    gtk_widget_set_halign(nav_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(container), nav_box);
    
    /* Back button */
    back_btn = gtk_button_new_with_label("Back");
    gtk_widget_add_css_class(back_btn, "secondary-button");
    g_signal_connect(back_btn, "clicked", G_CALLBACK(on_back_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), back_btn);
    
    /* Skip button */
    skip_btn = gtk_button_new_with_label("Skip");
    gtk_widget_add_css_class(skip_btn, "secondary-button");
    g_signal_connect(skip_btn, "clicked", G_CALLBACK(on_skip_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), skip_btn);
    
    /* Next button */
    next_btn = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_btn, "primary-button");
    g_signal_connect(next_btn, "clicked", G_CALLBACK(on_next_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), next_btn);
    
    /* Store widget */
    app->screens[SCREEN_NETWORK].widget = container;
    
    /* Clean up */
    g_slist_free(network_group);
}
