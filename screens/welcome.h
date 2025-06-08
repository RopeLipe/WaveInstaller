#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <gtk/gtk.h>
#include "../installer.h"

G_BEGIN_DECLS

#define WELCOME_TYPE_SCREEN (welcome_screen_get_type())
G_DECLARE_FINAL_TYPE(WelcomeScreen, welcome_screen, WELCOME, SCREEN, GObject)

WelcomeScreen *welcome_screen_new(void);

G_END_DECLS

#endif // WELCOME_SCREEN_H
