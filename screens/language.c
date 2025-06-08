#include "language.h"

struct _LanguageScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *language_listbox;
    gchar *selected_language;
};

typedef struct {
    const gchar *code;
    const gchar *name;
    const gchar *native_name;
    const gchar *flag_icon;
} LanguageInfo;

static const LanguageInfo languages[] = {
    {"en_US", "English", "English", "🇺🇸"},
    {"es_ES", "Spanish", "Español", "🇪🇸"},
    {"fr_FR", "French", "Français", "🇫🇷"},
    {"de_DE", "German", "Deutsch", "🇩🇪"},
    {"it_IT", "Italian", "Italiano", "🇮🇹"},
    {"pt_BR", "Portuguese (Brazil)", "Português (Brasil)", "🇧🇷"},
    {"ru_RU", "Russian", "Русский", "🇷🇺"},
    {"zh_CN", "Chinese (Simplified)", "中文 (简体)", "🇨🇳"},
    {"ja_JP", "Japanese", "日本語", "🇯🇵"},
    {"ko_KR", "Korean", "한국어", "🇰🇷"},
    {NULL, NULL, NULL, NULL}
};

static void language_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(LanguageScreen, language_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 language_screen_interface_init))

static void on_language_selected(GtkListBox *listbox, GtkListBoxRow *row, LanguageScreen *self) {
    if (row) {
        gint index = gtk_list_box_row_get_index(row);
        if (index >= 0 && languages[index].code) {
            g_free(self->selected_language);
            self->selected_language = g_strdup(languages[index].code);
        }
    }
}

static GtkWidget *create_language_row(const LanguageInfo *lang) {
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(row, "language-item");
    gtk_widget_set_margin_start(row, 16);
    gtk_widget_set_margin_end(row, 16);
    gtk_widget_set_margin_top(row, 12);
    gtk_widget_set_margin_bottom(row, 12);
    
    // Flag emoji as icon
    GtkWidget *flag_label = gtk_label_new(lang->flag_icon);
    gtk_widget_set_margin_end(flag_label, 12);
    
    // Language names container
    GtkWidget *names_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    
    GtkWidget *name_label = gtk_label_new(lang->name);
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(name_label, "language-name");
    
    GtkWidget *native_label = gtk_label_new(lang->native_name);
    gtk_widget_set_halign(native_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(native_label, "language-native");
    gtk_widget_add_css_class(native_label, "text-muted");
    
    gtk_box_append(GTK_BOX(names_box), name_label);
    if (g_strcmp0(lang->name, lang->native_name) != 0) {
        gtk_box_append(GTK_BOX(names_box), native_label);
    }
    
    gtk_box_append(GTK_BOX(row), flag_label);
    gtk_box_append(GTK_BOX(row), names_box);
    
    return row;
}

static GtkWidget *language_screen_create_widget(InstallerScreen *screen) {
    LanguageScreen *self = LANGUAGE_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
    
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(self->widget, "installer-screen");
    
    // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 32);
    
    GtkWidget *icon = gtk_image_new_from_icon_name("preferences-desktop-locale");
    gtk_image_set_pixel_size(GTK_IMAGE(icon), 64);
    gtk_widget_set_margin_bottom(icon, 16);
    
    GtkWidget *title = gtk_label_new("Choose Your Language");
    gtk_widget_add_css_class(title, "welcome-title");
    
    GtkWidget *subtitle = gtk_label_new("Select the language you want to use during installation and as the default system language.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    gtk_label_set_wrap(GTK_LABEL(subtitle), TRUE);
    gtk_label_set_justify(GTK_LABEL(subtitle), GTK_JUSTIFY_CENTER);
    
    gtk_box_append(GTK_BOX(title_box), icon);
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Language list container
    GtkWidget *list_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(list_container, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(list_container, TRUE);
    
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled, 500, 400);
    gtk_widget_add_css_class(scrolled, "language-list");
    
    self->language_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(self->language_listbox), GTK_SELECTION_SINGLE);
    gtk_widget_add_css_class(self->language_listbox, "language-list");
    
    // Add languages to list
    for (int i = 0; languages[i].code != NULL; i++) {
        GtkWidget *row_widget = create_language_row(&languages[i]);
        GtkWidget *list_row = gtk_list_box_row_new();
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(list_row), row_widget);
        gtk_list_box_append(GTK_LIST_BOX(self->language_listbox), list_row);
    }
    
    // Select English by default
    gtk_list_box_select_row(GTK_LIST_BOX(self->language_listbox), 
                           gtk_list_box_get_row_at_index(GTK_LIST_BOX(self->language_listbox), 0));
    self->selected_language = g_strdup("en_US");
    
    g_signal_connect(self->language_listbox, "row-selected", 
                    G_CALLBACK(on_language_selected), self);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), self->language_listbox);
    gtk_box_append(GTK_BOX(list_container), scrolled);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), list_container);
    
    return self->widget;
}

static void language_screen_show_screen(InstallerScreen *screen) {
    // Called when screen becomes visible
}

static void language_screen_hide_screen(InstallerScreen *screen) {
    // Called when screen becomes hidden
}

static gboolean language_screen_validate(InstallerScreen *screen) {
    LanguageScreen *self = LANGUAGE_SCREEN(screen);
    return self->selected_language != NULL;
}

static void language_screen_next_clicked(InstallerScreen *screen) {
    LanguageScreen *self = LANGUAGE_SCREEN(screen);
    g_print("Selected language: %s\\n", self->selected_language ? self->selected_language : "none");
}

static void language_screen_back_clicked(InstallerScreen *screen) {
    // Handle back button click if needed
}

static void language_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = language_screen_create_widget;
    iface->show_screen = language_screen_show_screen;
    iface->hide_screen = language_screen_hide_screen;
    iface->validate = language_screen_validate;
    iface->next_clicked = language_screen_next_clicked;
    iface->back_clicked = language_screen_back_clicked;
}

static void language_screen_finalize(GObject *object) {
    LanguageScreen *self = LANGUAGE_SCREEN(object);
    g_free(self->selected_language);
    G_OBJECT_CLASS(language_screen_parent_class)->finalize(object);
}

static void language_screen_init(LanguageScreen *self) {
    self->widget = NULL;
    self->language_listbox = NULL;
    self->selected_language = NULL;
}

static void language_screen_class_init(LanguageScreenClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = language_screen_finalize;
}

LanguageScreen *language_screen_new(void) {
    return g_object_new(LANGUAGE_TYPE_SCREEN, NULL);
}
