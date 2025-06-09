/* wave-installer.c - Main application implementation */
#include "wave-installer.h"
#include "screens/welcome-screen.h"
#include "screens/language-screen.h"
#include "screens/timezone-screen.h"
#include "screens/keyboard-screen.h"
#include "screens/disk-screen.h"
#include "screens/network-screen.h"
#include "screens/user-screen.h"
#include "screens/install-screen.h"

G_DEFINE_FINAL_TYPE(WaveInstallerApplication, wave_installer_application, ADW_TYPE_APPLICATION)

static void wave_installer_application_activate(GApplication *app);
static void load_css(WaveInstallerApplication *self);

WaveInstallerApplication *
wave_installer_application_new(void) {
    return g_object_new(WAVE_TYPE_INSTALLER_APPLICATION,
                       "application-id", "com.wave.installer",
                       NULL);
}

static void
wave_installer_application_class_init(WaveInstallerApplicationClass *klass) {
    GApplicationClass *app_class = G_APPLICATION_CLASS(klass);
    app_class->activate = wave_installer_application_activate;
}

static void
wave_installer_application_init(WaveInstallerApplication *self) {
    self->current_screen_index = 0;
    self->current_screen = NULL;
    
    /* Initialize screen definitions */
    self->screens[SCREEN_WELCOME] = (Screen){"Welcome", welcome_screen_create};
    self->screens[SCREEN_LANGUAGE] = (Screen){"Language", language_screen_create};
    self->screens[SCREEN_TIMEZONE] = (Screen){"Timezone", timezone_screen_create};
    self->screens[SCREEN_KEYBOARD] = (Screen){"Keyboard", keyboard_screen_create};
    self->screens[SCREEN_DISK] = (Screen){"Disk", disk_screen_create};
    self->screens[SCREEN_NETWORK] = (Screen){"Network", network_screen_create};
    self->screens[SCREEN_USER] = (Screen){"User", user_screen_create};
    self->screens[SCREEN_INSTALL] = (Screen){"Install", install_screen_create};
}

static void
wave_installer_application_activate(GApplication *app) {
    WaveInstallerApplication *self = WAVE_INSTALLER_APPLICATION(app);
    
    /* Load CSS */
    load_css(self);
    
    /* Create main window */
    self->main_window = GTK_WINDOW(gtk_application_window_new(GTK_APPLICATION(app)));
    gtk_window_set_title(self->main_window, "");
    gtk_window_set_default_size(self->main_window, 900, 600);
    gtk_window_set_resizable(self->main_window, FALSE);
    
    /* Remove titlebar */
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(headerbar), FALSE);
    gtk_widget_add_css_class(headerbar, "hidden-headerbar");
    gtk_window_set_titlebar(self->main_window, headerbar);
    
    /* Create main container */
    self->main_box = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));
    gtk_window_set_child(self->main_window, GTK_WIDGET(self->main_box));
    
    /* Show first screen */
    wave_installer_application_show_screen(self, 0);
    
    /* Present window */
    gtk_window_present(self->main_window);
}

static void
load_css(WaveInstallerApplication *self) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    
    /* Try to load CSS from different possible locations */
    const char *css_paths[] = {
        "resources/style.css",                           /* Development */
        "/usr/local/share/wave-installer/style.css",    /* Installed */
        "./style.css",                                   /* Current directory */
        NULL
    };
    
    gboolean loaded = FALSE;
    for (int i = 0; css_paths[i] != NULL && !loaded; i++) {
        GFile *css_file = g_file_new_for_path(css_paths[i]);
        if (g_file_query_exists(css_file, NULL)) {
            gtk_css_provider_load_from_file(provider, css_file, &error);
            if (!error) {
                loaded = TRUE;
                g_print("Loaded CSS from: %s\n", css_paths[i]);
            } else {
                g_warning("Failed to load CSS from %s: %s", css_paths[i], error->message);
                g_clear_error(&error);
            }
        }
        g_object_unref(css_file);
    }
    
    if (!loaded) {
        g_warning("Could not find CSS file in any expected location");
    }
    
    /* Apply CSS */
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    
    g_object_unref(provider);
}

void
wave_installer_application_show_screen(WaveInstallerApplication *self, int screen_index) {
    g_return_if_fail(screen_index >= 0 && screen_index < SCREEN_COUNT);
    
    /* Remove current screen */
    if (self->current_screen) {
        gtk_box_remove(self->main_box, self->current_screen);
    }
    
    /* Create new screen if needed */
    if (!self->screens[screen_index].widget) {
        self->screens[screen_index].create_fn(self);
    }
    
    /* Add new screen */
    self->current_screen = self->screens[screen_index].widget;
    self->current_screen_index = screen_index;
    gtk_box_append(self->main_box, self->current_screen);
}

void
wave_installer_application_next_screen(WaveInstallerApplication *self) {
    if (self->current_screen_index < SCREEN_COUNT - 1) {
        wave_installer_application_show_screen(self, self->current_screen_index + 1);
    }
}

void
wave_installer_application_previous_screen(WaveInstallerApplication *self) {
    if (self->current_screen_index > 0) {
        wave_installer_application_show_screen(self, self->current_screen_index - 1);
    }
}

void
wave_installer_application_quit_installer(WaveInstallerApplication *self) {
    g_application_quit(G_APPLICATION(self));
}
