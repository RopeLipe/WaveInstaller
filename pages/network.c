#include "../installer.h"

static GtkWidget* wifi_toggle = NULL;
static GtkWidget* network_list_box = NULL;
static GtkWidget* selected_network_card = NULL;

static void on_network_card_clicked(GtkButton* button, gpointer user_data) {
    GtkWidget* card = GTK_WIDGET(button);
    const char* network_name = (const char*)user_data;
    
    // Remove selection from previous card
    if (selected_network_card) {
        gtk_widget_remove_css_class(selected_network_card, "selected-card");
    }
    
    // Add selection to new card
    selected_network_card = card;
    gtk_widget_add_css_class(card, "selected-card");
    
    // Show password dialog for secured networks
    gboolean is_secure = g_str_has_suffix(network_name, " (Secured)");
    if (is_secure) {
        show_wifi_password_dialog(main_window, network_name);
    }
}

static void on_wifi_toggle_changed(GtkCheckButton* toggle, gpointer user_data) {
    gboolean wifi_enabled = gtk_check_button_get_active(toggle);
    gtk_widget_set_sensitive(network_list_box, wifi_enabled);
    
    if (wifi_enabled) {
        // Simulate network scanning
        g_print("Scanning for Wi-Fi networks...\n");
    } else {
        // Clear network selection
        if (selected_network_card) {
            gtk_widget_remove_css_class(selected_network_card, "selected-card");
            selected_network_card = NULL;
        }
    }
}

GtkWidget* create_network_card(const char* name, const char* signal_strength, gboolean is_secure) {
    GtkWidget* card_button = gtk_button_new();
    gtk_widget_add_css_class(card_button, "network-card");
    
    char* full_name = g_strdup_printf("%s%s", name, is_secure ? " (Secured)" : "");
    g_signal_connect(card_button, "clicked", G_CALLBACK(on_network_card_clicked), full_name);
    
    GtkWidget* card_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(card_box, 12);
    gtk_widget_set_margin_bottom(card_box, 12);
    gtk_widget_set_margin_start(card_box, 16);
    gtk_widget_set_margin_end(card_box, 16);
    
    // Network icon
    const char* icon_name = is_secure ? "network-wireless-encrypted-symbolic" : "network-wireless-symbolic";
    GtkWidget* icon = gtk_image_new_from_icon_name(icon_name);
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 24);
    gtk_widget_add_css_class(icon, "network-icon");
    gtk_box_append(GTK_BOX(card_box), icon);
    
    // Network info
    GtkWidget* info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_hexpand(info_box, TRUE);
    gtk_widget_set_halign(info_box, GTK_ALIGN_START);
    
    GtkWidget* name_label = gtk_label_new(name);
    gtk_widget_add_css_class(name_label, "network-name");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), name_label);
    
    GtkWidget* security_label = gtk_label_new(is_secure ? "WPA2/WPA3 Security" : "Open Network");
    gtk_widget_add_css_class(security_label, "network-security");
    gtk_widget_set_halign(security_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), security_label);
    
    gtk_box_append(GTK_BOX(card_box), info_box);
    
    // Signal strength
    GtkWidget* signal_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_halign(signal_box, GTK_ALIGN_END);
    
    GtkWidget* signal_icon = gtk_image_new_from_icon_name("network-wireless-signal-excellent-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(signal_icon), 16);
    gtk_widget_add_css_class(signal_icon, "signal-icon");
    gtk_box_append(GTK_BOX(signal_box), signal_icon);
    
    GtkWidget* signal_label = gtk_label_new(signal_strength);
    gtk_widget_add_css_class(signal_label, "signal-strength");
    gtk_box_append(GTK_BOX(signal_box), signal_label);
    
    gtk_box_append(GTK_BOX(card_box), signal_box);
    
    gtk_button_set_child(GTK_BUTTON(card_button), card_box);
    
    return card_button;
}

void show_wifi_password_dialog(GtkWidget* parent, const char* network_name) {
    GtkWidget* dialog = gtk_dialog_new_with_buttons(
        "Wi-Fi Password",
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        "Cancel", GTK_RESPONSE_CANCEL,
        "Connect", GTK_RESPONSE_ACCEPT,
        NULL
    );
    
    gtk_widget_add_css_class(dialog, "wifi-dialog");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, -1);
    
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_widget_set_margin_top(content_area, 20);
    gtk_widget_set_margin_bottom(content_area, 20);
    gtk_widget_set_margin_start(content_area, 20);
    gtk_widget_set_margin_end(content_area, 20);
    
    GtkWidget* dialog_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    
    // Network name
    char* title_text = g_strdup_printf("Enter password for \"%s\"", network_name);
    GtkWidget* title_label = gtk_label_new(title_text);
    gtk_widget_add_css_class(title_label, "dialog-title");
    gtk_box_append(GTK_BOX(dialog_box), title_label);
    g_free(title_text);
    
    // Password entry
    GtkWidget* password_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    
    GtkWidget* password_label = gtk_label_new("Password:");
    gtk_widget_set_halign(password_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(password_label, "field-label");
    gtk_box_append(GTK_BOX(password_box), password_label);
    
    GtkWidget* password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_input_purpose(GTK_ENTRY(password_entry), GTK_INPUT_PURPOSE_PASSWORD);
    gtk_widget_add_css_class(password_entry, "password-entry");
    gtk_box_append(GTK_BOX(password_box), password_entry);
    
    // Show password toggle
    GtkWidget* show_password = gtk_check_button_new_with_label("Show password");
    gtk_widget_add_css_class(show_password, "show-password-check");
    g_signal_connect_swapped(show_password, "toggled", 
                            G_CALLBACK(gtk_entry_set_visibility), password_entry);
    gtk_box_append(GTK_BOX(password_box), show_password);
    
    gtk_box_append(GTK_BOX(dialog_box), password_box);
    
    gtk_box_append(GTK_BOX(content_area), dialog_box);
    
    gtk_widget_show(dialog);
    
    // Handle dialog response
    g_signal_connect(dialog, "response", G_CALLBACK(gtk_window_destroy), NULL);
}

GtkWidget* create_network_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Network Configuration");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Connect to the internet to download updates during installation.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);
    
    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_halign(content_box, GTK_ALIGN_FILL);
    
    // Connection type toggle
    GtkWidget* toggle_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(toggle_frame, "toggle-frame");
    
    GtkWidget* toggle_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(toggle_box, 16);
    gtk_widget_set_margin_bottom(toggle_box, 16);
    gtk_widget_set_margin_start(toggle_box, 16);
    gtk_widget_set_margin_end(toggle_box, 16);
    
    GtkWidget* wifi_icon = gtk_image_new_from_icon_name("network-wireless-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(wifi_icon), 24);
    gtk_box_append(GTK_BOX(toggle_box), wifi_icon);
    
    GtkWidget* wifi_label = gtk_label_new("Enable Wi-Fi");
    gtk_widget_add_css_class(wifi_label, "toggle-label");
    gtk_widget_set_hexpand(wifi_label, TRUE);
    gtk_widget_set_halign(wifi_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(toggle_box), wifi_label);
    
    wifi_toggle = gtk_check_button_new();
    gtk_widget_add_css_class(wifi_toggle, "wifi-toggle");
    gtk_check_button_set_active(GTK_CHECK_BUTTON(wifi_toggle), TRUE);
    g_signal_connect(wifi_toggle, "toggled", G_CALLBACK(on_wifi_toggle_changed), NULL);
    gtk_box_append(GTK_BOX(toggle_box), wifi_toggle);
    
    gtk_frame_set_child(GTK_FRAME(toggle_frame), toggle_box);
    gtk_box_append(GTK_BOX(content_box), toggle_frame);
    
    // Network list
    GtkWidget* network_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(network_frame, "network-frame");
    
    GtkWidget* network_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(network_container, 16);
    gtk_widget_set_margin_bottom(network_container, 16);
    gtk_widget_set_margin_start(network_container, 16);
    gtk_widget_set_margin_end(network_container, 16);
    
    GtkWidget* network_title = gtk_label_new("Available Networks");
    gtk_widget_add_css_class(network_title, "section-title");
    gtk_widget_set_halign(network_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(network_container), network_title);
      // Scrollable network list
    GtkWidget* scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);
    
    network_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    
    // Sample networks
    struct {
        const char* name;
        const char* strength;
        gboolean secure;
    } networks[] = {
        {"HomeNetwork_5G", "Strong", TRUE},
        {"CoffeeShop_WiFi", "Excellent", FALSE},
        {"MyRouter_2.4G", "Good", TRUE},
        {"PublicWiFi", "Weak", FALSE},
        {"Office_Network", "Strong", TRUE}
    };
    
    for (int i = 0; i < 5; i++) {
        GtkWidget* network_card = create_network_card(
            networks[i].name,
            networks[i].strength,
            networks[i].secure
        );
        gtk_box_append(GTK_BOX(network_list_box), network_card);
    }
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), network_list_box);
    gtk_box_append(GTK_BOX(network_container), scrolled);
    
    gtk_frame_set_child(GTK_FRAME(network_frame), network_container);
    gtk_box_append(GTK_BOX(content_box), network_frame);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    // Skip option
    GtkWidget* skip_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(skip_frame, "info-frame");
    
    GtkWidget* skip_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(skip_box, 16);
    gtk_widget_set_margin_bottom(skip_box, 16);
    gtk_widget_set_margin_start(skip_box, 16);
    gtk_widget_set_margin_end(skip_box, 16);
    
    GtkWidget* info_icon = gtk_image_new_from_icon_name("dialog-information-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(info_icon), 16);
    gtk_widget_add_css_class(info_icon, "info-icon");
    gtk_box_append(GTK_BOX(skip_box), info_icon);
    
    GtkWidget* skip_text = gtk_label_new("You can skip network configuration and set it up after installation.");
    gtk_widget_add_css_class(skip_text, "info-text");
    gtk_label_set_wrap(GTK_LABEL(skip_text), TRUE);
    gtk_box_append(GTK_BOX(skip_box), skip_text);
    
    gtk_frame_set_child(GTK_FRAME(skip_frame), skip_box);
    gtk_box_append(GTK_BOX(page), skip_frame);
    
    return page;
}
