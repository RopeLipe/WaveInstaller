#include "installer.h"
#include <stdio.h>

void apply_custom_css(void) {
    GtkCssProvider* provider = gtk_css_provider_new();
    GError* error = NULL;
    
    // Load CSS from external file
    const char* css_file_path = "/home/l/WaveInstaller/style.css";
    
    if (!gtk_css_provider_load_from_path(provider, css_file_path, &error)) {
        g_warning("Failed to load CSS from %s: %s", css_file_path, error->message);
        g_error_free(error);
        
        // Fallback: load minimal inline CSS
        const char* fallback_css = 
            ".main-frame { background: @theme_bg_color; border-radius: 12px; }\n"
            ".primary-button { background: #0066cc; color: white; border-radius: 6px; padding: 12px 24px; }\n"
            ".secondary-button { background: @theme_base_color; border: 1px solid @borders; border-radius: 6px; padding: 12px 24px; }\n";
        
        gtk_css_provider_load_from_data(provider, fallback_css, -1);
        g_print("Using fallback CSS styling\n");
    } else {
        g_print("Successfully loaded CSS from %s\n", css_file_path);
    }
    
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
}
