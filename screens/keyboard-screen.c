/* keyboard-screen.c - Keyboard layout selection screen implementation */
#include "keyboard-screen.h"
#include "../wave-installer.h"
#include <string.h>

typedef struct {
    const char *code;
    const char *name;
    const char *variants[10];
} KeyboardLayout;

static KeyboardLayout layouts[] = {
    {"us", "English (US)", {"", "intl", "alt-intl", "dvorak", "colemak", NULL}},
    {"gb", "English (UK)", {"", "intl", "dvorak", NULL}},
    {"de", "German", {"", "nodeadkeys", "neo", NULL}},
    {"fr", "French", {"", "nodeadkeys", "bepo", "oss", NULL}},
    {"es", "Spanish", {"", "nodeadkeys", "winkeys", NULL}},
    {"it", "Italian", {"", "nodeadkeys", "winkeys", NULL}},
    {"pt", "Portuguese", {"", "nodeadkeys", "nativo", NULL}},
    {"ru", "Russian", {"", "phonetic", "typewriter", NULL}},
    {"cn", "Chinese", {"", NULL}},
    {"jp", "Japanese", {"", NULL}},
    {"kr", "Korean", {"", NULL}},
    {"ar", "Arabic", {"", "azerty", "qwerty", NULL}},
    {"in", "Hindi", {"", "eng", NULL}},
    {"nl", "Dutch", {"", "nodeadkeys", NULL}},
    {"se", "Swedish", {"", "nodeadkeys", "dvorak", NULL}},
    {"dk", "Danish", {"", "nodeadkeys", NULL}},
    {"no", "Norwegian", {"", "nodeadkeys", "dvorak", NULL}},
    {"fi", "Finnish", {"", "nodeadkeys", NULL}},
    {"pl", "Polish", {"", "qwertz", "dvorak", NULL}},
    {"tr", "Turkish", {"", "f", "alt", NULL}},
    {NULL, NULL, {NULL}}
};

static char *selected_layout = NULL;
static char *selected_variant = NULL;
static GtkWidget *layout_box = NULL;
static GtkWidget *variant_box = NULL;
static GtkWidget *test_entry = NULL;

static void
populate_variants(const char *layout_code) {
    GtkWidget *child;
    GSList *group = NULL;
    int layout_index = -1;
    
    /* Clear existing variants */
    while ((child = gtk_widget_get_first_child(variant_box)) != NULL) {
        gtk_box_remove(GTK_BOX(variant_box), child);
    }
    
    /* Find layout */
    for (int i = 0; layouts[i].code != NULL; i++) {
        if (strcmp(layouts[i].code, layout_code) == 0) {
            layout_index = i;
            break;
        }
    }
    
    if (layout_index == -1) return;
    
    /* Add variant options */
    for (int i = 0; layouts[layout_index].variants[i] != NULL; i++) {
        const char *variant = layouts[layout_index].variants[i];
        const char *display_name = strlen(variant) > 0 ? variant : "Default";
        
        /* Variant item */
        GtkWidget *variant_item = gtk_toggle_button_new_with_label(display_name);
        gtk_widget_add_css_class(variant_item, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(variant_item), 
                                   group ? GTK_TOGGLE_BUTTON(group->data) : NULL);
        if (!group) group = g_slist_append(group, variant_item);
        
        /* Select default */
        if (i == 0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(variant_item), TRUE);
            if (selected_variant) g_free(selected_variant);
            selected_variant = g_strdup(variant);
        }
        
        /* Store variant */
        g_object_set_data_full(G_OBJECT(variant_item), "variant", g_strdup(variant), g_free);
        
        gtk_box_append(GTK_BOX(variant_box), variant_item);
    }
    
    g_slist_free(group);
}

static void
on_layout_selected(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *layout_code = (const char *)g_object_get_data(G_OBJECT(button), "layout-code");
        if (selected_layout) g_free(selected_layout);
        selected_layout = g_strdup(layout_code);
        
        /* Update variants */
        populate_variants(layout_code);
    }
}

static void
on_variant_selected(GtkToggleButton *button, gpointer user_data) {
    if (gtk_toggle_button_get_active(button)) {
        const char *variant = (const char *)g_object_get_data(G_OBJECT(button), "variant");
        if (selected_variant) g_free(selected_variant);
        selected_variant = g_strdup(variant);
    }
}

static void
on_next_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    if (selected_layout) {
        wave_installer_application_next_screen(app);
    }
}

static void
on_back_clicked(GtkButton *button, gpointer user_data) {
    WaveInstallerApplication *app = (WaveInstallerApplication *)user_data;
    wave_installer_application_previous_screen(app);
}

void
keyboard_screen_create(WaveInstallerApplication *app) {
    GtkWidget *container, *content, *title, *subtitle, *columns, *left_column, *right_column;
    GtkWidget *layout_label, *layout_scrolled, *variant_label, *variant_scrolled;
    GtkWidget *test_box, *test_label, *nav_box, *back_btn, *next_btn;
    GSList *group = NULL;
    
    /* Main container */
    container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_add_css_class(container, "screen-container");
    
    /* Content area */
    content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_widget_add_css_class(content, "screen-content");
    gtk_box_append(GTK_BOX(container), content);
    
    /* Title */
    title = gtk_label_new("Keyboard Layout");
    gtk_widget_add_css_class(title, "screen-title");
    gtk_box_append(GTK_BOX(content), title);
    
    /* Subtitle */
    subtitle = gtk_label_new("Select your keyboard layout and test it below");
    gtk_widget_add_css_class(subtitle, "screen-subtitle");
    gtk_box_append(GTK_BOX(content), subtitle);
    
    /* Two column layout */
    columns = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
    gtk_widget_set_hexpand(columns, TRUE);
    gtk_widget_set_vexpand(columns, TRUE);
    gtk_box_append(GTK_BOX(content), columns);
    
    /* Left column - Keyboard layouts */
    left_column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_hexpand(left_column, TRUE);
    gtk_box_append(GTK_BOX(columns), left_column);
    
    layout_label = gtk_label_new("Keyboard Layout");
    gtk_widget_add_css_class(layout_label, "section-title");
    gtk_widget_set_halign(layout_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(left_column), layout_label);
    
    /* Layout list */
    layout_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(layout_scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(layout_scrolled), 300);
    gtk_widget_set_vexpand(layout_scrolled, TRUE);
    gtk_box_append(GTK_BOX(left_column), layout_scrolled);
    
    layout_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(layout_box, "option-list");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(layout_scrolled), layout_box);
    
    /* Add layout options */
    for (int i = 0; layouts[i].code != NULL; i++) {
        GtkWidget *layout_item = gtk_toggle_button_new_with_label(layouts[i].name);
        gtk_widget_add_css_class(layout_item, "option-item");
        gtk_toggle_button_set_group(GTK_TOGGLE_BUTTON(layout_item), 
                                   group ? GTK_TOGGLE_BUTTON(group->data) : NULL);
        if (!group) group = g_slist_append(group, layout_item);
        
        /* Select default (US) */
        if (strcmp(layouts[i].code, "us") == 0) {
            gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(layout_item), TRUE);
            selected_layout = g_strdup(layouts[i].code);
        }
        
        /* Store layout code */
        g_object_set_data(G_OBJECT(layout_item), "layout-code", (gpointer)layouts[i].code);
        
        /* Connect signal */
        g_signal_connect(layout_item, "toggled", G_CALLBACK(on_layout_selected), app);
        
        gtk_box_append(GTK_BOX(layout_box), layout_item);
    }
    
    /* Right column - Variants and test */
    right_column = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_hexpand(right_column, TRUE);
    gtk_box_append(GTK_BOX(columns), right_column);
    
    variant_label = gtk_label_new("Layout Variant");
    gtk_widget_add_css_class(variant_label, "section-title");
    gtk_widget_set_halign(variant_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right_column), variant_label);
    
    /* Variant list */
    variant_scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(variant_scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(variant_scrolled), 200);
    gtk_widget_set_vexpand(variant_scrolled, TRUE);
    gtk_box_append(GTK_BOX(right_column), variant_scrolled);
    
    variant_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_add_css_class(variant_box, "option-list");
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(variant_scrolled), variant_box);
    
    /* Test area */
    test_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(test_box, 20);
    gtk_box_append(GTK_BOX(right_column), test_box);
    
    test_label = gtk_label_new("Test your keyboard:");
    gtk_widget_add_css_class(test_label, "form-label");
    gtk_widget_set_halign(test_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(test_box), test_label);
    
    test_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(test_entry), "Type here to test your keyboard layout...");
    gtk_widget_add_css_class(test_entry, "form-input");
    gtk_box_append(GTK_BOX(test_box), test_entry);
    
    /* Initialize variants for US layout */
    populate_variants("us");
    
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
    app->screens[SCREEN_KEYBOARD].widget = container;
    
    /* Connect variant selection signal */
    g_signal_connect_swapped(variant_box, "notify::visible-child", 
                           G_CALLBACK(on_variant_selected), app);
    
    /* Clean up */
    g_slist_free(group);
}
