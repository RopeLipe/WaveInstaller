#include "timezone.h"

struct _TimezoneScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *timezone_listbox;
    gchar *selected_timezone;
};

typedef struct {
    const gchar *timezone;
    const gchar *city;
    const gchar *country;
    const gchar *offset;
} TimezoneInfo;

static const TimezoneInfo timezones[] = {
    {"America/New_York", "New York", "United States", "UTC-5"},
    {"America/Los_Angeles", "Los Angeles", "United States", "UTC-8"},
    {"Europe/London", "London", "United Kingdom", "UTC+0"},
    {"Europe/Paris", "Paris", "France", "UTC+1"},
    {"Europe/Berlin", "Berlin", "Germany", "UTC+1"},
    {"Asia/Tokyo", "Tokyo", "Japan", "UTC+9"},
    {"Asia/Shanghai", "Shanghai", "China", "UTC+8"},
    {"Australia/Sydney", "Sydney", "Australia", "UTC+10"},
    {NULL, NULL, NULL, NULL}
};

static void timezone_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(TimezoneScreen, timezone_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 timezone_screen_interface_init))

static void on_timezone_selected(GtkListBox *listbox, GtkListBoxRow *row, TimezoneScreen *self) {
    if (row) {
        gint index = gtk_list_box_row_get_index(row);
        if (index >= 0 && timezones[index].timezone) {
            g_free(self->selected_timezone);
            self->selected_timezone = g_strdup(timezones[index].timezone);
        }
    }
}

static GtkWidget *timezone_screen_create_widget(InstallerScreen *screen) {
    TimezoneScreen *self = TIMEZONE_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
    
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(self->widget, "installer-screen");
      // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 32);
    gtk_widget_set_margin_top(title_box, 24);
    
    GtkWidget *title = gtk_label_new("Select Your Timezone");
    gtk_widget_add_css_class(title, "welcome-title");
    
    GtkWidget *subtitle = gtk_label_new("Choose your timezone to set the correct system time.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Timezone list
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled, 500, 400);
    gtk_widget_set_halign(scrolled, GTK_ALIGN_CENTER);
    
    self->timezone_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(self->timezone_listbox), GTK_SELECTION_SINGLE);
    
    for (int i = 0; timezones[i].timezone != NULL; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
        gtk_widget_set_margin_start(row, 16);
        gtk_widget_set_margin_end(row, 16);
        gtk_widget_set_margin_top(row, 12);
        gtk_widget_set_margin_bottom(row, 12);
        
        GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        
        GtkWidget *city_label = gtk_label_new(timezones[i].city);
        gtk_widget_set_halign(city_label, GTK_ALIGN_START);
        
        gchar *detail_text = g_strdup_printf("%s • %s", timezones[i].country, timezones[i].offset);
        GtkWidget *detail_label = gtk_label_new(detail_text);
        gtk_widget_set_halign(detail_label, GTK_ALIGN_START);
        gtk_widget_add_css_class(detail_label, "text-muted");
        g_free(detail_text);
        
        gtk_box_append(GTK_BOX(info_box), city_label);
        gtk_box_append(GTK_BOX(info_box), detail_label);
        gtk_box_append(GTK_BOX(row), info_box);
        
        GtkWidget *list_row = gtk_list_box_row_new();
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(list_row), row);
        gtk_list_box_append(GTK_LIST_BOX(self->timezone_listbox), list_row);
    }
    
    g_signal_connect(self->timezone_listbox, "row-selected", 
                    G_CALLBACK(on_timezone_selected), self);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), self->timezone_listbox);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), scrolled);
    
    return self->widget;
}

static gboolean timezone_screen_validate(InstallerScreen *screen) {
    TimezoneScreen *self = TIMEZONE_SCREEN(screen);
    return self->selected_timezone != NULL;
}

static void timezone_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = timezone_screen_create_widget;
    iface->validate = timezone_screen_validate;
}

static void timezone_screen_finalize(GObject *object) {
    TimezoneScreen *self = TIMEZONE_SCREEN(object);
    g_free(self->selected_timezone);
    G_OBJECT_CLASS(timezone_screen_parent_class)->finalize(object);
}

static void timezone_screen_init(TimezoneScreen *self) {
    self->widget = NULL;
    self->selected_timezone = NULL;
}

static void timezone_screen_class_init(TimezoneScreenClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = timezone_screen_finalize;
}

TimezoneScreen *timezone_screen_new(void) {
    return g_object_new(TIMEZONE_TYPE_SCREEN, NULL);
}
