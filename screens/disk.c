#include "disk.h"

struct _DiskScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *disk_grid;
    GtkWidget *selected_disk_card;
    gchar *selected_disk_path;
};

typedef struct {
    const gchar *path;
    const gchar *name;
    const gchar *size;
    const gchar *type;
    gboolean is_removable;
    gboolean has_os;
} DiskInfo;

// Mock disk data - in real implementation, this would come from system detection
static const DiskInfo mock_disks[] = {
    {"/dev/sda", "Samsung SSD 980 PRO", "500 GB", "NVMe SSD", FALSE, TRUE},
    {"/dev/sdb", "WD Blue HDD", "1 TB", "SATA HDD", FALSE, FALSE},
    {"/dev/sdc", "SanDisk Ultra", "32 GB", "USB Drive", TRUE, FALSE},
    {NULL, NULL, NULL, NULL, FALSE, FALSE}
};

static void disk_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(DiskScreen, disk_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 disk_screen_interface_init))

static void on_disk_card_clicked(GtkGestureClick *gesture, gint n_press, gdouble x, gdouble y, gpointer user_data) {
    DiskScreen *self = DISK_SCREEN(g_object_get_data(G_OBJECT(user_data), "screen"));
    GtkWidget *clicked_card = GTK_WIDGET(user_data);
    
    // Remove selection from previous card
    if (self->selected_disk_card) {
        gtk_widget_remove_css_class(self->selected_disk_card, "selected");
    }
    
    // Select new card
    self->selected_disk_card = clicked_card;
    gtk_widget_add_css_class(clicked_card, "selected");
    
    // Store selected disk path
    g_free(self->selected_disk_path);
    self->selected_disk_path = g_strdup(g_object_get_data(G_OBJECT(clicked_card), "disk-path"));
}

static GtkWidget *create_disk_card(const DiskInfo *disk, DiskScreen *self) {
    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_add_css_class(card, "disk-card");
    gtk_widget_set_size_request(card, 320, 200);
    
    // Store disk path in card data
    g_object_set_data_full(G_OBJECT(card), "disk-path", g_strdup(disk->path), g_free);
    g_object_set_data(G_OBJECT(card), "screen", self);
    
    // Add click gesture
    GtkGesture *click_gesture = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(click_gesture), GDK_BUTTON_PRIMARY);
    g_signal_connect(click_gesture, "pressed", G_CALLBACK(on_disk_card_clicked), card);
    gtk_widget_add_controller(card, GTK_EVENT_CONTROLLER(click_gesture));
    
    // Card has background and padding
    gtk_widget_set_margin_top(card, 8);
    gtk_widget_set_margin_bottom(card, 8);
    gtk_widget_set_margin_start(card, 8);
    gtk_widget_set_margin_end(card, 8);
    
    // Header with large disk icon
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(header_box, 20);
    
    // Large disk icon
    GtkWidget *disk_icon = gtk_image_new_from_icon_name(
        g_str_equal(disk->type, "SSD") ? "drive-harddisk-solidstate" : "drive-harddisk");
    gtk_image_set_pixel_size(GTK_IMAGE(disk_icon), 48);
    gtk_widget_add_css_class(disk_icon, "disk-icon-large");
    
    // Disk name (prominent)
    GtkWidget *name_label = gtk_label_new(disk->name);
    gtk_widget_add_css_class(name_label, "disk-name-large");
    gtk_label_set_ellipsize(GTK_LABEL(name_label), PANGO_ELLIPSIZE_MIDDLE);
    
    gtk_box_append(GTK_BOX(header_box), disk_icon);
    gtk_box_append(GTK_BOX(header_box), name_label);
    
    // Disk details section
    GtkWidget *details_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_halign(details_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(details_box, 20);
    
    // Size (most prominent)
    GtkWidget *size_label = gtk_label_new(disk->size);
    gtk_widget_add_css_class(size_label, "disk-size-large");
    
    // Type
    GtkWidget *type_label = gtk_label_new(disk->type);
    gtk_widget_add_css_class(type_label, "disk-type");
    
    // Path (smaller)
    GtkWidget *path_label = gtk_label_new(disk->path);
    gtk_widget_add_css_class(path_label, "disk-path");
    
    gtk_box_append(GTK_BOX(details_box), size_label);
    gtk_box_append(GTK_BOX(details_box), type_label);
    gtk_box_append(GTK_BOX(details_box), path_label);
    
    // Warning if has OS (subtle badge)
    if (disk->has_os) {
        GtkWidget *warning_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
        gtk_widget_set_halign(warning_box, GTK_ALIGN_CENTER);
        gtk_widget_set_margin_top(warning_box, 8);
        gtk_widget_add_css_class(warning_box, "disk-warning-badge");
        
        GtkWidget *warning_icon = gtk_image_new_from_icon_name("dialog-warning");
        gtk_image_set_pixel_size(GTK_IMAGE(warning_icon), 14);
        gtk_widget_add_css_class(warning_icon, "warning-icon-small");
        
        GtkWidget *warning_label = gtk_label_new("Contains OS");
        gtk_widget_add_css_class(warning_label, "disk-warning-text");
        
        gtk_box_append(GTK_BOX(warning_box), warning_icon);
        gtk_box_append(GTK_BOX(warning_box), warning_label);
        gtk_box_append(GTK_BOX(details_box), warning_box);
    }
    
    gtk_box_append(GTK_BOX(card), header_box);
    gtk_box_append(GTK_BOX(card), details_box);    
    gtk_box_append(GTK_BOX(card), header_box);
    gtk_box_append(GTK_BOX(card), details_box);
    
    return card;
}

static GtkWidget *disk_screen_create_widget(InstallerScreen *screen) {
    DiskScreen *self = DISK_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
      self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    gtk_widget_set_margin_top(self->widget, 48);
    gtk_widget_set_margin_start(self->widget, 40);
    gtk_widget_set_margin_end(self->widget, 40);
      // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    
    GtkWidget *title = gtk_label_new("Choose Installation Disk");
    gtk_widget_add_css_class(title, "screen-title");
    
    GtkWidget *subtitle = gtk_label_new("Select the disk where you want to install Wave OS");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Disk grid container
    GtkWidget *disk_section = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    
    // Create grid for disk cards
    self->disk_grid = gtk_grid_new();
    gtk_widget_add_css_class(self->disk_grid, "disk-grid");
    gtk_grid_set_row_spacing(GTK_GRID(self->disk_grid), 20);
    gtk_grid_set_column_spacing(GTK_GRID(self->disk_grid), 20);
    gtk_widget_set_halign(self->disk_grid, GTK_ALIGN_CENTER);
    
    // Add disk cards to grid (2 columns max for better layout)
    int col = 0, row = 0;
    for (int i = 0; mock_disks[i].path != NULL; i++) {
        GtkWidget *card = create_disk_card(&mock_disks[i], self);
        gtk_grid_attach(GTK_GRID(self->disk_grid), card, col, row, 1, 1);
        
        col++;
        if (col >= 2) {
            col = 0;
            row++;
        }
    }
    
    gtk_box_append(GTK_BOX(disk_section), self->disk_grid);
    
    // Installation options in a card
    GtkWidget *options_card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(options_card, "options-card");
    gtk_widget_set_margin_top(options_card, 16);
    
    // Options header
    GtkWidget *options_header = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(options_header, 24);
    gtk_widget_set_margin_bottom(options_header, 16);
    gtk_widget_set_margin_start(options_header, 24);
    gtk_widget_set_margin_end(options_header, 24);
    
    GtkWidget *options_title = gtk_label_new("Installation Options");
    gtk_widget_add_css_class(options_title, "options-title");
    gtk_widget_set_halign(options_title, GTK_ALIGN_START);
    
    gtk_box_append(GTK_BOX(options_header), options_title);
    
    // Options list
    GtkWidget *options_list = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(options_list, 24);
    gtk_widget_set_margin_end(options_list, 24);
    gtk_widget_set_margin_bottom(options_list, 24);
    
    // Encryption option
    GtkWidget *encrypt_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *encrypt_check = gtk_check_button_new();
    GtkWidget *encrypt_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    
    GtkWidget *encrypt_label = gtk_label_new("Encrypt the installation");
    gtk_widget_add_css_class(encrypt_label, "option-label");
    gtk_widget_set_halign(encrypt_label, GTK_ALIGN_START);
    
    GtkWidget *encrypt_desc = gtk_label_new("Protects your data with a password");
    gtk_widget_add_css_class(encrypt_desc, "option-description");
    gtk_widget_set_halign(encrypt_desc, GTK_ALIGN_START);
    
    gtk_box_append(GTK_BOX(encrypt_info), encrypt_label);
    gtk_box_append(GTK_BOX(encrypt_info), encrypt_desc);
    gtk_box_append(GTK_BOX(encrypt_box), encrypt_check);
    gtk_box_append(GTK_BOX(encrypt_box), encrypt_info);
    
    // Format option (default checked)
    GtkWidget *format_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    GtkWidget *format_check = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(format_check), TRUE);
    GtkWidget *format_info = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    
    GtkWidget *format_label = gtk_label_new("Format disk before installation");
    gtk_widget_add_css_class(format_label, "option-label");
    gtk_widget_set_halign(format_label, GTK_ALIGN_START);
    
    GtkWidget *format_desc = gtk_label_new("Recommended - erases all existing data");
    gtk_widget_add_css_class(format_desc, "option-description");
    gtk_widget_set_halign(format_desc, GTK_ALIGN_START);
    
    gtk_box_append(GTK_BOX(format_info), format_label);
    gtk_box_append(GTK_BOX(format_info), format_desc);
    gtk_box_append(GTK_BOX(format_box), format_check);
    gtk_box_append(GTK_BOX(format_box), format_info);
    
    gtk_box_append(GTK_BOX(options_list), encrypt_box);
    gtk_box_append(GTK_BOX(options_list), format_box);
    
    gtk_box_append(GTK_BOX(options_card), options_header);
    gtk_box_append(GTK_BOX(options_card), options_list);
    
    gtk_box_append(GTK_BOX(disk_section), options_card);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), disk_section);
    
    return self->widget;
}

static void disk_screen_show_screen(InstallerScreen *screen) {
    // Called when screen becomes visible
    // Could refresh disk list here
}

static void disk_screen_hide_screen(InstallerScreen *screen) {
    // Called when screen becomes hidden
}

static gboolean disk_screen_validate(InstallerScreen *screen) {
    DiskScreen *self = DISK_SCREEN(screen);
    return self->selected_disk_path != NULL;
}

static void disk_screen_next_clicked(InstallerScreen *screen) {
    DiskScreen *self = DISK_SCREEN(screen);
    if (self->selected_disk_path) {
        g_print("Selected disk: %s\\n", self->selected_disk_path);
    }
}

static void disk_screen_back_clicked(InstallerScreen *screen) {
    // Handle back button click if needed
}

static void disk_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = disk_screen_create_widget;
    iface->show_screen = disk_screen_show_screen;
    iface->hide_screen = disk_screen_hide_screen;
    iface->validate = disk_screen_validate;
    iface->next_clicked = disk_screen_next_clicked;
    iface->back_clicked = disk_screen_back_clicked;
}

static void disk_screen_finalize(GObject *object) {
    DiskScreen *self = DISK_SCREEN(object);
    g_free(self->selected_disk_path);
    G_OBJECT_CLASS(disk_screen_parent_class)->finalize(object);
}

static void disk_screen_init(DiskScreen *self) {
    self->widget = NULL;
    self->disk_grid = NULL;
    self->selected_disk_card = NULL;
    self->selected_disk_path = NULL;
}

static void disk_screen_class_init(DiskScreenClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = disk_screen_finalize;
}

DiskScreen *disk_screen_new(void) {
    return g_object_new(DISK_TYPE_SCREEN, NULL);
}
