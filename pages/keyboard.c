#include "../installer.h"

static GtkWidget* keyboard_combo = NULL;
static GtkWidget* test_entry = NULL;

static void on_keyboard_layout_changed(GtkComboBox* combo, gpointer user_data) {
    int active = gtk_combo_box_get_active(combo);
    g_print("Selected keyboard layout index: %d\n", active);
}

GtkWidget* create_keyboard_page(void) {
    GtkWidget* page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 32);
    gtk_widget_set_valign(page, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(page, GTK_ALIGN_FILL);
    
    // Page header
    GtkWidget* header_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(header_box, GTK_ALIGN_CENTER);
    
    GtkWidget* title = gtk_label_new("Keyboard Layout");
    gtk_widget_add_css_class(title, "page-title");
    gtk_box_append(GTK_BOX(header_box), title);
    
    GtkWidget* subtitle = gtk_label_new("Select your keyboard layout and test it below.");
    gtk_widget_add_css_class(subtitle, "page-subtitle");
    gtk_box_append(GTK_BOX(header_box), subtitle);
    
    gtk_box_append(GTK_BOX(page), header_box);
    
    // Content area
    GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_set_halign(content_box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(content_box, TRUE);
    
    // Keyboard layout selection
    GtkWidget* layout_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    
    GtkWidget* layout_label = gtk_label_new("Keyboard Layout:");
    gtk_widget_set_halign(layout_label, GTK_ALIGN_START);
    gtk_widget_add_css_class(layout_label, "field-label");
    gtk_box_append(GTK_BOX(layout_box), layout_label);
    
    keyboard_combo = gtk_combo_box_text_new();
    gtk_widget_add_css_class(keyboard_combo, "keyboard-combo");
    g_signal_connect(keyboard_combo, "changed", G_CALLBACK(on_keyboard_layout_changed), NULL);
    
    // Add keyboard layouts
    const char* layouts[] = {
        "English (US)",
        "English (UK)",
        "German",
        "French",
        "Spanish",
        "Italian",
        "Portuguese",
        "Russian",
        "Chinese (Simplified)",
        "Japanese",
        "Korean",
        "Arabic",
        "Hindi",
        "Dutch",
        "Polish",
        "Swedish",
        "Norwegian",
        "Danish",
        "Finnish",
        "Greek"
    };
    
    for (int i = 0; i < 20; i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(keyboard_combo), layouts[i]);
    }
    
    gtk_combo_box_set_active(GTK_COMBO_BOX(keyboard_combo), 0);
    gtk_box_append(GTK_BOX(layout_box), keyboard_combo);
    
    gtk_box_append(GTK_BOX(content_box), layout_box);
    
    // Keyboard test area
    GtkWidget* test_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(test_frame, "test-frame");
    
    GtkWidget* test_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(test_box, 20);
    gtk_widget_set_margin_bottom(test_box, 20);
    gtk_widget_set_margin_start(test_box, 20);
    gtk_widget_set_margin_end(test_box, 20);
    
    GtkWidget* test_title = gtk_label_new("Test Your Keyboard");
    gtk_widget_add_css_class(test_title, "test-title");
    gtk_widget_set_halign(test_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(test_box), test_title);
    
    GtkWidget* test_description = gtk_label_new("Type in the box below to test your keyboard layout:");
    gtk_widget_add_css_class(test_description, "test-description");
    gtk_widget_set_halign(test_description, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(test_box), test_description);
    
    test_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(test_entry), "Type here to test your keyboard...");
    gtk_widget_add_css_class(test_entry, "test-entry");
    gtk_box_append(GTK_BOX(test_box), test_entry);
    
    // Sample text to test special characters
    GtkWidget* sample_label = gtk_label_new("Try typing: @ # $ % ^ & * ( ) [ ] { } | \\ / ? < >");
    gtk_widget_add_css_class(sample_label, "sample-text");
    gtk_widget_set_halign(sample_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(test_box), sample_label);
    
    gtk_frame_set_child(GTK_FRAME(test_frame), test_box);
    gtk_box_append(GTK_BOX(content_box), test_frame);
    
    // Layout preview (placeholder)
    GtkWidget* preview_frame = create_rounded_frame(NULL);
    gtk_widget_add_css_class(preview_frame, "preview-frame");
    
    GtkWidget* preview_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_top(preview_box, 16);
    gtk_widget_set_margin_bottom(preview_box, 16);
    gtk_widget_set_margin_start(preview_box, 16);
    gtk_widget_set_margin_end(preview_box, 16);
    gtk_widget_set_halign(preview_box, GTK_ALIGN_CENTER);
    
    GtkWidget* preview_title = gtk_label_new("Layout Preview");
    gtk_widget_add_css_class(preview_title, "preview-title");
    gtk_box_append(GTK_BOX(preview_box), preview_title);
    
    // Simplified keyboard representation
    GtkWidget* keyboard_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(keyboard_grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(keyboard_grid), 4);
    gtk_widget_set_halign(keyboard_grid, GTK_ALIGN_CENTER);
    
    // First row keys
    const char* first_row[] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"};
    for (int i = 0; i < 10; i++) {
        GtkWidget* key = gtk_label_new(first_row[i]);
        gtk_widget_add_css_class(key, "keyboard-key");
        gtk_widget_set_size_request(key, 30, 30);
        gtk_grid_attach(GTK_GRID(keyboard_grid), key, i, 0, 1, 1);
    }
    
    // Second row keys
    const char* second_row[] = {"A", "S", "D", "F", "G", "H", "J", "K", "L"};
    for (int i = 0; i < 9; i++) {
        GtkWidget* key = gtk_label_new(second_row[i]);
        gtk_widget_add_css_class(key, "keyboard-key");
        gtk_widget_set_size_request(key, 30, 30);
        gtk_grid_attach(GTK_GRID(keyboard_grid), key, i, 1, 1, 1);
    }
    
    gtk_box_append(GTK_BOX(preview_box), keyboard_grid);
    
    gtk_frame_set_child(GTK_FRAME(preview_frame), preview_box);
    gtk_box_append(GTK_BOX(content_box), preview_frame);
    
    gtk_box_append(GTK_BOX(page), content_box);
    
    return page;
}
