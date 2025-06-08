#ifndef INSTALLER_H
#define INSTALLER_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

// Main installer window
#define INSTALLER_TYPE_WINDOW (installer_window_get_type())
G_DECLARE_FINAL_TYPE(InstallerWindow, installer_window, INSTALLER, WINDOW, GtkApplicationWindow)

InstallerWindow *installer_window_new(GtkApplication *app);

// Screen types
typedef enum {
    SCREEN_WELCOME,
    SCREEN_LANGUAGE,
    SCREEN_TIMEZONE,
    SCREEN_KEYBOARD,
    SCREEN_DISK,
    SCREEN_NETWORK,
    SCREEN_USER,
    SCREEN_INSTALL
} ScreenType;

// Base screen interface
#define INSTALLER_TYPE_SCREEN (installer_screen_get_type())
G_DECLARE_INTERFACE(InstallerScreen, installer_screen, INSTALLER, SCREEN, GObject)

struct _InstallerScreenInterface {
    GTypeInterface parent_iface;
    
    GtkWidget *(*create_widget)(InstallerScreen *self);
    void (*show_screen)(InstallerScreen *self);
    void (*hide_screen)(InstallerScreen *self);
    gboolean (*validate)(InstallerScreen *self);
    void (*next_clicked)(InstallerScreen *self);
    void (*back_clicked)(InstallerScreen *self);
};

GtkWidget *installer_screen_create_widget(InstallerScreen *self);
void installer_screen_show_screen(InstallerScreen *self);
void installer_screen_hide_screen(InstallerScreen *self);
gboolean installer_screen_validate(InstallerScreen *self);
void installer_screen_next_clicked(InstallerScreen *self);
void installer_screen_back_clicked(InstallerScreen *self);

// Individual screens
#include "screens/welcome.h"
#include "screens/language.h"
#include "screens/timezone.h"
#include "screens/keyboard.h"
#include "screens/disk.h"
#include "screens/network.h"
#include "screens/user.h"
#include "screens/install.h"

G_END_DECLS

#endif // INSTALLER_H
