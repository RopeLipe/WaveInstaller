/* welcome-screen.h */
#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <gtk/gtk.h>
#include "../wave-installer.h"

G_BEGIN_DECLS

GtkWidget *welcome_screen_create(WaveInstallerApplication *app);

G_END_DECLS

#endif /* WELCOME_SCREEN_H */
