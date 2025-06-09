/* disk-screen.c - Disk partitioning screen implementation */
#include "disk-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *device;
    const char *name;
    const char *size;
    const char *type;
} DiskInfo;

static DiskInfo sample_disks[] = {
    {"/dev/sda", "SAMSUNG SSD 970 EVO Plus", "500 GB", "SSD"},
    {"/dev/sdb", "WD Blue HDD", "1 TB", "HDD"},
    {"/dev/nvme0n1", "Intel NVMe SSD", "256 GB", "NVMe"},
    {NULL, NULL, NULL, NULL}
};

static char *selected_disk = NULL;
static char *selected_scheme = NULL;

static void
on_disk_selected(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *disk = (const char *)g_object_get_data(G_OBJECT(button), "disk-device");
        if (selected_disk) g_free(selected_disk);
        selected_disk = g_strdup(disk);
    }
}

static void
on_scheme_selected(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *scheme = (const char *)g_object_get_data(G_OBJECT(button), "scheme");
        if (selected_scheme) g_free(selected_scheme);
        selected_scheme = g_strdup(scheme);
    }
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    if (selected_disk && selected_scheme) {
        wave_installer_application_next_screen(app);
    }
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

void
disk_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *warning_box, *warning_icon, *warning_text;
    GtkWidget *disk_scrolled, *disk_container, *scheme_box, *scheme_label, *nav_box, *back_btn, *next_btn;
    GSList *disk_group = NULL, *scheme_group = NULL;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Disk Selection");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Choose where to install the system. All data on the selected disk will be erased.");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Warning box */
    warning_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_add_css_class(warning_box, "card");
    gtk_widget_add_css_class(warning_box, "warning");
    gtk_widget_set_margin_bottom(warning_box, 20);
    
    warning_icon = gtk_image_new_from_icon_name("dialog-warning");
    gtk_image_set_pixel_size(GTK_IMAGE(warning_icon), 24);
    gtk_box_append(GTK_BOX(warning_box), warning_icon);
    
    warning_text = gtk_label_new("Warning: This will permanently erase all data on the selected disk.");
    gtk_label_set_wrap(GTK_LABEL(warning_text), TRUE);
    gtk_widget_add_css_class(warning_text, "warning-text");
    gtk_box_append(GTK_BOX(warning_box), warning_text);
    
    gtk_box_append(GTK_BOX(content), warning_box);
    
    /* Disk selection area */
    disk_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(disk_scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(disk_scrolled), 300);
    gtk_widget_set_vexpand(disk_scrolled, TRUE);
    gtk_box_append(GTK_BOX(content), disk_scrolled);
    
    disk_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_add_css_class(disk_container, "disk-grid");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(disk_scrolled), disk_container);
    
    /* Add disk options */
    for (int i = 0; sample_disks[i].device != NULL; i++) {
        /* Disk card */
        GtkWidget *disk_card = gtk_toggle_button_new();
        gtk_widget_add_css_class(disk_card, "disk-card");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(disk_card), 
                                   disk_group ? GTK_TOGGLE_BUTTON(disk_group->data) : NULL);
        if (!disk_group) disk_group = g_slist_append(disk_group, disk_card);
          /* Disk content */
        GtkWidget *disk_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
        gtk_button_set_child(GTK_BUTTON(disk_card), disk_content);
        
        /* Disk icon */
        GtkWidget *disk_icon = gtk_image_new_from_icon_name("drive-harddisk");
        gtk_image_set_pixel_size(GTK_IMAGE(disk_icon), 48);
        gtk_box_append(GTK_BOX(disk_content), disk_icon);
        
        /* Disk info */
        GtkWidget *disk_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        gtk_widget_set_hexpand(disk_info, TRUE);
        gtk_box_append(GTK_BOX(disk_content), disk_info);
        
        /* Disk name */
        GtkWidget *name_label = gtk_label_new(sample_disks[i].name);
        gtk_widget_add_css_class(name_label, "disk-name");
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(disk_info), name_label);
        
        /* Disk device */
        GtkWidget *device_label = gtk_label_new(sample_disks[i].device);
        gtk_widget_add_css_class(device_label, "disk-device");
        gtk_widget_set_halign(device_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(disk_info), device_label);
        
        /* Disk details */
        char details[256];
        snprintf(details, sizeof(details), "%s • %s", sample_disks[i].size, sample_disks[i].type);
        GtkWidget *details_label = gtk_label_new(details);
        gtk_widget_add_css_class(details_label, "disk-details");
        gtk_widget_set_halign(details_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(disk_info), details_label);
        
        /* Store disk device */
        g_object_set_data(G_OBJECT(disk_card), "disk-device", (gpointer)sample_disks[i].device);
        
        /* Connect signal */
        g_signal_connect(disk_card, "toggled", G_CALLBACK(on_disk_selected), app);
        
        gtk_box_append(GTK_BOX(disk_container), disk_card);
    }
    
    /* Partition scheme options */
    scheme_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(scheme_box, 20);
    gtk_box_append(GTK_BOX(content), scheme_box);
    
    scheme_label = gtk_label_new("Partition Scheme");
    gtk_widget_add_css_class(scheme_label, "section-title");
    gtk_widget_set_halign(scheme_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(scheme_box), scheme_label);
    
    /* Scheme options */
    const char *schemes[][2] = {
        {"auto", "Automatic partitioning (recommended)"},
        {"manual", "Manual partitioning (advanced)"},
        {NULL, NULL}
    };
    
    for (int i = 0; schemes[i][0] != NULL; i++) {
        GtkWidget *scheme_btn = gtk_toggle_button_new_with_label(schemes[i][1]);
        gtk_widget_add_css_class(scheme_btn, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(scheme_btn), 
                                   scheme_group ? GTK_TOGGLE_BUTTON(scheme_group->data) : NULL);
        if (!scheme_group) scheme_group = g_slist_append(scheme_group, scheme_btn);
        
        /* Select auto by default */
        if (strcmp(schemes[i][0], "auto") == 0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scheme_btn), TRUE);
            selected_scheme = g_strdup(schemes[i][0]);
        }
        
        /* Store scheme */
        g_object_set_data(G_OBJECT(scheme_btn), "scheme", (gpointer)schemes[i][0]);
        
        /* Connect signal */
        g_signal_connect(scheme_btn, "toggled", G_CALLBACK(on_scheme_selected), app);
        
        gtk_box_append(GTK_BOX(scheme_box), scheme_btn);
    }
    
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
    app->screens[SCREEN_DISK].widget = container;
    
    /* Clean up */
    g_slist_free(disk_group);
    g_slist_free(scheme_group);
}
