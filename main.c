#include <gtk/gtk.h>
#include "installer.h"

static void load_css(void) {
    GtkCssProvider *css_provider = gtk_css_provider_new();
    
    // Try to load from current directory first, then from system location
    GFile *css_file = g_file_new_for_path("style.css");
    
    if (!g_file_query_exists(css_file, NULL)) {
        g_object_unref(css_file);
        css_file = g_file_new_for_path("/usr/local/share/wave-installer/style.css");
    }
    
    if (!g_file_query_exists(css_file, NULL)) {
        g_warning("CSS file not found in current directory or system location");
        g_object_unref(css_file);
        g_object_unref(css_provider);
        return;
    }
    
    gtk_css_provider_load_from_file(css_provider, css_file);
    g_object_unref(css_file);
    
    // Apply CSS to default display
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display,
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_object_unref(css_provider);
}

static void activate(GtkApplication *app, gpointer user_data) {
    load_css();
    
    InstallerWindow *window = installer_window_new(app);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.wave.installer", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
