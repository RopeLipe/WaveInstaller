#include "installer.h"
#include <stdio.h>

void apply_custom_css(void) {
    GtkCssProvider* provider = gtk_css_provider_new();
    GError* error = NULL;
    
    // Load CSS from external file
    const char* css_file_path = "/home/l/WaveInstaller/style.css";
    
    gtk_css_provider_load_from_path(provider, css_file_path);
    
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
}
