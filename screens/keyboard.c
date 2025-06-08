#include "keyboard.h"

struct _KeyboardScreen {
    GObject parent_instance;
    GtkWidget *widget;
    GtkWidget *layout_listbox;
    GtkWidget *test_entry;
    gchar *selected_layout;
};

typedef struct {
    const gchar *layout;
    const gchar *name;
    const gchar *preview;
} KeyboardLayout;

static const KeyboardLayout layouts[] = {
    {"us", "US English", "QWERTY"},
    {"gb", "UK English", "QWERTY"},
    {"fr", "French", "AZERTY"},
    {"de", "German", "QWERTZ"},
    {"es", "Spanish", "QWERTY"},
    {"it", "Italian", "QWERTY"},
    {"dvorak", "Dvorak", "',.PY"},
    {NULL, NULL, NULL}
};

static void keyboard_screen_interface_init(InstallerScreenInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE(KeyboardScreen, keyboard_screen, G_TYPE_OBJECT,
                             G_IMPLEMENT_INTERFACE(INSTALLER_TYPE_SCREEN,
                                                 keyboard_screen_interface_init))

static void on_layout_selected(GtkListBox *listbox, GtkListBoxRow *row, KeyboardScreen *self) {
    if (row) {
        gint index = gtk_list_box_row_get_index(row);
        if (index >= 0 && layouts[index].layout) {
            g_free(self->selected_layout);
            self->selected_layout = g_strdup(layouts[index].layout);
        }
    }
}

static GtkWidget *keyboard_screen_create_widget(InstallerScreen *screen) {
    KeyboardScreen *self = KEYBOARD_SCREEN(screen);
    
    if (self->widget)
        return self->widget;
    
    self->widget = gtk_box_new(GTK_ORIENTATION_VERTICAL, 24);
    gtk_widget_add_css_class(self->widget, "installer-screen");
      // Title section
    GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_halign(title_box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom(title_box, 32);
    gtk_widget_set_margin_top(title_box, 24);
    
    GtkWidget *title = gtk_label_new("Select Keyboard Layout");
    gtk_widget_add_css_class(title, "welcome-title");
    
    GtkWidget *subtitle = gtk_label_new("Choose your keyboard layout for typing in your language.");
    gtk_widget_add_css_class(subtitle, "welcome-subtitle");
    
    gtk_box_append(GTK_BOX(title_box), title);
    gtk_box_append(GTK_BOX(title_box), subtitle);
    
    // Main content area
    GtkWidget *content_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 32);
    gtk_widget_set_halign(content_box, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(content_box, TRUE);
    
    // Layout list
    GtkWidget *list_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    
    GtkWidget *list_label = gtk_label_new("Available Layouts");
    gtk_widget_add_css_class(list_label, "text-muted");
    gtk_widget_set_halign(list_label, GTK_ALIGN_START);
    
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled, 300, 300);
    
    self->layout_listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(self->layout_listbox), GTK_SELECTION_SINGLE);
    
    for (int i = 0; layouts[i].layout != NULL; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
        gtk_widget_set_margin_start(row, 16);
        gtk_widget_set_margin_end(row, 16);
        gtk_widget_set_margin_top(row, 12);
        gtk_widget_set_margin_bottom(row, 12);
        
        GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
        
        GtkWidget *name_label = gtk_label_new(layouts[i].name);
        gtk_widget_set_halign(name_label, GTK_ALIGN_START);
        
        GtkWidget *preview_label = gtk_label_new(layouts[i].preview);
        gtk_widget_set_halign(preview_label, GTK_ALIGN_START);
        gtk_widget_add_css_class(preview_label, "text-muted");
        
        gtk_box_append(GTK_BOX(info_box), name_label);
        gtk_box_append(GTK_BOX(info_box), preview_label);
        gtk_box_append(GTK_BOX(row), info_box);
        
        GtkWidget *list_row = gtk_list_box_row_new();
        gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(list_row), row);
        gtk_list_box_append(GTK_LIST_BOX(self->layout_listbox), list_row);
    }
    
    // Select US layout by default
    gtk_list_box_select_row(GTK_LIST_BOX(self->layout_listbox), 
                           gtk_list_box_get_row_at_index(GTK_LIST_BOX(self->layout_listbox), 0));
    self->selected_layout = g_strdup("us");
    
    g_signal_connect(self->layout_listbox, "row-selected", 
                    G_CALLBACK(on_layout_selected), self);
    
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), self->layout_listbox);
    gtk_box_append(GTK_BOX(list_container), list_label);
    gtk_box_append(GTK_BOX(list_container), scrolled);
    
    // Test area
    GtkWidget *test_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    
    GtkWidget *test_label = gtk_label_new("Test Your Keyboard");
    gtk_widget_add_css_class(test_label, "text-muted");
    gtk_widget_set_halign(test_label, GTK_ALIGN_START);
    
    GtkWidget *test_frame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(test_frame, "keyboard-preview");
    gtk_widget_set_size_request(test_frame, 300, 200);
    
    GtkWidget *test_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_start(test_box, 24);
    gtk_widget_set_margin_end(test_box, 24);
    gtk_widget_set_margin_top(test_box, 24);
    gtk_widget_set_margin_bottom(test_box, 24);
    
    GtkWidget *test_instruction = gtk_label_new("Type here to test the selected keyboard layout:");
    gtk_widget_set_halign(test_instruction, GTK_ALIGN_START);
    gtk_widget_add_css_class(test_instruction, "text-muted");
    
    self->test_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(self->test_entry), "Test typing here...");
    gtk_widget_add_css_class(self->test_entry, "form-entry");
    
    gtk_box_append(GTK_BOX(test_box), test_instruction);
    gtk_box_append(GTK_BOX(test_box), self->test_entry);
    gtk_frame_set_child(GTK_FRAME(test_frame), test_box);
    
    gtk_box_append(GTK_BOX(test_container), test_label);
    gtk_box_append(GTK_BOX(test_container), test_frame);
    
    gtk_box_append(GTK_BOX(content_box), list_container);
    gtk_box_append(GTK_BOX(content_box), test_container);
    
    gtk_box_append(GTK_BOX(self->widget), title_box);
    gtk_box_append(GTK_BOX(self->widget), content_box);
    
    return self->widget;
}

static gboolean keyboard_screen_validate(InstallerScreen *screen) {
    KeyboardScreen *self = KEYBOARD_SCREEN(screen);
    return self->selected_layout != NULL;
}

static void keyboard_screen_interface_init(InstallerScreenInterface *iface) {
    iface->create_widget = keyboard_screen_create_widget;
    iface->validate = keyboard_screen_validate;
}

static void keyboard_screen_finalize(GObject *object) {
    KeyboardScreen *self = KEYBOARD_SCREEN(object);
    g_free(self->selected_layout);
    G_OBJECT_CLASS(keyboard_screen_parent_class)->finalize(object);
}

static void keyboard_screen_init(KeyboardScreen *self) {
    self->widget = NULL;
    self->selected_layout = NULL;
}

static void keyboard_screen_class_init(KeyboardScreenClass *klass) {
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->finalize = keyboard_screen_finalize;
}

KeyboardScreen *keyboard_screen_new(void) {
    return g_object_new(KEYBOARD_TYPE_SCREEN, NULL);
}
