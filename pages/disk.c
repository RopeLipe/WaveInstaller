#include "../installer.h"

static GtkWidget* selected_disk_card = NULL;

static void on_disk_card_clicked(GtkButton* button, gpointer user_data) {
    GtkWidget* card = GTK_WIDGET(button);
    
    // Remove selection from previous card
    if (selected_disk_card) {
        gtk_widget_remove_css_class(selected_disk_card, "selected-card");
    }
    
    // Add selection to new card
    selected_disk_card = card;
    gtk_widget_add_css_class(card, "selected-card");
}

GtkWidget* create_disk_card(const char* name, const char* size, const char* type, const char* icon_name) {
    GtkWidget* card_button = gtk_button_new();
    gtk_widget_add_css_class(card_button, "disk-card");
    g_signal_connect(card_button, "clicked", G_CALLBACK(on_disk_card_clicked), NULL);
    
    GtkWidget* card_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(card_box, 16);
    gtk_widget_set_margin_bottom(card_box, 16);
    gtk_widget_set_margin_start(card_box, 16);
    gtk_widget_set_margin_end(card_box, 16);
    
    // Disk icon
    GtkWidget* icon = gtk_image_new_from_icon_name(icon_name);
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 48);
    gtk_widget_add_css_class(icon, "disk-icon");
    gtk_box_append(GTK_BOX(card_box), icon);
    
    // Disk info
    GtkWidget* info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_hexpand(info_box, TRUE);
    gtk_widget_set_halign(info_box, GTK_ALIGN_START);
    
    GtkWidget* name_label = gtk_label_new(name);
    gtk_widget_add_css_class(name_label, "disk-name");
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), name_label);
    
    GtkWidget* size_label = gtk_label_new(size);
    gtk_widget_add_css_class(size_label, "disk-size");
    gtk_widget_set_halign(size_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), size_label);
    
    GtkWidget* type_label = gtk_label_new(type);
    gtk_widget_add_css_class(type_label, "disk-type");
    gtk_widget_set_halign(type_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(info_box), type_label);
    
    gtk_box_append(GTK_BOX(card_box), info_box);
    
    // Selection indicator
    GtkWidget* check_icon = gtk_image_new_from_icon_name("emblem-ok-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(check_icon), 20);
    gtk_widget_add_css_class(check_icon, "selection-check");
    gtk_box_append(GTK_BOX(card_box), check_icon);
    
    gtk_button_set_child(GTK_BUTTON(card_button), card_box);
    
    return card_button;
}

GtkWidget* create_disk_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Select Installation Disk");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Choose the disk where Wave OS will be installed. All data on the selected disk will be erased.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);
    
    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_halign(content_box, GTK_ALIGN_FILL);
    gtk_widget_set_size_request(content_box, -1, 300);
    
    // Scrollable disk list
    GtkWidget* scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_widget_add_css_class(scrolled, "disk-list");
    
    GtkWidget* disk_list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(disk_list_box, 12);
    gtk_widget_set_margin_bottom(disk_list_box, 12);
    gtk_widget_set_margin_start(disk_list_box, 12);
    gtk_widget_set_margin_end(disk_list_box, 12);
    
    // Sample disks
    struct {
        const char* name;
        const char* size;
        const char* type;
        const char* icon;
    } disks[] = {
        {"Samsung SSD 980 PRO 1TB", "931.5 GB", "NVMe SSD", "drive-harddisk-solidstate"},
        {"Western Digital Blue 2TB", "1.82 TB", "SATA HDD", "drive-harddisk"},
        {"Kingston USB Drive", "32.0 GB", "USB Storage", "drive-removable-media"},
        {"Seagate Backup Plus", "4.0 TB", "External HDD", "drive-harddisk-usb"}
    };
    
    for (int i = 0; i < 4; i++) {
        GtkWidget* disk_card = create_disk_card(
            disks[i].name,
            disks[i].size,
            disks[i].type,
            disks[i].icon
        );
        gtk_box_append(GTK_BOX(disk_list_box), disk_card);
    }
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), disk_list_box);
    gtk_box_append(GTK_BOX(content_box), scrolled);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    // Warning message
    GtkWidget* warning_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(warning_frame, "warning-frame");
    
    GtkWidget* warning_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_margin_top(warning_box, 16);
    gtk_widget_set_margin_bottom(warning_box, 16);
    gtk_widget_set_margin_start(warning_box, 16);
    gtk_widget_set_margin_end(warning_box, 16);
    
    GtkWidget* warning_icon = gtk_image_new_from_icon_name("dialog-warning-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(warning_icon), 20);
    gtk_widget_add_css_class(warning_icon, "warning-icon");
    gtk_box_append(GTK_BOX(warning_box), warning_icon);
    
    GtkWidget* warning_text = gtk_label_new("Warning: Installing Wave OS will erase all existing data on the selected disk. Make sure to backup any important files before proceeding.");
    gtk_widget_add_css_class(warning_text, "warning-text");
    gtk_label_set_wrap(GTK_LABEL(warning_text), TRUE);
    gtk_widget_set_hexpand(warning_text, TRUE);
    gtk_box_append(GTK_BOX(warning_box), warning_text);
    
    gtk_frame_set_child(GTK_FRAME(warning_frame), warning_box);
    gtk_box_append(GTK_BOX(page), warning_frame);
    
    return page;
}
