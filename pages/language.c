#include "../installer.h"

static GtkWidget* language_combo = NULL;
static GtkWidget* search_entry = NULL;

static void on_search_changed(GtkEditable* editable, gpointer user_data) {
    const char* search_text = gtk_editable_get_text(editable);
    // In a real implementation, this would filter the combo box
    // For now, we'll just print the search text
    g_print("Searching for: %s\n", search_text);
}

GtkWidget* create_language_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Select Language");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Choose your preferred language for the installation process.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_halign(content_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(content_box, TRUE);
    gtk_widget_set_vexpand(content_box, TRUE);
    
    // Search entry
    search_entry = gtk_search_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search languages...");
    gtk_widget_add_css_class(search_entry, "search-entry");
    g_signal_connect(search_entry, "search-changed", G_CALLBACK(on_search_changed), NULL);
    gtk_box_append(GTK_BOX(content_box), search_entry);
      // Scrollable language list
    GtkWidget* scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_widget_set_hexpand(scrolled, TRUE);
    gtk_widget_set_size_request(scrolled, -1, 300); // Set minimum height
    gtk_widget_add_css_class(scrolled, "language-list");
    
    GtkWidget* language_list_box = gtk_list_box_new();
    gtk_widget_add_css_class(language_list_box, "language-listbox");
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(language_list_box), GTK_SELECTION_SINGLE);
    
    // Add sample languages
    const char* languages[] = {
        "English (United States)",
        "English (United Kingdom)",
        "Deutsch (Deutschland)",
        "Français (France)",
        "Español (España)",
        "Italiano (Italia)",
        "Português (Brasil)",
        "Русский (Россия)",
        "中文 (简体)",
        "日本語 (日本)",
        "한국어 (대한민국)",
        "العربية",
        "हिन्दी (भारत)",
        "Nederlands (Nederland)",
        "Polski (Polska)",
        "Svenska (Sverige)",
        "Norsk (Norge)",
        "Dansk (Danmark)",
        "Suomi (Suomi)",
        "Ελληνικά (Ελλάδα)"
    };
    
    for (int i = 0; i < 20; i++) {        GtkWidget* row = gtk_list_box_row_new();
        gtk_widget_add_css_class(row, "language-row");
        
        // Create a more visually distinct row with icon
        GtkWidget* row_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
        gtk_widget_set_margin_top(row_box, 10);
        gtk_widget_set_margin_bottom(row_box, 10);
        gtk_widget_set_margin_start(row_box, 12);
        gtk_widget_set_margin_end(row_box, 12);
        
        // Add globe icon for languages
        GtkWidget* icon = gtk_image_new_from_icon_name("preferences-desktop-locale-symbolic");
        gtk_widget_set_margin_start(icon, 4);
        gtk_box_append(GTK_BOX(row_box), icon);
        
        GtkWidget* label = gtk_label_new(languages[i]);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_widget_set_hexpand(label, TRUE);
        gtk_box_append(GTK_BOX(row_box), label);

        // Add selection indicator
        GtkWidget* check = gtk_image_new_from_icon_name("emblem-ok-symbolic");
        gtk_widget_set_opacity(check, 0);
        gtk_widget_add_css_class(check, "selection-check");
        gtk_box_append(GTK_BOX(row_box), check);
        
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), row_box);
        
        gtk_list_box_append(GTK_LIST_BOX(language_list_box), row);
    }
    
    // Select first item by default
    gtk_list_box_select_row(GTK_LIST_BOX(language_list_box), 
                           gtk_list_box_get_row_at_index(GTK_LIST_BOX(language_list_box), 0));
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), language_list_box);
    gtk_box_append(GTK_BOX(content_box), scrolled);
    
    // Language info
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
    
    GtkWidget* info_label = gtk_label_new("You can change the language after installation in the system settings.");
    gtk_widget_add_css_class(info_label, "info-text");
    gtk_label_set_wrap(GTK_LABEL(info_label), TRUE);
    gtk_box_append(GTK_BOX(info_box), info_label);
    
    gtk_frame_set_child(GTK_FRAME(info_frame), info_box);
    gtk_box_append(GTK_BOX(content_box), info_frame);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    return page;
}
