/* timezone-screen.c - Timezone selection screen implementation */
#include "timezone-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *zone;
    const char *description;
    const char *region;
} Timezone;

static Timezone timezones[] = {
    {"America/New_York", "New York (Eastern Time)", "Americas"},
    {"America/Chicago", "Chicago (Central Time)", "Americas"},
    {"America/Denver", "Denver (Mountain Time)", "Americas"},
    {"America/Los_Angeles", "Los Angeles (Pacific Time)", "Americas"},
    {"America/Toronto", "Toronto", "Americas"},
    {"America/Vancouver", "Vancouver", "Americas"},
    {"America/Mexico_City", "Mexico City", "Americas"},
    {"America/Sao_Paulo", "São Paulo", "Americas"},
    {"America/Buenos_Aires", "Buenos Aires", "Americas"},
    {"Europe/London", "London", "Europe"},
    {"Europe/Paris", "Paris", "Europe"},
    {"Europe/Berlin", "Berlin", "Europe"},
    {"Europe/Rome", "Rome", "Europe"},
    {"Europe/Madrid", "Madrid", "Europe"},
    {"Europe/Amsterdam", "Amsterdam", "Europe"},
    {"Europe/Stockholm", "Stockholm", "Europe"},
    {"Europe/Moscow", "Moscow", "Europe"},
    {"Asia/Tokyo", "Tokyo", "Asia"},
    {"Asia/Shanghai", "Shanghai", "Asia"},
    {"Asia/Hong_Kong", "Hong Kong", "Asia"},
    {"Asia/Singapore", "Singapore", "Asia"},
    {"Asia/Seoul", "Seoul", "Asia"},
    {"Asia/Mumbai", "Mumbai", "Asia"},
    {"Asia/Dubai", "Dubai", "Asia"},
    {"Asia/Bangkok", "Bangkok", "Asia"},
    {"Australia/Sydney", "Sydney", "Australia"},
    {"Australia/Melbourne", "Melbourne", "Australia"},
    {"Australia/Perth", "Perth", "Australia"},
    {"Pacific/Auckland", "Auckland", "Pacific"},
    {"Africa/Cairo", "Cairo", "Africa"},
    {"Africa/Johannesburg", "Johannesburg", "Africa"},
    {NULL, NULL, NULL}
};

static char *selected_timezone = NULL;
static GtkWidget *timezone_box = NULL;
static GtkWidget *search_entry = NULL;

static void
on_timezone_selected(GtkToggleButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    
    if (gtk_toggle_button_get_active(button)) {
        const char *tz = (const char *)g_object_get_data(G_OBJECT(button), "timezone");
        if (selected_timezone) {
            g_free(selected_timezone);
        }
        selected_timezone = g_strdup(tz);
    }
}

static void
populate_timezone_list(const char *filter) {
    GtkWidget *child;
    GSList *group = NULL;
    
    /* Clear existing items */
    while ((child = gtk_widget_get_first_child(timezone_box)) != NULL) {
        gtk_box_remove(GTK_BOX(timezone_box), child);
    }
    
    /* Add filtered timezone options */
    for (int i = 0; timezones[i].zone != NULL; i++) {
        /* Apply filter if provided */
        if (filter && strlen(filter) > 0) {
            char *lower_filter = g_utf8_strdown(filter, -1);
            char *lower_zone = g_utf8_strdown(timezones[i].zone, -1);
            char *lower_desc = g_utf8_strdown(timezones[i].description, -1);
            
            if (!strstr(lower_zone, lower_filter) && !strstr(lower_desc, lower_filter)) {
                g_free(lower_filter);
                g_free(lower_zone);
                g_free(lower_desc);
                continue;
            }
            
            g_free(lower_filter);
            g_free(lower_zone);
            g_free(lower_desc);
        }
        
        /* Timezone item container */
        GtkWidget *tz_item = gtk_toggle_button_new();
        gtk_widget_add_css_class(tz_item, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(tz_item), 
                                   group ? GTK_TOGGLE_BUTTON(group->data) : NULL);
        if (!group) group = g_slist_append(group, tz_item);
        
        /* Timezone content */
        GtkWidget *tz_content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        gtk_button_set_child(GTK_BUTTON(tz_item), tz_content);
        
        /* Main name */
        GtkWidget *name_label = gtk_label_new(timezones[i].description);
        gtk_widget_add_css_class(name_label, "option-title");
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(tz_content), name_label);
        
        /* Zone path */
        GtkWidget *zone_label = gtk_label_new(timezones[i].zone);
        gtk_widget_add_css_class(zone_label, "option-subtitle");
        gtk_widget_set_halign(zone_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(tz_content), zone_label);
        
        /* Store timezone */
        g_object_set_data(G_OBJECT(tz_item), "timezone", (gpointer)timezones[i].zone);
        
        /* Connect signal */
        g_signal_connect(tz_item, "toggled", G_CALLBACK(on_timezone_selected), NULL);
        
        gtk_box_append(GTK_BOX(timezone_box), tz_item);
    }
    
    g_slist_free(group);
}

static void
on_search_changed(GtkEditable *editable, gpointer user_data) {
    const char *text = gtk_editable_get_text(editable);
    populate_timezone_list(text);
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    if (selected_timezone) {
        wave_installer_application_next_screen(app);
    }
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

void
timezone_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *search_box, *scrolled, *nav_box, *back_btn, *next_btn;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Select Your Timezone");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Choose your timezone to set the correct system time");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Search box */
    search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_bottom(search_box, 20);
    
    search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search for your city or timezone...");
    gtk_widget_add_css_class(search_entry, "form-input");
    gtk_widget_set_hexpand(search_entry, TRUE);
    g_signal_connect(search_entry, "changed", G_CALLBACK(on_search_changed), app);
    gtk_box_append(GTK_BOX(search_box), search_entry);
    
    gtk_box_append(GTK_BOX(content), search_box);
    
    /* Scrolled window for timezones */
    scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled), 350);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append(GTK_BOX(content), scrolled);
    
    /* Timezone list */
    timezone_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(timezone_box, "option-list");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), timezone_box);
    
    /* Populate timezone list */
    populate_timezone_list(NULL);
    
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
    
    /* Next button */
    next_btn = gtk_button_new_with_label("Next");
    gtk_widget_add_css_class(next_btn, "primary-button");
    g_signal_connect(next_btn, "clicked", G_CALLBACK(on_next_clicked), app);
    gtk_box_append(GTK_BOX(nav_box), next_btn);
    
    /* Store widget */
    app->screens[SCREEN_TIMEZONE].widget = container;
}
