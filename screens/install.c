#include "install.h"

struct _InstallScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *progress_bar;
    GtkWidget *status_label;
    GtkWidget *current_task_label;
    GtkWidget *install_icon;
    guint progress_timeout_id;
    gboolean installation_complete;
};

typedef struct {
    const gchar *task;
    gint duration; // in seconds (simulated)
} InstallTask;

static const InstallTask install_tasks[] = {
    {"Preparing installation...", 2},
    {"Partitioning disk...", 3},
    {"Formatting partitions...", 4},
    {"Installing base system...", 15},
    {"Installing packages...", 20},
    {"Configuring system...", 8},
    {"Installing bootloader...", 5},
    {"Setting up user account...", 3},
    {"Finalizing installation...", 2},
    {NULL, 0}
};

static void install_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(InstallScreen, install_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 install_screen_interface_init))

static gboolean update_progress(gpointer user_data) {
    InstallScreen *self = INSTALL_SCREEN(user_data);
    static int current_task = 0;
    static int task_progress = 0;
    static int total_progress = 0;
    
    // Calculate total duration for all tasks
    static int total_duration = 0;
    if (total_duration == 0) {
        for (int i = 0; install_tasks[i].task != NULL; i++) {
            total_duration += install_tasks[i].duration;
        }
    }
    
    if (current_task < G_N_ELEMENTS(install_tasks) - 1 && install_tasks[current_task].task != NULL) {
        // Update current task progress
        task_progress++;
        total_progress++;
        
        // Update UI
        gtk_label_set_text(GTK_LABEL(self->current_task_label), install_tasks[current_task].task);
        
        double fraction = (double)total_progress / total_duration;
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), fraction);
        
        gchar *status_text = g_strdup_printf("Installing Wave OS... (%d%%)", 
                                           (int)(fraction * 100));
        gtk_label_set_text(GTK_LABEL(self->status_label), status_text);
        g_free(status_text);
        
        // Move to next task if current one is complete
        if (task_progress >= install_tasks[current_task].duration) {
            current_task++;
            task_progress = 0;
        }
        
        return G_SOURCE_CONTINUE;
    } else {
        // Installation complete
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), 1.0);
        gtk_label_set_text(GTK_LABEL(self->status_label), "Installation Complete!");
        gtk_label_set_text(GTK_LABEL(self->current_task_label), "Wave OS has been successfully installed.");
        
        // Change icon to success
        gtk_image_set_from_icon_name(GTK_IMAGE(self->install_icon), "emblem-ok");
        gtk_widget_remove_css_class(self->install_icon, "install-icon");
        
        self->installation_complete = TRUE;
        self->progress_timeout_id = 0;
        return G_SOURCE_REMOVE;
    }
}

static GtkWidget *install_screen_create_widget(InstallerScreen *screen) {
    InstallScreen *self = INSTALL_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
    
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    
    // Main install container
    GtkWidget *install_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_add_css_class(install_container, "install-container");
    gtk_widget_set_halign(install_container, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(install_container, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(install_container, TRUE);
    gtk_widget_set_vexpand(install_container, TRUE);
    
    // Install icon
    self->install_icon = gtk_image_new_from_icon_name("system-software-install");
    gtk_image_set_pixel_size(GTK_IMAGE(self->install_icon), 64);
    gtk_widget_add_css_class(self->install_icon, "install-icon");
    gtk_widget_set_halign(self->install_icon, GTK_ALIGN_CENTER);
    
    // Status title
    self->status_label = gtk_label_new("Preparing to install Wave OS...");
    gtk_widget_add_css_class(self->status_label, "welcome-title");
    gtk_widget_set_halign(self->status_label, GTK_ALIGN_CENTER);
    
    // Progress bar
    self->progress_bar = gtk_progress_bar_new();
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(self->progress_bar), 0.0);
    gtk_widget_add_css_class(self->progress_bar, "install-progress");
    gtk_widget_set_size_request(self->progress_bar, 400, -1);
    gtk_widget_set_halign(self->progress_bar, GTK_ALIGN_CENTER);
    
    // Current task label
    self->current_task_label = gtk_label_new("Getting ready...");
    gtk_widget_add_css_class(self->current_task_label, "install-status");
    gtk_widget_set_halign(self->current_task_label, GTK_ALIGN_CENTER);
    
    // Information box
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(info_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_top(info_box, 48);
    
    GtkWidget *info_title = gtk_label_new("While you wait...");
    gtk_widget_add_css_class(info_title, "text-muted");
    
    GtkWidget *features_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    
    const gchar *features[] = {
        "🚀 Lightning-fast boot times",
        "🔒 Built-in security features", 
        "🎨 Beautiful modern interface",
        "📱 Mobile-friendly applications",
        NULL
    };
    
    for (int i = 0; features[i] != NULL; i++) {
        GtkWidget *feature_label = gtk_label_new(features[i]);
        gtk_widget_add_css_class(feature_label, "text-muted");
        gtk_widget_set_halign(feature_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(features_box), feature_label);
    }
    
    gtk_box_append(GTK_BOX(info_box), info_title);
    gtk_box_append(GTK_BOX(info_box), features_box);
    
    // Add all elements to main container
    gtk_box_append(GTK_BOX(install_container), self->install_icon);
    gtk_box_append(GTK_BOX(install_container), self->status_label);
    gtk_box_append(GTK_BOX(install_container), self->progress_bar);
    gtk_box_append(GTK_BOX(install_container), self->current_task_label);
    gtk_box_append(GTK_BOX(install_container), info_box);
    
    gtk_box_append(GTK_BOX(self->widget), install_container);
    
    return self->widget;
}

static void install_screen_show_screen(InstallerScreen *screen) {
    InstallScreen *self = INSTALL_SCREEN(screen);
    
    // Start the installation simulation when screen is shown
    if (self->progress_timeout_id == 0) {
        self->progress_timeout_id = g_timeout_add(1000, update_progress, self);
    }
}

static void install_screen_hide_screen(InstallerScreen *screen) {
    InstallScreen *self = INSTALL_SCREEN(screen);
    
    // Stop the progress timer if screen is hidden
    if (self->progress_timeout_id > 0) {
        g_source_remove(self->progress_timeout_id);
        self->progress_timeout_id = 0;
    }
}

static gboolean install_screen_validate(InstallerScreen *screen) {
    InstallScreen *self = INSTALL_SCREEN(screen);
    return self->installation_complete;
}

static void install_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = install_screen_create_widget;
    iface->show_screen = install_screen_show_screen;
    iface->hide_screen = install_screen_hide_screen;
    iface->validate = install_screen_validate;
}

static void install_screen_finalize(GObject *object) {
    InstallScreen *self = INSTALL_SCREEN(object);
    
    if (self->progress_timeout_id > 0) {
        g_source_remove(self->progress_timeout_id);
    }
    
    G_OBJECT_CLASS(install_screen_parent_class)->finalize(object);
}

static void install_screen_init(InstallScreen *self) {
    self->widget = NULL;
    self->progress_timeout_id = 0;
    self->installation_complete = FALSE;
}

static void install_screen_class_init(InstallScreenClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = install_screen_finalize;
}

InstallScreen *install_screen_new(void) {
    return g_object_new(INSTALL_TYPE_SCREEN, NULL);
}
