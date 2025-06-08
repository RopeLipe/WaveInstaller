#ifndef INSTALL_SCREEN_H
#define INSTALL_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define INSTALL_TYPE_SCREEN (install_screen_get_type())
G_DECLARE_FINAL_TYPE(InstallScreen, install_screen, INSTALL, SCREEN, GObject)

InstallScreen *install_screen_new(void);

G_END_DECLS

#endif // INSTALL_SCREEN_H
