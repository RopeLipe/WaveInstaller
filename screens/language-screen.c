/* language-screen.c - Language selection screen implementation */
#include "language-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *code;
    const char *name;
    const char *native;
    const char *flag;
} Language;

static Language languages[] = {
    {"en_US", "English", "English", "🇺🇸"},
    {"es_ES", "Spanish", "Español", "🇪🇸"},
    {"fr_FR", "French", "Français", "🇫🇷"},
    {"de_DE", "German", "Deutsch", "🇩🇪"},
    {"it_IT", "Italian", "Italiano", "🇮🇹"},
    {"pt_PT", "Portuguese", "Português", "🇵🇹"},
    {"ru_RU", "Russian", "Русский", "🇷🇺"},
    {"zh_CN", "Chinese (Simplified)", "简体中文", "🇨🇳"},
    {"zh_TW", "Chinese (Traditional)", "繁體中文", "🇹🇼"},
    {"ja_JP", "Japanese", "日本語", "🇯🇵"},
    {"ko_KR", "Korean", "한국어", "🇰🇷"},
    {"ar_SA", "Arabic", "العربية", "🇸🇦"},
    {"hi_IN", "Hindi", "हिन्दी", "🇮🇳"},
    {"nl_NL", "Dutch", "Nederlands", "🇳🇱"},
    {"sv_SE", "Swedish", "Svenska", "🇸🇪"},
    {"da_DK", "Danish", "Dansk", "🇩🇰"},
    {"no_NO", "Norwegian", "Norsk", "🇳🇴"},
    {"fi_FI", "Finnish", "Suomi", "🇫🇮"},
    {"pl_PL", "Polish", "Polski", "🇵🇱"},
    {"tr_TR", "Turkish", "Türkçe", "🇹🇷"},
    {NULL, NULL, NULL, NULL}
};

static char *selected_language = NULL;

static void
on_language_selected(GtkToggleButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    
    if (gtk_toggle_button_get_active(button)) {
        const char *lang_code = (const char *)g_object_get_data(G_OBJECT(button), "lang-code");
        if (selected_language) {
            g_free(selected_language);
        }
        selected_language = g_strdup(lang_code);
    }
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    if (selected_language) {
        wave_installer_application_next_screen(app);
    }
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

void
language_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *scrolled, *lang_box, *nav_box, *back_btn, *next_btn;
    GtkWidget *lang_item, *lang_content, *flag_label, *name_box, *name_label, *native_label;
    GSList *group = NULL;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Choose Your Language");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Select your preferred language for the installation");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Scrolled window for languages */
    scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scrolled), 400);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_box_append(GTK_BOX(content), scrolled);
    
    /* Language list */
    lang_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(lang_box, "option-list");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), lang_box);
    
    /* Add language options */
    for (int i = 0; languages[i].code != NULL; i++) {
        /* Language item container */
        lang_item = gtk_toggle_button_new();
        gtk_widget_add_css_class(lang_item, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(lang_item), 
                                   group ? GTK_TOGGLE_BUTTON(group->data) : NULL);
        if (!group) group = g_slist_append(group, lang_item);
        
        /* Set default selection (English) */
        if (strcmp(languages[i].code, "en_US") == 0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(lang_item), TRUE);
            selected_language = g_strdup(languages[i].code);
        }
        
        /* Language content */
        lang_content = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
        gtk_button_set_child(GTK_BUTTON(lang_item), lang_content);
        
        /* Flag */
        flag_label = gtk_label_new(languages[i].flag);
        gtk_widget_add_css_class(flag_label, "flag-emoji");
        gtk_box_append(GTK_BOX(lang_content), flag_label);
        
        /* Name box */
        name_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
        gtk_box_append(GTK_BOX(lang_content), name_box);
        
        /* Language name */
        name_label = gtk_label_new(languages[i].name);
        gtk_widget_add_css_class(name_label, "option-title");
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(name_box), name_label);
        
        /* Native name */
        native_label = gtk_label_new(languages[i].native);
        gtk_widget_add_css_class(native_label, "option-subtitle");
        gtk_widget_set_halign(native_label, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(name_box), native_label);
        
        /* Store language code */
        g_object_set_data(G_OBJECT(lang_item), "lang-code", (gpointer)languages[i].code);
        
        /* Connect signal */
        g_signal_connect(lang_item, "toggled", G_CALLBACK(on_language_selected), app);
        
        gtk_box_append(GTK_BOX(lang_box), lang_item);
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
    app->screens[SCREEN_LANGUAGE].widget = container;
    
    /* Clean up */
    g_slist_free(group);
}
