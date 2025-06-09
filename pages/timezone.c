#include "../installer.h"

static GtkWidget* timezone_combo = NULL;
static GtkWidget* timezone_search = NULL;

static void on_timezone_search_changed(GtkEditable* editable, gpointer user_data) {
    const char* search_text = gtk_editable_get_text(editable);
    g_print("Searching timezones: %s\n", search_text);
}

GtkWidget* create_timezone_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Select Timezone");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Choose your timezone to configure the system clock correctly.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);
    
    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(content_box, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(content_box, 400, -1);
    
    // Search entry for timezones
    timezone_search = gtk_search_entry_new();
    gtk_search_entry_set_placeholder_text(GTK_SEARCH_ENTRY(timezone_search), "Search timezones...");
    gtk_widget_add_css_class(timezone_search, "search-entry");
    g_signal_connect(timezone_search, "search-changed", G_CALLBACK(on_timezone_search_changed), NULL);
    gtk_box_append(GTK_BOX(content_box), timezone_search);
    
    // Timezone selection combo box
    timezone_combo = gtk_combo_box_text_new();
    gtk_widget_add_css_class(timezone_combo, "timezone-combo");
    
    // Add sample timezones in Continent/City format
    const char* timezones[] = {
        "Africa/Cairo",
        "America/New_York",
        "America/Los_Angeles",
        "America/Chicago",
        "America/Denver",
        "America/Sao_Paulo",
        "America/Mexico_City",
        "Asia/Tokyo",
        "Asia/Shanghai",
        "Asia/Seoul",
        "Asia/Mumbai",
        "Asia/Dubai",
        "Asia/Singapore",
        "Europe/London",
        "Europe/Paris",
        "Europe/Berlin",
        "Europe/Rome",
        "Europe/Moscow",
        "Europe/Stockholm",
        "Europe/Amsterdam",
        "Australia/Sydney",
        "Australia/Melbourne",
        "Pacific/Auckland",
        "Pacific/Honolulu"
    };
    
    for (int i = 0; i < 24; i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(timezone_combo), timezones[i]);
    }
    
    // Set default selection (UTC)
    gtk_combo_box_text_prepend_text(GTK_COMBO_BOX_TEXT(timezone_combo), "UTC");
    gtk_combo_box_set_active(GTK_COMBO_BOX(timezone_combo), 0);
    
    gtk_box_append(GTK_BOX(content_box), timezone_combo);
    
    // Current time display
    GtkWidget* time_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(time_frame, "time-frame");
    
    GtkWidget* time_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(time_box, 16);
    gtk_widget_set_margin_bottom(time_box, 16);
    gtk_widget_set_margin_start(time_box, 16);
    gtk_widget_set_margin_end(time_box, 16);
    gtk_widget_set_halign(time_box, GTK_ALIGN_CENTER);
    
    GtkWidget* time_label = gtk_label_new("Current Time");
    gtk_widget_add_css_class(time_label, "time-label");
    gtk_box_append(GTK_BOX(time_box), time_label);
    
    // Get current time (placeholder)
    GtkWidget* current_time = gtk_label_new("2025-06-09 14:30:00");
    gtk_widget_add_css_class(current_time, "current-time");
    gtk_box_append(GTK_BOX(time_box), current_time);
    
    gtk_frame_set_child(GTK_FRAME(time_frame), time_box);
    gtk_box_append(GTK_BOX(content_box), time_frame);
    
    // Timezone info
    GtkWidget* info_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(info_frame, "info-frame");
    
    GtkWidget* info_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(info_box, 16);
    gtk_widget_set_margin_bottom(info_box, 16);
    gtk_widget_set_margin_start(info_box, 16);
    gtk_widget_set_margin_end(info_box, 16);
    
    GtkWidget* info_icon = gtk_image_new_from_icon_name("dialog-information-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(info_icon), 16);
    gtk_widget_add_css_class(info_icon, "info-icon");
    gtk_box_append(GTK_BOX(info_box), info_icon);
    
    GtkWidget* info_label = gtk_label_new("The system will automatically synchronize with internet time servers.");
    gtk_widget_add_css_class(info_label, "info-text");
    gtk_label_set_wrap(GTK_LABEL(info_label), TRUE);
    gtk_box_append(GTK_BOX(info_box), info_label);
    
    gtk_frame_set_child(GTK_FRAME(info_frame), info_box);
    gtk_box_append(GTK_BOX(content_box), info_frame);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    return page;
}
