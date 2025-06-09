#include "installer.h"

// Global variables
GtkWidget* main_window = NULL;
GtkWidget* main_stack = NULL;
GtkWidget* navigation_box = NULL;

void create_installer_window(GtkApplication *app) {
    // Apply custom CSS first
    apply_custom_css();    // Create main window
    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Wave Installer");
    gtk_window_set_decorated(GTK_WINDOW(main_window), FALSE);
    
    // Set a default size and allow proper resizing
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(main_window), TRUE);
      // Create main container
    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(main_window), main_box);
    
    // Create content area with padding
    GtkWidget* content_frame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(content_frame, "main-frame");
    gtk_widget_set_hexpand(content_frame, TRUE);
    gtk_widget_set_vexpand(content_frame, TRUE);
    gtk_box_append(GTK_BOX(main_box), content_frame);
      GtkWidget* content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(content_box, 20);
    gtk_widget_set_margin_bottom(content_box, 20);
    gtk_widget_set_margin_start(content_box, 40);
    gtk_widget_set_margin_end(content_box, 40);
    gtk_frame_set_child(GTK_FRAME(content_frame), content_box);
    
    // Create stack for pages
    main_stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(main_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(main_stack), 300);
    gtk_widget_set_vexpand(main_stack, TRUE);
    gtk_box_append(GTK_BOX(content_box), main_stack);
    
    // Create navigation area
    navigation_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
    gtk_widget_set_halign(navigation_box, GTK_ALIGN_END);
    gtk_box_append(GTK_BOX(content_box), navigation_box);
    
    // Add all pages to stack
    gtk_stack_add_named(GTK_STACK(main_stack), create_welcome_page(), "welcome");
    gtk_stack_add_named(GTK_STACK(main_stack), create_language_page(), "language");
    gtk_stack_add_named(GTK_STACK(main_stack), create_timezone_page(), "timezone");
    gtk_stack_add_named(GTK_STACK(main_stack), create_keyboard_page(), "keyboard");
    gtk_stack_add_named(GTK_STACK(main_stack), create_disk_page(), "disk");
    gtk_stack_add_named(GTK_STACK(main_stack), create_network_page(), "network");
    gtk_stack_add_named(GTK_STACK(main_stack), create_user_page(), "user");    // Set initial page and present window
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), "welcome");
    setup_navigation_buttons(NULL, "welcome");
    
    gtk_window_present(GTK_WINDOW(main_window));
}

void navigate_to_page(const char* page_name) {
    gtk_stack_set_visible_child_name(GTK_STACK(main_stack), page_name);
    setup_navigation_buttons(NULL, page_name);
}

void setup_navigation_buttons(GtkWidget* page, const char* current_page) {
    // Clear existing buttons
    GtkWidget* child = gtk_widget_get_first_child(navigation_box);
    while (child) {
        GtkWidget* next = gtk_widget_get_next_sibling(child);
        gtk_box_remove(GTK_BOX(navigation_box), child);
        child = next;
    }
    
    // Back button (if not on welcome page)
    if (g_strcmp0(current_page, "welcome") != 0) {
        GtkWidget* back_button = gtk_button_new_with_label("Back");
        gtk_widget_add_css_class(back_button, "secondary-button");
        gtk_box_append(GTK_BOX(navigation_box), back_button);
        
        // Connect back button signal based on current page
        if (g_strcmp0(current_page, "language") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "welcome");
        } else if (g_strcmp0(current_page, "timezone") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "language");
        } else if (g_strcmp0(current_page, "keyboard") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "timezone");
        } else if (g_strcmp0(current_page, "disk") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "keyboard");
        } else if (g_strcmp0(current_page, "network") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "disk");
        } else if (g_strcmp0(current_page, "user") == 0) {
            g_signal_connect_swapped(back_button, "clicked", G_CALLBACK(navigate_to_page), "network");
        }
    }
    
    // Next button
    GtkWidget* next_button;
    if (g_strcmp0(current_page, "user") == 0) {
        next_button = gtk_button_new_with_label("Install");
    } else {
        next_button = gtk_button_new_with_label("Next");
    }
    gtk_widget_add_css_class(next_button, "primary-button");
    gtk_box_append(GTK_BOX(navigation_box), next_button);
    
    // Connect next button signal based on current page
    if (g_strcmp0(current_page, "welcome") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "language");
    } else if (g_strcmp0(current_page, "language") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "timezone");
    } else if (g_strcmp0(current_page, "timezone") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "keyboard");
    } else if (g_strcmp0(current_page, "keyboard") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "disk");
    } else if (g_strcmp0(current_page, "disk") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "network");
    } else if (g_strcmp0(current_page, "network") == 0) {
        g_signal_connect_swapped(next_button, "clicked", G_CALLBACK(navigate_to_page), "user");
    }
}

GtkWidget* create_rounded_frame(GtkWidget* child) {
    GtkWidget* frame = gtk_frame_new(NULL);
    gtk_widget_add_css_class(frame, "rounded-frame");
    gtk_frame_set_child(GTK_FRAME(frame), child);
    return frame;
}
