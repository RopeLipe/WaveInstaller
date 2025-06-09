/* wave-installer.h - Main application header */
#ifndef WAVE_INSTALLER_H
#define WAVE_INSTALLER_H

#include <gtk/gtk.h>
#include <adwaita.h>

G_BEGIN_DECLS

#define WAVE_TYPE_INSTALLER_APPLICATION (wave_installer_application_get_type())
G_DECLARE_FINAL_TYPE(WaveInstallerApplication, wave_installer_application, WAVE, INSTALLER_APPLICATION, AdwApplication)

typedef enum {
    SCREEN_WELCOME = 0,
    SCREEN_LANGUAGE,
    SCREEN_TIMEZONE,
    SCREEN_KEYBOARD,
    SCREEN_DISK,
    SCREEN_NETWORK,
    SCREEN_USER,
    SCREEN_INSTALL,
    SCREEN_COUNT
} ScreenType;

typedef struct {
    GtkWidget *widget;
    const char *name;
    void (*create_fn)(WaveInstallerApplication *app);
} Screen;

struct _WaveInstallerApplication {
    AdwApplication parent_instance;
    
    GtkWindow *main_window;
    GtkBox *main_box;
    GtkWidget *current_screen;
    
    Screen screens[SCREEN_COUNT];
    int current_screen_index;
    
    /* Screen widgets */
    GtkWidget *welcome_screen;
    GtkWidget *language_screen;
    GtkWidget *timezone_screen;
    GtkWidget *keyboard_screen;
    GtkWidget *disk_screen;
    GtkWidget *network_screen;
    GtkWidget *user_screen;
    GtkWidget *install_screen;
};

WaveInstallerApplication *wave_installer_application_new(void);
void wave_installer_application_next_screen(WaveInstallerApplication *app);
void wave_installer_application_previous_screen(WaveInstallerApplication *app);
void wave_installer_application_show_screen(WaveInstallerApplication *app, int screen_index);
void wave_installer_application_quit_installer(WaveInstallerApplication *app);

G_END_DECLS

#endif /* WAVE_INSTALLER_H */
